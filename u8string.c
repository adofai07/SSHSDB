#include <string.h>
#include <stdlib.h>

#include "u8string.h"

/// @brief Checks if byte is a continuation byte
/// @param c target byte
/// @return 1 if byte is a continuation byte, 0 elsewise
int is_continuation(char c) {
    return (c & 0xc0) == 0x80;
}

/// @brief Checks if character is single-byte
/// @param c target character
/// @return 1 if character is single-byte (0xxx xxxx)
int is_single_byte(char *c) {
    return (c[0] & 0x80) == 0x0;
}

/// @brief Checks if character is double-byte
/// @param c target character
/// @return 1 if character is double-byte (110x xxxx and continuation byte)
int is_double_byte(char *c) {
    return (c[0] & 0xe0) == 0xc0 && 
           is_continuation(c[1]);
}

/// @brief Checks if character is triple-byte
/// @param c target character
/// @return 1 if character is triple-byte (1110 xxxx and continuation byte)
int is_triple_byte(char *c) {
    return (c[0] & 0xf0) == 0xe0 &&
           is_continuation(c[1]) &&
           is_continuation(c[2]);
}

/// @brief Checks if character is quadruple-byte
/// @param c target character
/// @return 1 if character is quadruple-byte (1111 0xxx and continuation byte)
int is_quadruple_byte(char *c) {
    return (c[0] & 0xf8) == 0xf0 &&
           is_continuation(c[1]) &&
           is_continuation(c[2]) &&
           is_continuation(c[3]);
}

/// @brief Returns the number of bytes in a utf-8 character
/// @param c target character
/// @return number of bytes in a utf-8 character
int num_bytes(char *c) {
    int len = strlen(c);
    int num_bytes = 0;
    
    if (len >= 1 && is_single_byte(c)) {
        num_bytes = 1;
        
    } else if (len >= 2 && is_double_byte(c)) {
        num_bytes = 2;
        
    } else if (len >= 3 && is_triple_byte(c)) {
        num_bytes = 3;
        
    } else if (len >= 4 && is_quadruple_byte(c)) {
        num_bytes = 4;
    }
    
    return num_bytes;
}

/// @brief utf-8 counterpart of strlen()
/// @param s target string
/// @return number of utf-8 characters in the string
int u8strlen(const char *s) {
    int len = 0;

    while (*s) {
        if ((*s & 0xC0) != 0x80) len++;
        s++;
    }

    return len;
}

/// @brief utf-8 counterpart of strcpy()
/// @param dest source string
/// @param src destination string
/// @return pointer to destination string
char *u8strcpy(char *dest, const char *src) {
    return u8strncpy(dest, src, strlen(src));
}

/// @brief utf-8 counterpart of strncpy()
/// @param dest source string
/// @param src destination string
/// @param n number of bytes to copy
/// @return pointer to destination string
char *u8strncpy(char *dest, const char *src, int n) {
    int k = n - 1;
    int i;

    if (n) {
        dest[k] = 0;
        strncpy(dest, src, n);

        // If last byte was truncated
        if (dest[k] & 0x80) {
            for (i = k; (i > 0) && (k - i < 3) && ((dest[i] & 0xC0) == 0x80); i--);

            switch (k - i) {
                case 0:                               dest[i] = '\0'; break;
                case 1: if ((dest[i] & 0xE0) != 0xC0) dest[i] = '\0'; break;
                case 2: if ((dest[i] & 0xF0) != 0xE0) dest[i] = '\0'; break;
                case 3: if ((dest[i] & 0xF8) != 0xF0) dest[i] = '\0'; break;
            }
        }
    }

    return dest;
}

/// @brief Removes a character from a utf-8 string
/// @param s target string
/// @param n nth byte to remove
/// @return new string
char *u8_remove_char(char *s, int n) {
    int len = strlen(s);

    // n cannot be bigger then the string length
    if (len < n) {
        exit(EXIT_FAILURE);
    }
    
    int n_bytes = num_bytes(s + n);
    char *new_string = (char *) malloc(len * sizeof(char));

    if (new_string == NULL) {
        exit(EXIT_FAILURE);
    }
    
    memcpy(new_string, s, n);
    memcpy(new_string + n, s + n + n_bytes, len - n - n_bytes + 1);
    
    return new_string;
}

/// @brief Adds a character to a utf-8 string
/// @param s target string
/// @param c character to add
/// @param n nth byte to add
/// @return new string
char *u8_add_char(char *s, char *c, int n) {
    int len = strlen(s);

    // n cannot be bigger then the string length
    if (len < n) {
        exit(EXIT_FAILURE);
    }
    
    int n_bytes = num_bytes(c);
    char *new_string = (char *) malloc((len + n_bytes + 1) * sizeof(char));

    if (new_string == NULL) {
        exit(EXIT_FAILURE);
    }
    
    memcpy(new_string, s, n);
    memcpy(new_string + n, c, n_bytes);
    memcpy(new_string + n + n_bytes, s + n, len - n + 1);
    
    return new_string;
}
