/**
 * @file hashmap.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for the hashtable
 * @copyright Copyright (c) 2021
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include "list.h"

#define HASHMAP_SIZE_START 16 /* Initial size */
#define HASHMAP_FILL_MAX 50   /* Max fill percent */
#define HASHMAP_EXP_FACT 2    /* Expansion factor */

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
    int _is_initialised;
    int _size;
    int _capacity;

    int (*init)(struct Hashmap *const this);
    int (*put)(struct Hashmap *const this, StringsPair pair);
    int (*remove)(struct Hashmap *const this, string key);
    int (*get)(struct Hashmap *const this, string key, StringsPair *pair);
    int (*clear)(struct Hashmap *const this);
    int (*print)(struct Hashmap *const this);
} Hashmap;

/**
 * @brief Initialise the hashmap, if it isn't initialised (allocating space for
 * the buckets, etc..)
 * @param this The hashmap this function is attached to
 * @return int The return code (0 for no errors)
 */
int hashmap_init(Hashmap *const this);

/**
 * @brief Insert a strings pair into the hashmap
 * @param this The hashmap this function is attached to
 * @param pair The pair to add to the hashmap
 * @return int The return code (0 for no errors, 1 if not initialised)
 */
int hashmap_put(Hashmap *const this, StringsPair pair);

/**
 * @brief Remove a strings pair from the hashmap
 * @param this The hashmap this function is attached to
 * @param key The key of the searched pair
 * @return int The return code (0 for no errors, 1 if not initialised)
 */
int hashmap_remove(Hashmap *const this, string key);

/**
 * @brief Search for a strings pair in the hashmap.
 * @param this The hashmap this function is attached to
 * @param key The key of the pair to be removed
 * @param pair The StringsPair with the required key. Returns a empty
 * pair if the key is not found
 * @return int The return code (0 for no errors, 1 if not initialised)
 */
int hashmap_get(Hashmap *const this, string key, StringsPair *pair);

/**
 * @brief Clear all values from the hashmap (and sort of "un-initialise" it)
 * @param this The hashmap this function is attached to
 * @return int The return code (0 for no errors, 1 if not initialised)
 */
int hashmap_clear(Hashmap *const this);

/**
 * @brief Print the values inside the hashmap
 * @param this The hashmap this function is attached to
 * @return int The return code (0 for no errors, 1 if not initialised)
 */
int hashmap_print(Hashmap *const this);

#endif
