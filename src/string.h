#ifndef __STRING_H__
#define __STRING_H__


/**
 * Get the string length
 *
 * @param string The input string
 *
 * @return The length of the input string
 */
int get_string_length(const char* string);

/**
 * Safe convertion for char* to unsigned int.
 * The input char* to be converted can be in base 16 (0x[0-9A-Fa-f]+),
 * base 8 (0[0-7]+) or base 10 ([0-9]+)
 *
 * @param src The source string to be converted
 * @param dest The destination converted value
 * @param min_value The minimum value that can assume dest
 * @param max_value The maximum value that can assume dest
 *
 * @return Status:
 *   0 => Success
 *   -1 => Invalid input argument
 *   -2 => Convertion failure
 *   -3 => The input argument (src) to be converted isn't in accordance with
 *     the validity range
 */
int str_to_uint(const char* src, unsigned int min_value, unsigned int max_value,
    unsigned int* dest);


#endif

