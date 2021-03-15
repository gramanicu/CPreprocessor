/**
 * @file hashmap.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for the hashtable
 * @copyright Copyright (c) 2021
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include <error_handling.h>
#include <list/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

#define HASHMAP_SIZE_START 128 /* Initial size */
#define HASHMAP_FILL_MAX 50    /* Max fill percent */
#define HASHMAP_EXP_FACT 2     /* Expansion factor */

/* A "constructor" for the hashmap */
#define INIT_HASHMAP                                                        \
    {                                                                       \
        0, 0, 0, 0, hashmap_init, hashmap_put, hashmap_remove, hashmap_get, \
            hashmap_clear, hashmap_print                                    \
    }

/**
 * @brief A bucket data structure. It stores lists of key-value pairs
 */
typedef PairList Bucket;

/**
 * @brief A hashmap data structure. It can store key/value pairs. The keys are
 * strings (char arrays), the values are integers.
 */
typedef struct Hashmap {
    Bucket *buckets;
    u_int8_t _is_initialised;
    u_int32_t _size;
    u_int32_t _capacity;

    int32_t (*init)(struct Hashmap *const this);
    int32_t (*put)(struct Hashmap *const this, StringsPair pair);
    int32_t (*remove)(struct Hashmap *const this, string key);
    int32_t (*get)(struct Hashmap *const this, string key, StringsPair *pair);
    int32_t (*clear)(struct Hashmap *const this);
    int32_t (*print)(struct Hashmap *const this);
} Hashmap;

/**
 * @brief Initialise the hashmap, if it isn't initialised (allocating space for
 * the buckets, etc..)
 * @param this The hashmap this function is attached to
 * @return int32_t The return code (0 for no errors)
 */
int32_t hashmap_init(struct Hashmap *const this);

/**
 * @brief Insert a strings pair into the hashmap
 * @param this The hashmap this function is attached to
 * @param pair The pair to add to the hashmap
 * @return int32_t The return code (0 for no errors, 1 if not initialised)
 */
int32_t hashmap_put(struct Hashmap *const this, StringsPair pair);

/**
 * @brief Remove a strings pair from the hashmap
 * @param this The hashmap this function is attached to
 * @param key The key of the searched pair
 * @return int32_t The return code (0 for no errors, 1 if not initialised)
 */
int32_t hashmap_remove(struct Hashmap *const this, string key);

/**
 * @brief Search for a strings pair in the hashmap.
 * @param this The hashmap this function is attached to
 * @param key The key of the pair to be removed
 * @param pair The StringsPair with the required key. Returns a empty
 * pair if the key is not found
 * @return int32_t The return code (0 for no errors, 1 if not initialised)
 */
int32_t hashmap_get(struct Hashmap *const this, string key, StringsPair *pair);

/**
 * @brief Clear all values from the hashmap (and sort of "un-initialise" it)
 * @param this The hashmap this function is attached to
 * @return int32_t The return code (0 for no errors, 1 if not initialised)
 */
int32_t hashmap_clear(struct Hashmap *const this);

/**
 * @brief Print the values inside the hashmap
 * @param this The hashmap this function is attached to
 * @return int32_t The return code (0 for no errors, 1 if not initialised)
 */
int32_t hashmap_print(struct Hashmap *const this);

#endif