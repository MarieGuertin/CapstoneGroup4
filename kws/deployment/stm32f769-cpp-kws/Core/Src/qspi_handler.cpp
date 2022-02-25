/*
 * qspi_handler.c
 *
 *  Created on: Dec. 19, 2021
 *      Author: Christian Martel
 */
#include <stdint.h>
#include "stm32f769i_discovery_qspi.h"
#include "main.h"
#include <stdlib.h>

void qspi_read(uint8_t* buffer_ptr, uint32_t address, uint32_t size) {
	if (BSP_QSPI_Read(buffer_ptr, address, size) != QSPI_OK)
		Error_Handler();
}

void qspi_write(uint8_t* buffer_ptr, uint32_t address, uint32_t size) {
	if(BSP_QSPI_Write(buffer_ptr, address, size) != QSPI_OK){
		Error_Handler();
	}
}

void qspi_init() {
	BSP_QSPI_Init();
}

void qspi_erase_block(uint32_t block_address) {
	BSP_QSPI_Erase_Block(block_address);
}

void qspi_erase_blocks(uint32_t start_address, uint32_t n){
	for(uint8_t i = 0; i < n; i++){
		qspi_erase_block(start_address + i * MX25L512_SUBSECTOR_SIZE);
	}
}

