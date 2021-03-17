/**
 * @file cpreprocessor.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for cpreprocessor
 * @copyright Copyright (c) 2021
 */

#ifndef CPREPROCESSOR_H
#define CPREPROCESSOR_H

#include <data.h>
#include <error_handling.h>
#include <stdlib.h>

typedef struct CPreprocessor {
    Hashmap map;
    string input;
    string output;
    string *includes;
    u_int32_t _c_includes;
    u_int8_t _in_set;
    u_int8_t _out_set;

    int32_t (*init)(struct CPreprocessor *const this, u_int32_t argc,
                    string argv[]);

    int32_t (*start)(struct CPreprocessor *const this);
    int32_t (*clear)(struct CPreprocessor *const this);
} CPreprocessor;

/**
 * @brief Initialise the c preprocessor (parse arguments and initialize the data
 * structures)
 * @param this The "object" this functions is attached to
 * @param argc The arguments count
 * @param argv The arguments vector
 * @return int32_t The return code
 */
int32_t cpreprocessor_init(struct CPreprocessor *const this, u_int32_t argc,
                           string argv[]);

/**
 * @brief Run the main logic behind the c preprocessor
 * @param this The "object" this functions is attached to
 * @return int32_t The return code
 */
int32_t cpreprocessor_start(struct CPreprocessor *const this);

/**
 * @brief Free the memory used by the processor
 * @param this The "object" this functions is attached to
 * @return int32_t The return code
 */
int32_t cpreprocessor_clear(struct CPreprocessor *const this);

#endif