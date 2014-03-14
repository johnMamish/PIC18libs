/**
 * @file        UART_lib.h
 *
 * @brief       Contains function prototypes for UART library.
 */

#include <stdint.h>
#include <circular_buffer.h>

#ifndef UART_LIB_H
#define	UART_LIB_H

#ifdef	__cplusplus
//extern "C" {
#endif

/**
 * TODO: make an errors enum.
 */

typedef enum
{
    BPS_4800,
    BPS_9600,
    BPS_19200,
    BPS_38400,
    BPS_57600,
    BPS_115200
}Brate;

void initUART();
void writeUART(uint8_t data, uint8_t* error);
void writeUARTMessage(const char* msg, uint8_t* error);
uint8_t readUART(uint8_t*);
void UARTISR();

#ifdef	__cplusplus
//}
#endif

#endif	/* UART_LIB_H */

