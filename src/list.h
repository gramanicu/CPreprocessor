/**
 * @file list.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for the list data structure
 * @copyright Copyright (c) 2021
 */

#ifndef LIST_H
#define LIST_H

#include "pair.h"

/* A "constructor" for the list */
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

    int32_t (*search)(struct PairList *const this, string key,
                      StringsPair *pair);
    int32_t (*insert)(struct PairList *const this, StringsPair pair);
    int32_t (*remove)(struct PairList *const this, string key);
    int32_t (*clear)(struct PairList *const this);
    int32_t (*print)(struct PairList *const this);
} PairList;

/**
 * @brief Search for a strings pair in the list.
 * @param this The list this function is attached to
 * @param key The key of the searched pair
 * @param pair The "returned" StringsPair with the required key. Returns a empty
 * pair if the key is not found
 * @return int32_t The return code (0 for no errors, 1 if the key is not
 * found)
 */
int32_t pairlist_search(struct PairList *const this, string key,
                        StringsPair *pair);

/**
 * @brief Insert a pair at the back of the list. If the key already exists,
 * update the value
 * @param this The list this function is attached to
 * @param pair The pair to insert
 * @return int32_t The return code (0 for no errors)
 */
int32_t pairlist_push_back(struct PairList *const this, StringsPair pair);

/**
 * @brief Remove a pair from the list
 * @param this The list this function is attached to
 * @param key The key of the pair that should be removed
 * @return int32_t The return code (0 for no errors, 1 if the key is not
 * found)
 */
int32_t pairlist_remove(struct PairList *const this, string key);

/**
 * @brief Clear all values from the list
 * @param this The list this function is attached to
 * @return int32_t The return code (0 for no errors)
 */
int32_t pairlist_clear(struct PairList *const this);

/**
 * @brief Print the data inside the list
 * @param this The list this function is attached to
 * @return int32_t The return code (0 for no errors)
 */
int32_t pairlist_print(struct PairList *const this);

#endif