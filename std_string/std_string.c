/**
 * @file        std_string.c
 *
 * @author      John Mamish
 *
 * @brief       Library of standard functions for dealing with strings
 *              and conversions between strings and numbers
 */

#include <stdint.h>
#include "std_string.h"

/**
 * @brief       Given a digit encoded in ascii, returns the corresponding
 *              integer value.
 *
 * For instance, '9' returns 9 and 'a' returns 10.
 *
 * @param digit - the digit to convert.
 *
 * @return      Returns the integer value of the given digit.
 */
inline uint8_t std_string_ascii_digit_to_int(uint8_t digit)
{
    digit -= '0';

    if(digit <= 9)
    {
        return digit;
    }

    digit -= ('A' - ('9' + 1));
    if((digit >= 0x0a) && (digit <= 0x0f))
    {
        return digit;
    }

    digit -= ('a' - 'A');
    if((digit >= 0x0a) && (digit <= 0x0f))
    {
        return digit;
    }

    return 0xff;
}

inline uint8_t std_string_int_to_ascii_digit(uint8_t num)
{
    num += '0';

    if((num >= '0') && (num <= '9'))
    {
        return num;
    }

    num += ('a' - ('9' + 1));

    if((num >= 'a') && (num <= 'f'))
    {
        return num;
    }

    return 0xff;
}

uint8_t std_string_num_length(uint32_t n, uint8_t base)
{
    uint8_t length = 0;
    do
    {
        n /= base;
        length++;
    }while(n);

    return length;
}

void std_string_uint_to_string(char* target, uint32_t n, uint8_t base)
{
    char* head = target + std_string_num_length(n, base);
    *head = '\0';
    head--;

    do
    {
        *head = std_string_int_to_ascii_digit(n % base);
        head--;
        n /= base;
    }while(n);
}

void std_string_str_cpy(char* src, char* dest)
{
    while(*src)
    {
        *(dest++) = *(src++);
    }
}

//reverses the block of memory starting at src that is len bytes long.
void std_string_reverse_mem(char* src, int len)
{
    char* lower = src;
    char* upper = src + len - 1;

    for(; lower < upper; lower++, upper--)
    {
        char temp = *lower;
        *lower = *upper;
        *upper = temp;
    }
}

int std_string_strcmp(char* a, char* b)
{
    while((*a == *b) && *b)
    {
        a++;
        b++;
    }

    if(*a > *b)
    {
        return 1;
    }
    else if(*b > *a)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}