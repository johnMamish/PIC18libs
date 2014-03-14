/**
 * @file        UART_lib.c
 *
 * @brief       Library for interrupt-driven UART communication
 */

#include <xc.h>
#include <htc.h>
#include <UART_lib.h>

#define TRANSMIT_BUFFER_SIZE 0x80
#define RECEIVE_BUFFER_SIZE 0x20

uint8_t transmitData[TRANSMIT_BUFFER_SIZE];
uint8_t receiveData[RECEIVE_BUFFER_SIZE];

CircularBuffer transmitBuffer = {&transmitData, TRANSMIT_BUFFER_SIZE, 0, 0};
CircularBuffer receiveBuffer = {&receiveData, RECEIVE_BUFFER_SIZE, 0, 0};

/**
 * @brief       Initializes UART with a transmit rate of 9600 bps (assuming a
 *              clock speed of 16MHz.)
 */
void initUART()
{
    //set up the baud rate generator.
    BAUDCONbits.BRG16 = 0b0;        //only use 8 bits to generate baud rate.
    BAUDCONbits.DTRXP = 0b0;
    TXSTAbits.BRGH = 0;             //don't make baud rate super fast.
    SPBRG = 25;                     //9600 bps assuming a system clock of 16 MHz.

    //set up TRIS registers.  Even though this sets both as input, the FRM says
    //to set both as 1.  When RCSTAbits.SPEN is set from 0 to 1, these pins will
    //automatically be reconfigured.
    TRISBbits.RB5 = 0b1;
    TRISBbits.RB7 = 0b1;

    //enable UART peripheral and disable synchronious communication.
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;

    //8 bits, not 9.
    TXSTAbits.TX9 = 0;
    RCSTAbits.RX9 = 0;

    //enable constant receive.
    RCSTAbits.CREN = 0b1;

    //enable recieve interrupt
    PIE1bits.RCIE = 0b1;
    
    //make sure global peripheral interrupt is enabled.
    INTCONbits.PEIE = 0b1;
}

/**
 * @brief       Writes a byte of data to the UART buffer to be written over the
 *              UART connection by the UART interrupt routine.
 * 
 * Enables and disables UART interrupts as necessary.
 * 
 * @param data is the byte to write to the UART buffer.
 */
void writeUART(uint8_t data, uint8_t* error)
{
    //attempt to write to the buffer.
    CircularBufferError e;
    writeCircularBuffer(&transmitBuffer, data, &e);
    
    //if e returns an error, return with an error.
    if(e != NO_ERROR)
    {
        *error = 0x01;
        return;
    }
    
    //otherwise, set interrupts, and return.
    PIE1bits.TXIE = 0b1;
}

/**
 * @brief       Puts a message in the UART buffer and enables UART transmit
 *              interrupts, if necessary.
 *
 * @param msg is the string to copy into the UART buffer.
 *
 * @param error points to space for an error code to be stored
 */
void writeUARTMessage(const char* msg, uint8_t* error)
{
    CircularBufferError e;

    for(char* i = (char*)msg;*i;i++)
    {
        while(circularBufferFull(&transmitBuffer));
        writeCircularBuffer(&transmitBuffer, (uint8_t)*i, &e);

        if(e != NO_ERROR)
        {
            *error = 0x01;
        }

        PIE1bits.TXIE = 0b1;
    }
}

/**
 * @brief       This routine should be called to access the read buffer.
 */
uint8_t readUART(uint8_t* error)
{
    //attempt to read from the buffer.
    CircularBufferError e;

    uint8_t ret = readCircularBuffer(&receiveBuffer, &e);

    if(e != NO_ERROR)
    {
        *error = 0x01;
        return 0x00;
    }

    *error = 0x00;
    return ret;
}

/**
 * @brief       This routine should be called during an ISR in order to see
 *              if the UART must be serviced.
 */
void UARTISR()
{
    //first, check to see if UART caused the interrupt.
    if(PIE1bits.TXIE && PIR1bits.TXIF)
    {
        CircularBufferError e;

        if(!circularBufferEmpty(&transmitBuffer))
        {
            //if the circular buffer is not empty, put a char in the transmit
            //buffer.
            TXREG = readCircularBuffer(&transmitBuffer, &e);
        }
        else
        {
            //if the circular buffer is empty, disable the interrupt.
            PIE1bits.TXIE = 0b0;
        }
    }
    if(PIE1bits.RCIE && PIR1bits.RCIF)
    {
        CircularBufferError e;

        if(!circularBufferFull(&receiveBuffer))
        {
            writeCircularBuffer(&receiveBuffer, RCREG, &e);
        }
    }
}