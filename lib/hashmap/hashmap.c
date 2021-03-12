/**
 * @file hashmap.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the hashmap
 * @copyright Copyright (c) 2021
 */

#include "hashmap.h"

#include <types.h>

/**
 * @brief A hashing function for a char array/string
 * Code taken from http://www.cse.yorku.ca/~oz/hash.html (djb2)
 * @param str The string to hash
 * @return unsigned long The hash
 */
u_int64_t hash_djb2(uchar *str) {
    u_int64_t hash = 5381;
    u_int32_t c;

    while ((c = *str++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * @brief A hashing function for a char array/string
 * Code taken from http://www.cse.yorku.ca/~oz/hash.html (sdbm)
 * @param str The string to hash
 * @return unsigned long The hash
 */
u_int64_t hash_sdbm(uchar *str) {
    u_int64_t hash = 0;
    u_int32_t c;

    while ((c = *str++)) { hash = c + (hash << 6) + (hash << 16) - hash; }

    return hash;
}