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

StringsPair pairlist_search(struct PairList *const this, string key) {
    PairListElem *curr = this->_head;

    if (this->_size == 0) {
        // Return empty pair if the key is not found (empty list)
        return make_spair("", "");
    }

    while (curr != 0) {
        if (curr->data.first == key) { return curr->data; }
        curr = curr->next;
    }

    // Return empty pair if the key is not found
    return make_spair("", "");
}

void pairlist_push_back(struct PairList *const this, StringsPair pair) {
    PairListElem *newNode = malloc(sizeof(PairListElem));
    newNode->data = pair;
    newNode->next = 0;
    newNode->prev = 0;

    if (this->_size == 0) {
        this->_head = newNode;
        this->_size++;
        return;
    }

    PairListElem *curr = this->_head;
    while (curr->next != 0) { curr = curr->next; }

    // The next is empty, so insert the new value there
    curr->next = newNode;
    newNode->prev = curr;
    this->_size++;
}

void pairlist_remove(struct PairList *const this, string key) {
    PairListElem *curr = this->_head;

    if (this->_size == 0) {
        // Nothing to remove
        return;
    }

    if (this->_head->data.first == key) {
        // The head must be removed
        this->_head = curr->next;
        free(curr);
        this->_size--;
        return;
    }

    while (curr != 0) {
        if (curr->data.first == key) {
            // Remove the pair
            PairListElem *next = curr->next;
            PairListElem *prev = curr->prev;

            if (next != 0) { next->prev = curr->prev; }
            if (prev != 0) { prev->next = curr->next; }

            free(curr);
            this->_size--;
            return;
        }
        curr = curr->next;
    }
}

void pairlist_clear(struct PairList *const this) {
    PairListElem *curr = this->_head;

    while (curr != 0) {
        PairListElem *next = curr->next;
        free(curr);
        curr = next;
    }

    this->_size = 0;
    this->_head = 0;
}