/**
 * @file pair.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the pair data structure
 * @copyright Copyright (c) 2021
 */

#include "pair.h"

int32_t make_spair(string first, string second, StringsPair *pair) {
    pair->first = strcpy(malloc(strlen(first) + 1), first);
    pair->second = strcpy(malloc(strlen(second) + 1), second);

    if (pair->first == 0 || pair->second == 0) {
        /* Mallocs failed */
        CERR(TRUE, "Couldn't create pair");
        return MALLOC_ERR;
    }
    return 0;
}

int32_t copy_spair(StringsPair source, StringsPair *target) {
    target->first = strcpy(malloc(strlen(source.first) + 1), source.first);
    target->second = strcpy(malloc(strlen(source.second) + 1), source.second);

    if (target->first == 0 || target->second == 0) {
        /* Mallocs failed */
        CERR(TRUE, "Couldn't create pair");
        return MALLOC_ERR;
    }

    return 0;
}

int32_t clear_spair(StringsPair *p) {
    free(p->first);
    free(p->second);
    return 0;
}