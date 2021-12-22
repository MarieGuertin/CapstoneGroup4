/*
 * qspi_handler.h
 *
 *  Created on: Dec. 19, 2021
 *      Author: Christian Martel
 */

#ifndef INC_QSPI_HANDLER_H_
#define INC_QSPI_HANDLER_H_

void qspi_read(uint8_t* buffer_ptr, uint32_t address, uint32_t size);
void qspi_write(uint8_t* buffer_ptr, uint32_t address, uint32_t size);
void qspi_init();
void qspi_erase_block(uint32_t block_address);
void qspi_erase_blocks(uint32_t start_address, uint32_t n);


#endif /* INC_QSPI_HANDLER_H_ */
