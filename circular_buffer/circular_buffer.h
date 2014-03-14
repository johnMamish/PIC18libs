/**
 * @file        circular_buffer.h
 *
 * @brief       Contains function prototypes and struct definitions for my
 *              circular buffer library.
 */

#include <stdint.h>

#ifndef CIRCULAR_BUFFER_H
#define	CIRCULAR_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t* base;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
}CircularBuffer;

typedef enum
{
    NO_ERROR,
    BUFFER_OVERFLOW,
    BUFFER_UNDERFLOW,
    UNKNOWN_ERROR
}CircularBufferError;

uint8_t readCircularBuffer(CircularBuffer*, CircularBufferError*);
void writeCircularBuffer(CircularBuffer*, uint8_t, CircularBufferError*);
inline uint8_t circularBufferFull(CircularBuffer*);
inline uint8_t circularBufferEmpty(CircularBuffer*);

#ifdef	__cplusplus
}
#endif

#endif	/* CIRCULAR_BUFFER_H */

