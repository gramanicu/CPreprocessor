/**
 * @file utils.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief Different macro utilities
 * @copyright Copyright (c) 2021
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Macro that executes the next instruction/block of code forever (or
 * until a break)
 */
#define FOREVER while (1)

/**
 * @brief Macro to do nothing
 */
#define NOTHING \
    do {        \
    } while (0)

#endif    // UTILS_H