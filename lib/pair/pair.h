/**
 * @file pair.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for the pair data structure
 * @copyright Copyright (c) 2021
 */

#ifndef PAIR_H
#define PAIR_H

#include <error_handling.h>
#include <stdlib.h>
#include <string.h>
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
 * @param pair The pair of the two strings (pointer)
 * @return int32_t The return code (0 for no errors)
 */
int32_t make_spair(string s1, string s2, StringsPair *pair);

/**
 * @brief Copy-Construct a string pair from another
 * @param p1 The source pair (value)
 * @param p2 The target pair (pointer)
 * @return int32_t The return code (0 for no errors)
 */
int32_t copy_spair(StringsPair source, StringsPair *target);

/**
 * @brief Free strings inside a pair
 * @param p The pair (pointer)
 * @return int32_t The return code (0 for no errors)
 */
int32_t clear_spair(StringsPair *p);

#endif