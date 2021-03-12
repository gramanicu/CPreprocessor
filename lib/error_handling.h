/**
 * @file error_handling.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief Functions used to handle errors and debug the code
 * @copyright Copyright (c) 2021
 */
#include <errno.h>
#include <stdio.h>

#include "types.h"

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

// Enable/Disable debug mode (debug messages)
#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

/**
 * @brief If the assertion is true, print error information
 * and exit the program, throwing the error code
 */
#define DIE(assertion, call_description)                       \
    do {                                                       \
        if (assertion) {                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
            perror(call_description);                          \
            exit(errno);                                       \
        }                                                      \
    } while (0)

/**
 * @brief The assertion must be true. If not, print error information. Does not
 * stop the execution
 * @param assertion The condition (a boolean expression is evaluated to 0 or 1)
 * @param msg The message to show
 */
static inline void CERR(bint assertion, char* msg) {
    if (assertion) {
        fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);
        perror(msg);
    }
}

/**
 * @brief Print the debug message (debug mode must be active)
 * @param msg The message
 */
static inline void DEBUG_MSG(char* msg) {
    if (DEBUG_MODE) { fprintf(stderr, "%s\n", msg); }
}

#endif    // ERROR_HANDLING_H
