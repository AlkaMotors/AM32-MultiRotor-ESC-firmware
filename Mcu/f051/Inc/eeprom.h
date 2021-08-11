/*
 *
 *  Created on: Mar. 25, 2020
 *      Author: Alka
 */
#include "mcu_main.h"
#include "eeprom_common.h"

#pragma once

void save_flash_nolib(eeprom_t *data, void *eeprom_address);
void read_eeprom(eeprom_t *data, uint8_t *eeprom_address);
void read_flash_bin(uint8_t *data, uint8_t *address);
