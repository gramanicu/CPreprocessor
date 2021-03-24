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
unsigned long hash_djb2(string str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        /* hash * 33 + c */
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
unsigned long hash_sdbm(string str) {
    unsigned long hash = 0;
    int c;

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
 * @return unsigned long The hash
 */
unsigned long hash_personal(string str) {
    unsigned long hash = 0;
    int c;

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
 * @return unsigned long The hash
 */
unsigned long hash(string src) { return hash_personal(src); }

/**
 * @brief Checks if the hashtable should have an increased size. As we insert
 more elements in the hashtable, the chance that collisions happen increases.
 So, to keep the search times minimal, the hashtable's array will be increased
 by a factor. Setting the `HASHMAP_FILL_MAX` to 50 and `HASHMAP_EXP_FACT` to 4
 will mean that "when the number of elements stored in the hashtable is over 50%
 of the number of buckets, the size of the array will quadruple". Also, when the
 array size is increased, the key-value pairs are redistributed.
 * @param this The hashmap this function is attached to
 * @return int The return code (0 for no errors)
 */
int check_resize(Hashmap *const this) {
    int ret_code;

    if ((float)this->_size / (float)this->_capacity >
        (float)HASHMAP_FILL_MAX / 100.0f) {
        /* The table needs to be increased */
        int i;

        int new_capacity = this->_capacity * HASHMAP_EXP_FACT;
        Bucket *new_buckets = calloc(new_capacity, sizeof(Bucket));

        /* Check if the malloc succeeded */
        if (new_buckets == NULL) {
            /* Mallocs failed */
            CERR(TRUE, "Couldn't resize hashmap");
            return MALLOC_ERR;
        }

        /* Initialise the new buckets */
        for (i = 0; i < new_capacity; ++i) {
            Bucket new_bucket = INIT_PAIRLIST;
            new_buckets[i] = new_bucket;
        }

        for (i = 0; i < this->_capacity; ++i) {
            /* Prepare to transfer the stored values to the new bucket */
            PairListElem *curr = this->buckets[i]._head;

            while (curr != NULL) {
                /* Compute the new hash */
                int new_id = hash(curr->data.first) % new_capacity;

                /* Insert into the new buckets */
                ret_code = new_buckets[new_id].insert(&new_buckets[new_id],
                                                      curr->data);

                if (ret_code < 0) { return ret_code; }

                curr = curr->next;
            }

            /* Free the old buckets */
            ret_code = this->buckets[i].clear(&this->buckets[i]);
            if (ret_code < 0) { return ret_code; }
        }
        /* Free the allocated memory */
        free(this->buckets);

        /* Assign the new buckets to the hashmap */
        this->buckets = new_buckets;
        this->_capacity = new_capacity;
    }

    return 0;
}

int hashmap_init(Hashmap *const this) {
    int i;
    this->_capacity = HASHMAP_SIZE_START;
    this->_size = 0;
    this->buckets = calloc(HASHMAP_SIZE_START, sizeof(Bucket));

    if (this->buckets == NULL) {
        CERR(TRUE, "Couldn't init hashmap");
        return MALLOC_ERR;
    }

    /* Initialise all the buckets */
    for (i = 0; i < this->_capacity; ++i) {
        Bucket new_bucket = INIT_PAIRLIST;
        this->buckets[i] = new_bucket;
    }

    this->_is_initialised = 1;
    return 0;
}

int hashmap_put(Hashmap *const this, StringsPair pair) {
    unsigned long id;
    int ret_code;

    /* Check if the hashmap is initialised */
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return 1;
    }

    /* Compute the hash */
    id = hash(pair.first) % this->_capacity;

    /* Insert the new pair */
    ret_code = this->buckets[id].insert(&this->buckets[id], pair);
    if (ret_code < 0) {
        DEBUG_MSG("Couldn't insert pair into the list");
        return ret_code;
    }

    this->_size++;

    /* Check if a resize is needed */
    ret_code = check_resize(this);
    if (ret_code < 0) {
        CERR(TRUE, "Error resizing hashmap after insert");
        return ret_code;
    }
    return 0;
}

int hashmap_remove(Hashmap *const this, string key) {
    unsigned long id;
    int ret_code;

    /* Check if the hashmap is initialised */
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return 1;
    }

    /* Compute the hash */
    id = hash(key) % this->_capacity;

    /* Remove the pair from the hashmap (using the key) */
    ret_code = this->buckets[id].remove(&this->buckets[id], key);

    if (ret_code < 0) {
        DEBUG_MSG("Couldn't remove pair from the hashmap");
        return ret_code;
    }

    if (ret_code == 0) { this->_size--; }
    return 0;
}

int hashmap_get(Hashmap *const this, string key, StringsPair *pair) {
    unsigned long id;
    int ret_code;

    /* Check if the hashmap is initialised */
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");

        ret_code = make_spair("", "", pair);
        if (ret_code < 0) {
            DEBUG_MSG("Couldn't retrieve an empty pair for the search");
            return ret_code;
        }
        return 1;
    }

    /* Compute the hash */
    id = hash(key) % this->_capacity;

    /* Search and return the pair from the hashmap */
    ret_code = this->buckets[id].search(&this->buckets[id], key, pair);
    if (ret_code < 0) {
        DEBUG_MSG("Error during key search");
        return ret_code;
    }
    return 0;
}

int hashmap_clear(Hashmap *const this) {
    int i;
    int ret_code;

    /* Check if the hashmap is initialised */
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return 1;
    }

    /* Clear all the buckets */
    for (i = 0; i < this->_capacity; ++i) {
        ret_code = this->buckets[i].clear(&this->buckets[i]);

        if (ret_code < 0) {
            DEBUG_MSG("Error during hashmap clear");
            return ret_code;
        }
    }

    /* Free the allocated memory */
    free(this->buckets);

    /* Set the hashmap as uninitialized */
    this->buckets = NULL;
    this->_capacity = 0;
    this->_size = 0;
    this->_is_initialised = 0;
    return 0;
}

int hashmap_print(Hashmap *const this) {
    int i;

    /* Check if the hashmap is initialised */
    if (!this->_is_initialised) {
        DEBUG_MSG("Hashmap was not initialised!");
        return 1;
    }

    printf("-- Hashmap --\n");
    for (i = 0; i < this->_capacity; ++i) {
        printf("%d - ", i);
        this->buckets[i].print(&this->buckets[i]);
    }
    printf("\n\n");
    return 0;
}
