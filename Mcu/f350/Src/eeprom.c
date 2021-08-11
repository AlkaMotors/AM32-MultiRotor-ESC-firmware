/*
 *
 *  Created on: Mar. 25, 2020
 *      Author: Alka
 *
 */

#include <string.h>


//#define APP_START (uint32_t)0x08001000
//#define FLASH_STORAGE 0x08005000  // at the 31kb mark
//#define PAGE_SIZE 0x800           // 1 kb for f051

uint32_t FLASH_FKEY1 = 0x45670123;
uint32_t FLASH_FKEY2 = 0xCDEF89AB;

void save_flash_nolib(eeprom_t *data, void *eeprom_address)
{
    // unlock flash
    while ((FLASH->SR & FLASH_SR_BSY) != 0) { /*  add time-out*/ }
    if ((FLASH->CR & FLASH_CR_LOCK) != 0) {
        FLASH->KEYR = FLASH_FKEY1;
        FLASH->KEYR = FLASH_FKEY2;
    }

    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = (uint32_t)eeprom_address;
    FLASH->CR |= FLASH_CR_STRT;
    while ((FLASH->SR & FLASH_SR_BSY) != 0) { /*  add time-out */ }
    if ((FLASH->SR & FLASH_SR_EOP) != 0){
        FLASH->SR = FLASH_SR_EOP;
    } else {
        /* error */
    }
    FLASH->CR &= ~FLASH_CR_PER;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress-of-packed-member"
    volatile uint16_t *data_ptr = (uint16_t *)data;
    #pragma GCC diagnostic pop

    uint16_t *data_ptr_end = (uint16_t *)((uint8_t *)data + sizeof(*data));
    uint16_t *flash_ptr = (uint16_t *)eeprom_address;

    while (data_ptr < data_ptr_end) {

        FLASH->CR |= FLASH_CR_PG; /* (1) */
        *flash_ptr = *data_ptr;
        while ((FLASH->SR & FLASH_SR_BSY) != 0) { /*  add time-out  */ }

        if ((FLASH->SR & FLASH_SR_EOP) != 0) {
            FLASH->SR = FLASH_SR_EOP;
        } else {
            /*  error  */
        }
        FLASH->CR &= ~FLASH_CR_PG;

        data_ptr += 1;
        flash_ptr += 1;
    }

    SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

void read_eeprom(eeprom_t *data, uint8_t *eeprom_address)
{
    memcpy(data, eeprom_address, sizeof(*data));
}
void read_flash_bin(uint8_t *data, uint8_t *address)
{
    memcpy(data, address, sizeof(*data));
}

