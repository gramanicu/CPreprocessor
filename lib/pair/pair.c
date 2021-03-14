/**
 * @file pair.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the pair data structure
 * @copyright Copyright (c) 2021
 */

#include "pair.h"

void make_spair(string first, string second, StringsPair *pair) {
    pair->first = malloc(strlen(first) + 1);
    pair->second = malloc(strlen(second) + 1);

    strcpy(pair->first, first);
    strcpy(pair->second, second);
}

void copy_spair(StringsPair source, StringsPair *target) {
    target->first = malloc(strlen(source.first) + 1);
    target->second = malloc(strlen(source.second) + 1);

    strcpy(target->first, source.first);
    strcpy(target->second, source.second);
}

void clear_spair(StringsPair *p) {
    free(p->first);
    free(p->second);
}