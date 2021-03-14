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

void pairlist_search(struct PairList *const this, string key,
                     StringsPair *pair) {
    PairListElem *curr = this->_head;

    if (this->_size == 0) {
        // Return empty pair if the key is not found (empty list)
        make_spair("", "", pair);
        return;
    }

    while (curr != 0) {
        if (strcmp(curr->data.first, key) == 0) {
            copy_spair(curr->data, pair);
            return;
        }
        curr = curr->next;
    }

    // Return empty pair if the key is not found
    make_spair("", "", pair);
}

void pairlist_push_back(struct PairList *const this, StringsPair pair) {
    PairListElem *new_node = malloc(sizeof(PairListElem));

    copy_spair(pair, &new_node->data);
    new_node->next = 0;
    new_node->prev = 0;

    if (this->_size == 0) {
        this->_head = new_node;
        this->_size++;
        return;
    }

    PairListElem *curr = this->_head;
    while (curr->next != 0) { curr = curr->next; }

    // The next is empty, so insert the new value there
    curr->next = new_node;
    new_node->prev = curr;
    this->_size++;
}

void pairlist_remove(struct PairList *const this, string key) {
    PairListElem *curr = this->_head;

    if (this->_size == 0) {
        // Nothing to remove
        return;
    }

    if (strcmp(this->_head->data.first, key) == 0) {
        // The head must be removed
        this->_head = curr->next;
        clear_spair(&curr->data);
        free(curr);
        this->_size--;
        return;
    }

    while (curr != 0) {
        if (strcmp(curr->data.first, key) == 0) {
            // Remove the pair
            PairListElem *next = curr->next;
            PairListElem *prev = curr->prev;

            if (next != 0) { next->prev = curr->prev; }
            if (prev != 0) { prev->next = curr->next; }

            clear_spair(&curr->data);
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
        clear_spair(&curr->data);
        free(curr);
        curr = next;
    }

    this->_size = 0;
    this->_head = 0;
}

void pairlist_print(struct PairList *const this) {
    PairListElem *curr = this->_head;

    while (curr != 0) {
        printf("{ %s - %s } ", curr->data.first, curr->data.second);
        curr = curr->next;
    }
    printf("\n");
}
