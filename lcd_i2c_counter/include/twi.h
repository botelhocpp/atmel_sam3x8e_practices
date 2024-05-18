#ifndef _TWI_H_
#define _TWI_H_

#include <sam3x8e.h>

#include <stdint.h>
#include <stdbool.h>

/* General Functions */

void TWI_Init(Twi *twi);

void TWI_StartWrite(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size);

void TWI_StartRead(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size);

void TWI_Stop(Twi *twi);

bool TWI_ByteTransmitted(Twi *twi);

bool TWI_ByteReceived(Twi *twi);

bool TWI_TransferCompleted(Twi *twi);

bool TWI_WaitByteTransfer(Twi *twi);

bool TWI_WaitByteReceive(Twi *twi);

bool TWI_WaitTransferComplete(Twi *twi);

void TWI_Write(Twi *twi, uint8_t byte);

uint8_t TWI_Read(Twi *twi);

/* Polling Functions */

void TWI_WriteByte(Twi *twi, uint8_t address, uint8_t data);

uint8_t TWI_ReadByte(Twi *twi, uint8_t address);

void TWI_WriteByteRegister(Twi *twi, uint8_t address, uint8_t register_address, uint8_t data);

uint8_t TWI_ReadByteRegister(Twi *twi, uint8_t address, uint8_t register_address);

bool TWI_WriteData(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size, const uint8_t *data, uint32_t length);

bool TWI_ReadData(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size, uint8_t *data, uint32_t length);

#endif
