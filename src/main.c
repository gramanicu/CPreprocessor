/**
 * @file main.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief
 * @copyright Copyright (c) 2021
 */

#include "cpreprocessor.h"

int process_ret_code(int ret_code) {
    if (ret_code == MALLOC_ERR) { return -MALLOC_ERR; }

    return 0;
}

int main(int argc, char *argv[]) {
    CPreprocessor p;
    int ret_code;

    /* Couldn't create a init macro, so this must be done manually */
    p.init = cpreprocessor_init;
    p.start = cpreprocessor_start;
    p.clear = cpreprocessor_clear;

    /* Initialise and preprocessor */
    ret_code = process_ret_code(p.init(&p, argc, argv));

    if (ret_code == 0) {
        /* Start the preprocessor */
        ret_code = process_ret_code(p.start(&p));
    }

    return ret_code;
}
