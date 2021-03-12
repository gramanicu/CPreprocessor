/**
 * @file pair.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the pair data structures
 * @copyright Copyright (c) 2021
 */

#include "pair.h"

StringsPair make_spair(string first, string second) {
    return (StringsPair){first, second};
}