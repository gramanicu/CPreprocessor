/**
 * @file cpreprocessor.h
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The definitions used for cpreprocessor
 * @copyright Copyright (c) 2021
 */

#ifndef CPREPROCESSOR_H
#define CPREPROCESSOR_H

#include "hashmap.h"

#define DELIMS "\t []{}<>=+-*/%!&|^.,:;()\\"
#define BUFFER_SIZE 256

typedef struct CPreprocessor {
    Hashmap map;
    string input;
    string output;
    string *includes;
    int _c_includes;
    int _in_set;
    int _out_set;

    int (*init)(struct CPreprocessor *const this, int argc, string argv[]);

    int (*start)(struct CPreprocessor *const this);
    int (*clear)(struct CPreprocessor *const this);
} CPreprocessor;

/**
 * @brief Initialise the c preprocessor (parse arguments and initialize the data
 * structures)
 * @param this The "object" this functions is attached to
 * @param argc The arguments count
 * @param argv The arguments vector
 * @return int The return code
 */
int cpreprocessor_init(CPreprocessor *const this, int argc, string argv[]);

/**
 * @brief Run the main logic behind the c preprocessor
 * @param this The "object" this functions is attached to
 * @return int The return code
 */
int cpreprocessor_start(CPreprocessor *const this);

/**
 * @brief Free the memory used by the processor
 * @param this The "object" this functions is attached to
 * @return int The return code
 */
int cpreprocessor_clear(CPreprocessor *const this);

#endif
