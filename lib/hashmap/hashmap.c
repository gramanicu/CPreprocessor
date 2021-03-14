/**
 * @file hashmap.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the hashmap
 * @copyright Copyright (c) 2021
 */

#include "hashmap.h"

/**
 * @brief A hashing function for a char array/string
 * Code taken from http://www.cse.yorku.ca/~oz/hash.html (djb2)
 * @param str The string to hash
 * @return unsigned long The hash
 */
u_int64_t hash_djb2(string str) {
    u_int64_t hash = 5381;
    u_int32_t c;

    while ((c = *str++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * @brief A hashing function for a char array/string
 * Code taken from http://www.cse.yorku.ca/~oz/hash.html (sdbm)
 * @param str The string to hash
 * @return unsigned long The hash
 */
u_int64_t hash_sdbm(string str) {
    u_int64_t hash = 0;
    u_int32_t c;

    while ((c = *str++)) { hash = c + (hash << 6) + (hash << 16) - hash; }

    return hash;
}

/**
 * @brief A hashing function for a char array, personal idea. It's a very bad
 * hash algorithm, but because (for some stupid reason) I can't use hash
 * functions from the internet, that ensure a good spread for the hash values
 * (which took some time to create & refine, as it's not exactly an exact
 * science) it's the best I could do.
 * @param str The string to hash
 * @return u_int64_t The hash
 */
u_int64_t hash_personal(string str) {
    u_int64_t hash = 0;
    u_int32_t c;

    uchar op = 0;

    while ((c = *str++)) {
        switch (op % 2) {
            case 0: {
                hash += c;
            } break;
            case 1: {
                hash *= c;
            } break;
        }

        op++;
    }

    return hash;
}

/**
 * @brief Hash a string. This is a wrapper function, that can use different
 * hashing functions. For the homework, it uses a simple (and bad) hashing
 * function I created.
 * @param src The string to hash
 * @return u_int64_t The hash
 */
u_int64_t hash(string src) { return hash_personal(src); }

/**
 * @brief Checks if the hashtable should have an increased size. As we insert
 more elements in the hashtable, the chance that collisions happen increases.
 So, to keep the search times minimal, the hashtable's array will be increased
 by a factor. Setting the `HASHMAP_FILL_MAX` to 50 and `HASHMAP_EXP_FACT` to 4
 will mean that "when the number of elements stored in the hashtable is over 50%
 of the number of buckets, the size of the array will quadruple". Also, when the
 array size is increased, the key-value pairs are redistributed.
 * @param this The hashmap this function is attached to
 */
void check_resize(struct Hashmap *const this) {
    if ((float)this->_size / (float)this->_capacity >
        (float)HASHMAP_FILL_MAX / 100.0f) {
        // The table needs to be increased

        u_int32_t new_capacity = this->_capacity * HASHMAP_EXP_FACT;
        Bucket *new_buckets = malloc(new_capacity * sizeof(Bucket));

        // Initialise the new buckets
        for (u_int32_t i = 0; i < new_capacity; ++i) {
            Bucket new_bucket = INIT_PAIRLIST;
            new_buckets[i] = new_bucket;
        }

        for (u_int32_t i = 0; i < this->_capacity; ++i) {
            // Prepare to transfer the stored values to the new bucket
            PairListElem *curr = this->buckets[i]._head;

            while (curr != 0) {
                // Compute the new hash
                u_int32_t new_id = hash(curr->data.first) % new_capacity;

                // Insert into the new buckets
                new_buckets[new_id].insert(&new_buckets[new_id], curr->data);

                curr = curr->next;
            }

            // Free the old buckets
            this->buckets[i].clear(&this->buckets[i]);
        }
        // Free the allocated memory
        free(this->buckets);

        // Assign the new buckets to the hashmap
        this->buckets = new_buckets;
        this->_capacity = new_capacity;
    }
}

void hashmap_init(struct Hashmap *const this) {
    this->_capacity = HASHMAP_SIZE_START;
    this->_size = 0;
    this->buckets = malloc(HASHMAP_SIZE_START * sizeof(Bucket));

    // Initialise all the buckets
    for (u_int32_t i = 0; i < this->_capacity; ++i) {
        Bucket new_bucket = INIT_PAIRLIST;
        this->buckets[i] = new_bucket;
    }

    this->_is_initialised = 1;
}

void hashmap_put(struct Hashmap *const this, StringsPair pair) {
    // Check if the hashmap is initialised
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return;
    }

    // Compute the hash
    u_int64_t id = hash(pair.first) % this->_capacity;

    // Insert the new pair
    this->buckets[id].insert(&this->buckets[id], pair);
    this->_size++;

    // Check if a resize is needed
    check_resize(this);
}

void hashmap_remove(struct Hashmap *const this, string key) {
    // Check if the hashmap is initialised
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return;
    }

    // Compute the hash
    u_int64_t id = hash(key) % this->_capacity;

    // Remove the pair from the hashmap (using the key)
    u_int32_t old_bsize = this->buckets[id]._size;
    this->buckets[id].remove(&this->buckets[id], key);

    if (old_bsize == this->buckets[id]._size + 1) { this->_size--; }
}

void hashmap_get(struct Hashmap *const this, string key, StringsPair *pair) {
    // Check if the hashmap is initialised
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        make_spair("", "", pair);
        return;
    }

    // Compute the hash
    u_int64_t id = hash(key) % this->_capacity;

    // Search and return the pair from the hashmap
    this->buckets[id].search(&this->buckets[id], key, pair);
}

void hashmap_clear(struct Hashmap *const this) {
    // Check if the hashmap is initialised
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return;
    }

    // Clear all the buckets
    for (u_int32_t i = 0; i < this->_capacity; ++i) {
        this->buckets[i].clear(&this->buckets[i]);
    }

    // Free the allocated memory
    free(this->buckets);

    // Set the hashmap as uninitialized
    this->buckets = 0;
    this->_capacity = 0;
    this->_size = 0;
    this->_is_initialised = 0;
}

void hashmap_print(struct Hashmap *const this) {
    printf("-- Hashmap --\n");
    for (u_int32_t i = 0; i < this->_capacity; ++i) {
        printf("%d - ", i);
        this->buckets[i].print(&this->buckets[i]);
    }
    printf("\n\n");
}