/*
 * bootloader.h
 *
 *  Created on: Mar. 25, 2020
 *      Author: Alka
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "main.h"

//void save_to_flash(uint8_t *data);
//void read_flash(uint8_t* data, uint32_t address);
//void save_to_flash_bin(uint8_t *data, int length, uint32_t add);
void read_flash_bin(uint8_t*  data, uint32_t add, int  out_buff_len);
void save_flash_nolib(uint8_t* data, int length, uint32_t add);

#endif /* INC_EEPROM_H_ */
