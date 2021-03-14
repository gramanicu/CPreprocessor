/**
 * @file list.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for the list data structure
 * @copyright Copyright (c) 2021
 */

#ifndef LIST_H
#define LIST_H

#include <error_handling.h>
#include <pair/pair.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>

// A "constructor" for the list
#define INIT_PAIRLIST                                               \
    {                                                               \
        0, 0, pairlist_search, pairlist_push_back, pairlist_remove, \
            pairlist_clear, pairlist_print                          \
    }

typedef struct PairListElem {
    struct PairListElem *prev;
    struct PairListElem *next;
    StringsPair data;
} PairListElem;

typedef struct PairList {
    PairListElem *_head;
    u_int32_t _size;

    void (*search)(struct PairList *const this, string key, StringsPair *pair);
    void (*insert)(struct PairList *const this, StringsPair pair);
    void (*remove)(struct PairList *const this, string key);
    void (*clear)(struct PairList *const this);
    void (*print)(struct PairList *const this);
} PairList;

// -- PairList "Methods" --

/**
 * @brief Search for a strings pair in the list.
 * @param this The list this function is attached to
 * @param key The key of the searched pair
 * @param pair The "returned" StringsPair with the required key. Returns a empty
 * pair if the key is not found
 */
void pairlist_search(struct PairList *const this, string key,
                     StringsPair *pair);

/**
 * @brief Insert a pair at the back of the list.
 * @param this The list this function is attached to
 * @param pair The pair to insert
 */
void pairlist_push_back(struct PairList *const this, StringsPair pair);

/**
 * @brief Remove a pair from the list
 * @param this The list this function is attached to
 * @param key The key of the pair that should be removed
 */
void pairlist_remove(struct PairList *const this, string key);

/**
 * @brief Clear all values from the list
 * @param this The list this function is attached to
 */
void pairlist_clear(struct PairList *const this);

/**
 * @brief Print the data inside the list
 * @param this The list this function is attached to
 */
void pairlist_print(struct PairList *const this);

#endif    // LIST_H