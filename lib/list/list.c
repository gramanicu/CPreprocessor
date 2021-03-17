/**
 * @file list.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the list data structure
 * For the PairList data structure, we have to implement 4 functions
 * (sort of methods), which are attached to the struct when it is
 * "instantiated"
 * @copyright Copyright (c) 2021
 */

#include "list.h"

int32_t pairlist_search(struct PairList *const this, string key,
                        StringsPair *pair) {
    PairListElem *curr = this->_head;
    int32_t ret_code;

    if (this->_size == 0) {
        /* Return empty pair if the key is not found (empty list) */
        ret_code = make_spair("", "", pair);
        if (ret_code < 0) {
            DEBUG_MSG("Error while searching for a pair in the list");
            return ret_code;
        }
        return 1;
    }

    while (curr != NULL) {
        if (strcmp(curr->data.first, key) == 0) {
            return copy_spair(curr->data, pair);
        }
        curr = curr->next;
    }

    /* Return empty pair if the key is not found */
    ret_code = make_spair("", "", pair);
    if (ret_code < -1) {
        DEBUG_MSG("Error while searching for a pair in the list");
        return ret_code;
    }
    return 1;
}

int32_t pairlist_push_back(struct PairList *const this, StringsPair pair) {
    PairListElem *new_node = calloc(1, sizeof(PairListElem));
    PairListElem *curr = this->_head;
    int32_t ret_code;

    if (new_node == NULL) {
        DEBUG_MSG("Error while creating a new node to push");
        return MALLOC_ERR;
    }

    ret_code = copy_spair(pair, &new_node->data);
    if (ret_code < 0) {
        DEBUG_MSG("Error while pushing a pair to the list");
        return ret_code;
    }

    new_node->next = NULL;
    new_node->prev = NULL;

    /* If the list is empty, add the node directly */
    if (this->_size == 0) {
        this->_head = new_node;
        this->_size++;
        return 0;
    }

    /* Remove the node with the same key (to avoid duplicates/update value) */
    ret_code = this->remove(this, pair.first);

    /* Check again the list size */
    if (this->_size == 0) {
        this->_head = new_node;
        this->_size++;
        return 0;
    }

    while (curr->next != NULL) { curr = curr->next; }

    /* The next is empty, so insert the new value there */
    curr->next = new_node;
    new_node->prev = curr;
    this->_size++;
    return 0;
}

int32_t pairlist_remove(struct PairList *const this, string key) {
    PairListElem *curr = this->_head;
    int32_t ret_code;

    if (this->_size == 0) {
        /* Nothing to remove */
        return 1;
    }

    if (strcmp(this->_head->data.first, key) == 0) {
        /* The head must be removed */
        this->_head = curr->next;

        ret_code = clear_spair(&curr->data);
        if (ret_code < 0) {
            DEBUG_MSG("Error when removing element from the list");
            return ret_code;
        }

        free(curr);
        this->_size--;
        return 0;
    }

    while (curr != NULL) {
        if (strcmp(curr->data.first, key) == 0) {
            /* Remove the pair */
            PairListElem *next = curr->next;
            PairListElem *prev = curr->prev;

            if (next != NULL) { next->prev = curr->prev; }
            if (prev != NULL) { prev->next = curr->next; }

            ret_code = clear_spair(&curr->data);
            if (ret_code < 0) {
                DEBUG_MSG("Error when removing element from the list");
                return ret_code;
            };

            free(curr);
            this->_size--;
            return 0;
        }
        curr = curr->next;
    }

    /* Nothing removed */
    return 1;
}

int32_t pairlist_clear(struct PairList *const this) {
    PairListElem *curr = this->_head;
    int32_t ret_code;

    while (curr != NULL) {
        PairListElem *next = curr->next;

        ret_code = clear_spair(&curr->data);
        if (ret_code < 0) {
            DEBUG_MSG("Error when clearing the list");
            return ret_code;
        }

        free(curr);
        curr = next;
    }

    this->_size = 0;
    this->_head = NULL;

    return 0;
}

int32_t pairlist_print(struct PairList *const this) {
    PairListElem *curr = this->_head;

    if (this->_size != 0) {
        while (curr != NULL) {
            printf("{ %s - %s } ", curr->data.first, curr->data.second);
            curr = curr->next;
        }
    }
    printf("\n");

    return 0;
}
