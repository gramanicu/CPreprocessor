/**
 * @file hashmap.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definition used for the hashtable
 * @copyright Copyright (c) 2021
 */

#ifndef HASHMAP_H
#define HASHMAP_H

/**
 * @brief A bucket data structure. It stores lists of key-value pairs
 */
typedef struct _Bucket {
    // List of key-value pairs

    // Several function pointers (see
    // https://stackoverflow.com/questions/17052443/c-function-inside-struct)

    // search
    // insert
    // remove
    // clear
} Bucket;

/**
 * @brief A hashmap data structure. It can store key/value pairs. The keys are
 * strings (char arrays), the values are integers.
 */
typedef struct _Hashmap {
    // Behind the scenes, the hashmap actually stores hash-bucket pairs. The
    // key-value pairs are stored inside the buckets.

    // The array of buckets
    Bucket *buckets;

    // Variables
    // If it was initialised
    // The size (number of buckets)
    // The number of pairs stored

    // Several function pointers (see
    // https://stackoverflow.com/questions/17052443/c-function-inside-struct)

    // init
    // put
    // remove
    // get
    // haskey
    // capacity
    // size
} Hashmap;

#endif    // HASHMAP_H