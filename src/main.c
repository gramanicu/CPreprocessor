/**
 * @file main.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief
 * @copyright Copyright (c) 2021
 */

#include <data.h>
#include <error_handling.h>
#include <stdlib.h>

int main() {
    PairList l = INIT_PAIRLIST;
    StringsPair p;
    int32_t ret_code;

    ret_code = make_spair("1", "b", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    ret_code = clear_spair(&p);
    if (ret_code < 0) goto clear_memory;
    ret_code = make_spair("1", "a", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    ret_code = clear_spair(&p);
    if (ret_code < 0) goto clear_memory;
    ret_code = make_spair("2", "d", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    ret_code = clear_spair(&p);
    if (ret_code < 0) goto clear_memory;
    ret_code = make_spair("3", "e", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    ret_code = clear_spair(&p);
    if (ret_code < 0) goto clear_memory;
    ret_code = make_spair("2", "b", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    ret_code = clear_spair(&p);
    if (ret_code < 0) goto clear_memory;
    ret_code = make_spair("3", "c", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    ret_code = clear_spair(&p);
    if (ret_code < 0) goto clear_memory;
    ret_code = make_spair("4", "d", &p);
    if (ret_code < 0) goto clear_memory;
    ret_code = l.insert(&l, p);
    if (ret_code < 0) goto clear_memory;

    pairlist_print(&l);

clear_memory:
    if (ret_code == MALLOC_ERR) { return -MALLOC_ERR; }
    return ret_code;
    clear_spair(&p);
    l.clear(&l);
}