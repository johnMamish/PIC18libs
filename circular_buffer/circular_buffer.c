/**
 * @file        circular_buffer.c
 */

#include "circular_buffer.h"

/**
 * @brief       Returns the next value in the circular buffer and advances the
 *              circular buffer, thereby discarding the element that was just
 *              read.
 *
 * If there is no value to be read, 0x00 will be returned and the value pointed
 * by error will be nonzero.
 *
 * @param buf is the circular buffer to be read.
 *
 * @param error points to a location where the success/fail status of this
 *              operation may be placed.
 *
 * @return      Returns the next value to be read in the buffer.
 *
 * If an error occurs, this value will be zero.
 */
uint8_t readCircularBuffer(CircularBuffer* buf, CircularBufferError* error)
{
    //check for buffer underflow
    if(circularBufferEmpty(buf))
    {
        *error = BUFFER_UNDERFLOW;
        return 0x00;
    }

    //if there was no buffer underflow, return the next element and then advance
    //the tail of the circular buffer.
    char temp = *(buf->base + buf->tail);
    buf->tail++;

    //if the tail has reached the boundary of the buffer, loop around.
    if(buf->tail >= buf->size)
    {
        buf->tail = 0;
    }

    //indicate no error and return the result.
    *error = NO_ERROR;
    return temp;
}

/**
 * @brief       Writes a value to a circular buffer and advances the head.
 *
 * If there is a buffer overflow error and no data may be written, the value
 * pointed by error will change to reflect the error and no data will be
 * written.
 *
 * @param buf points to the circular buffer to write to.
 *
 * @param data is the byte of data to be written to the circular buffer.
 *
 * @param error points to space where an error code may be stored.
 */
void writeCircularBuffer(CircularBuffer* buf, uint8_t data, CircularBufferError* error)
{
    //check for buffer overflow
    if(circularBufferFull(buf))
    {
        *error = BUFFER_OVERFLOW;
        return;
    }

    //if there was no buffer overflow, write to the buffer and the advance
    //the head of the buffer.
    *(buf->base + buf->head) = data;
    buf->head++;

    //check to see if the buffer head has to be wrapped around.
    if(buf->head >= buf->size)
    {
        buf->head = 0;
    }

    //indicate that everything was error-free
    *error = NO_ERROR;
}

/**
 * @brief       Tells whether a circular buffer is full or not.
 *
 * @param buf points to the circular buffer to test.
 *
 * @return      Returns zero if the circular buffer is not full and a nonzero
 *              value if it is full.
 */
inline uint8_t circularBufferFull(CircularBuffer* buf)
{
    uint16_t headTemp = buf->head;
    if(++headTemp >= buf->size)
    {
        headTemp = 0;
    }
    return (headTemp == buf->tail);
}

/**
 * @brief       Tells whether a circular buffer is empty or not.
 *
 * A buffer is empty when
 * tail+1
 * @param buf
 * @return
 */
inline uint8_t circularBufferEmpty(CircularBuffer* buf)
{
    return (buf->head == buf->tail);
}