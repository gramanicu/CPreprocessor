/**
 * @file pair.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for the pair data structure
 * @copyright Copyright (c) 2021
 */

#ifndef PAIR_H
#define PAIR_H

#include <types.h>

/**
 * @brief A pair data structure, that stores two strings/char arrays
 */
typedef struct _StringsPair {
    string first;
    string second;
} StringsPair;

/**
 * @brief Creates a pair from two strings
 * @param s1 First string
 * @param s2 Second string
 * @return StringsPair The pair of the two strings
 */
StringsPair make_spair(string s1, string s2);

#endif    // PAIR_H