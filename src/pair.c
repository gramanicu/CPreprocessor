/**
 * @file pair.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the pair data structure
 * @copyright Copyright (c) 2021
 */

#include "pair.h"

int32_t make_spair(string first, string second, struct StringsPair *pair) {
    pair->first = strcpy(calloc(1, strlen(first) + 1), first);
    pair->second = strcpy(calloc(1, strlen(second) + 1), second);

    if (pair->first == NULL || pair->second == NULL) {
        /* Mallocs failed */
        CERR(TRUE, "Couldn't create pair");
        return MALLOC_ERR;
    }
    return 0;
}

int32_t copy_spair(struct StringsPair source, struct StringsPair *target) {
    target->first = strcpy(calloc(1, strlen(source.first) + 1), source.first);
    target->second =
        strcpy(calloc(1, strlen(source.second) + 1), source.second);

    if (target->first == NULL || target->second == NULL) {
        /* Mallocs failed */
        CERR(TRUE, "Couldn't create pair");
        return MALLOC_ERR;
    }

    return 0;
}

int32_t clear_spair(struct StringsPair *p) {
    free(p->first);
    free(p->second);
    return 0;
}