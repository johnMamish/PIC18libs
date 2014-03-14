/**
 * @file        M74HC164B1_shift_register.h
 * @author      John Mamish
 *
 * @brief       Library for using an M74HC164B1 shift register.
 */

#ifndef STD_STRING_H
#define	STD_STRING_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

inline uint8_t std_string_ascii_digit_to_int(uint8_t digit);
inline uint8_t std_string_int_to_ascii_digit(uint8_t num);
uint8_t std_string_num_length(uint32_t n, uint8_t base);
void std_string_uint_to_string(char* target, uint32_t n, uint8_t base);
void std_string_str_cpy(char* src, char* dest);
int std_string_strcmp(char* a, char* b);

#ifdef	__cplusplus
}
#endif

#endif	/* M74HC164B1_SHIFT_REGISTER_H */

