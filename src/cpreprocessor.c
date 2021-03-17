/**
 * @file cpreprocessor.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The implementation of the preprocessor
 * @copyright Copyright (c) 2021
 */

#include "cpreprocessor.h"

/**
 * @brief Set the input file
 * @param this The processor
 * @param input The input file
 * @return int32_t The return code
 */
int32_t set_input(struct CPreprocessor *const this, string input) {
    free(this->input);
    this->input = strcpy(calloc(1, strlen(input) + 1), input);
    if (this->input == NULL) {
        /* Mallocs failed */
        CERR(TRUE, "Couldn't set the input file");
        return MALLOC_ERR;
    }

    this->_in_set = TRUE;
    return 0;
}

/**
 * @brief Set the output file
 * @param this The processor
 * @param output The output file
 * @return int32_t The return code
 */
int32_t set_output(struct CPreprocessor *const this, string output) {
    free(this->output);
    this->output = strcpy(calloc(1, strlen(output) + 1), output);
    if (this->output == NULL) {
        /* Mallocs failed */
        CERR(TRUE, "Couldn't set the output file");
        return MALLOC_ERR;
    }

    this->_out_set = TRUE;
    return 0;
}

/**
 * @brief Add a include path
 * @param this The preprocessor
 * @param new_include The new included path
 * @return int32_t The return code
 */
int32_t add_include(struct CPreprocessor *const this, string new_include) {
    u_int32_t id = this->_c_includes;
    u_int32_t i;
    string *aux_buff;

    this->_c_includes++;

    /* Add space for the new path */
    aux_buff = realloc(this->includes, this->_c_includes * sizeof(string));
    if (aux_buff == NULL) {
        /* Realloc failed. Free all the paths here */
        for (i = 0; i < this->_c_includes; ++i) { free(this->includes[i]); }
        free(this->includes);
        this->_c_includes = 0;
        this->includes = aux_buff;
        CERR(TRUE, "Couldn't add a space for a new directory");
        return MALLOC_ERR;
    }
    this->includes = aux_buff;

    /* Add the include directory */
    this->includes[id] =
        strcpy(calloc(1, strlen(new_include) + 1), new_include);

    if (this->includes[id] == NULL) {
        /* Mallocs failed */
        this->_c_includes--;
        CERR(TRUE, "Couldn't set one of the include directories");
        return MALLOC_ERR;
    }

    return 0;
}

/**
 * @brief Add a define to the list, in the key=value format
 * @param this The cpreprocessor
 * @param key_value The key_value (a string of the key=value format)
 * @return int32_t The return code
 */
int32_t add_define(struct CPreprocessor *const this, string key_value) {
    string d_key;
    string d_value;
    StringsPair p;
    int32_t ret_code;

    d_key = strtok(key_value, "=");
    d_value = strtok(NULL, "=");

    /* In case the definition had no value */
    if (d_value == NULL) { d_value = ""; }

    /* Create the pair */
    ret_code = make_spair(d_key, d_value, &p);
    if (ret_code < 0) { return ret_code; }

    /* Add the pair in the map, then clear the memory used
     * "locally" */
    this->map.put(&this->map, p);
    ret_code = clear_spair(&p);
    if (ret_code < 0) { return ret_code; }

    return 0;
}

int32_t cpreprocessor_clear(struct CPreprocessor *const this) {
    u_int32_t i;
    int32_t ret_code = 0;
    ret_code = this->map.clear(&this->map);
    free(this->input);
    free(this->output);

    for (i = 0; i < this->_c_includes; ++i) { free(this->includes[i]); }
    free(this->includes);

    return ret_code;
}

int32_t cpreprocessor_init(struct CPreprocessor *const this, u_int32_t argc,
                           string argv[]) {
    Hashmap new_map = INIT_HASHMAP;
    u_int32_t i;
    int32_t ret_code = 0;
    u_int8_t i_specified = 0;

    /* Allocate all the memory */
    this->_c_includes = 0;
    this->input = calloc(1, sizeof(char));
    this->output = calloc(1, sizeof(char));
    this->map = new_map;
    this->map.init(&this->map);
    this->includes = calloc(1, sizeof(string));
    this->_in_set = FALSE;
    this->_out_set = FALSE;

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'D': {
                    /* Add the defines */
                    ret_code = add_define(
                        this, strlen(argv[i]) == 2 ? argv[++i] : argv[i] + 2);
                } break;
                case 'I': {
                    /* Set include directories */
                    ret_code = add_include(
                        this, strlen(argv[i]) == 2 ? argv[++i] : argv[i] + 2);
                } break;
                case 'o': {
                    /* Set the output file */
                    ret_code = set_output(
                        this, strlen(argv[i]) == 2 ? argv[++i] : argv[i] + 2);
                } break;
            }
        } else {
            /* A argument with no '-' before it. The first one will be the
             * input, the second will be the output */
            if (i_specified == FALSE) {
                ret_code = set_input(this, argv[i]);
                if (ret_code == 0) { i_specified = TRUE; }
            } else {
                ret_code = set_output(this, argv[i]);
            }
        }

        /* Check the return codes for the last operation */
        if (ret_code < 0) {
            /* In the case of an error, clear the memory and stop the program */
            this->clear(this);
            return ret_code;
        }
    }

    return 0;
}

int32_t cpreprocessor_start(struct CPreprocessor *const this) {
    u_int32_t i;

    printf("Input: %s\nOutput: %s\n",
           this->_in_set == TRUE ? this->input : "STDIN",
           this->_out_set == TRUE ? this->output : "STDOUT");
    printf("Includes: ");
    for (i = 0; i < this->_c_includes; ++i) {
        printf("%s ", this->includes[i]);
    }
    printf("\nDefines:\n");
    this->map.print(&this->map);

    this->clear(this);
    return 0;
}