#include "twi.h"

#include <sam3x8e.h>

/* Private Functions */

static inline void TWI_Start(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size, bool read_operation) {
    twi->TWI_MMR = (address << TWI_MMR_DADR_Pos) | (read_operation << 12);

    if(register_address_size != 0) {
        twi->TWI_MMR |= (register_address_size << TWI_MMR_IADRSZ_Pos);
        twi->TWI_IADR = register_address;
    }

    twi->TWI_CR = read_operation;
}

static inline bool TWI_OperationFailed(Twi *twi) {
    return (twi->TWI_SR & TWI_SR_NACK) == TWI_SR_NACK;
}

/* General Functions */

void TWI_StartWrite(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size) {
    TWI_Start(twi, address, register_address, register_address_size, false);
}

void TWI_StartRead(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size) {
    TWI_Start(twi, address, register_address, register_address_size, true);
}

void TWI_Stop(Twi *twi) {
    twi->TWI_CR |= TWI_CR_STOP;
}

bool TWI_ByteTransmitted(Twi *twi) {
    return (twi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY;
}

bool TWI_ByteReceived(Twi *twi) {
    return (twi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY;
}

bool TWI_TransferCompleted(Twi *twi) {
    return (twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP;
}

bool TWI_WaitByteTransfer(Twi *twi) {
    while(!TWI_ByteTransmitted(twi)) {
        if(TWI_OperationFailed(twi)) {
            return false;
        }
    }
    return true;
}

bool TWI_WaitByteReceive(Twi *twi) {
    while(!TWI_ByteReceived(twi)) {
        if(TWI_OperationFailed(twi)) {
            return false;
        }
    }
    return true;
}

bool TWI_WaitTransferComplete(Twi *twi) {
    while(!TWI_TransferCompleted(twi)) {
        if(TWI_OperationFailed(twi)) {
            return false;
        }
    }
    return true;
}

void TWI_Write(Twi *twi, uint8_t byte) {
    twi->TWI_THR |= byte;
}

uint8_t TWI_Read(Twi *twi) {
    return twi->TWI_RHR;
}

/* General Functions */

void TWI_Init(Twi *twi) {
    if(twi == TWI0) {
        PMC->PMC_PCER0 |= PMC_PCER0_PID22;
        PIOA->PIO_PDR |= PIO_PA17 | PIO_PA18;
        PIOA->PIO_ABSR &= ~PIO_PA17 & ~PIO_PA18;
        PIOA->PIO_PUDR |= PIO_PA17 | PIO_PA18;
    }
    else if(twi == TWI1) {
        PMC->PMC_PCER0 |= PMC_PCER0_PID23;
        PIOB->PIO_PDR |= PIO_PB12 | PIO_PB13;
        PIOB->PIO_ABSR &= ~PIO_PB12 & ~PIO_PB13;
        PIOB->PIO_PUDR |= PIO_PB12 | PIO_PB13;
    }
    else {
        while(1);
    }

    /* Disable TWI */
    twi->TWI_CR |= TWI_CR_MSDIS | TWI_CR_SVDIS;

    /* Enable Master Mode */
    twi->TWI_CR |= TWI_CR_MSEN;

    /* Configure the TWCK line to 100kHz */
    twi->TWI_CWGR |= TWI_CWGR_CLDIV(13)
        | TWI_CWGR_CHDIV(13)
        | TWI_CWGR_CKDIV(5);
}

/* Polling Functions */

void TWI_WriteByte(Twi *twi, uint8_t address, uint8_t data) {
    while(!TWI_WriteData(twi, address, 0, 0, &data, 1));
}

uint8_t TWI_ReadByte(Twi *twi, uint8_t address) {
    uint8_t data = 0;
    while(!TWI_ReadData(twi, address, 0, 0, &data, 1));
    return data;
}

void TWI_WriteByteRegister(Twi *twi, uint8_t address, uint8_t register_address, uint8_t data) {
    while(!TWI_WriteData(twi, address, register_address, 1, &data, 1));
}

uint8_t TWI_ReadByteRegister(Twi *twi, uint8_t address, uint8_t register_address) {
    uint8_t data = 0;
    while(!TWI_ReadData(twi, address, register_address, 1, &data, 1));
    return data;
}

bool TWI_WriteData(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size, const uint8_t *data, uint32_t length) {
    TWI_StartWrite(twi, address, register_address, register_address_size);

    for(int i = 0; i < length; i++) {
        TWI_Write(twi, data[i]);
        if(!TWI_WaitByteTransfer(twi)) {
            return false;
        }
    } 
    TWI_Stop(twi);
    return TWI_WaitTransferComplete(twi);
}

bool TWI_ReadData(Twi *twi, uint8_t address, uint8_t register_address, uint32_t register_address_size, uint8_t *data, uint32_t length) {
    TWI_StartRead(twi, address, register_address, register_address_size);

    for(int i = 0; i < length; i++) {
        if(i + 1 == length) {
            TWI_Stop(twi);
        }
        if(!TWI_WaitByteReceive(twi)) {
            return false;
        }
        data[i] = TWI_Read(twi);
    }  
    
    return TWI_WaitTransferComplete(twi);
}
