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

/**
 * @brief Concatenate two strings into the "dest" string
 * @param dest The result of the concatentation
 * @param first The first string
 * @param second The second string
 * @return int32_t The return code
 */
int32_t concatentate(string *dest, string first, string second) {
    size_t len1, len2;
    string s1 = strcpy(calloc(strlen(first) + 1, 1), first);
    string s2 = strcpy(calloc(strlen(second) + 1, 1), second);

    if (s1 == NULL || s2 == NULL) {
        CERR(TRUE, "Couldn't allocate memory for the strings");
        free(s1);
        free(s2);
        return MALLOC_ERR;
    }

    len1 = strlen(s1);
    len2 = strlen(s2);

    free(*dest);
    *dest = strcpy(calloc(len1 + len2 + 1, sizeof(char)), s1);

    if (*dest == NULL) {
        CERR(TRUE, "Couldn't allocate memory for the string");
        free(s1);
        free(s2);
        return MALLOC_ERR;
    }

    if (strcat(*dest, s2) == NULL) {
        CERR(TRUE, "Couldn't concatenate the strings");
        free(s1);
        free(s2);
        return 1;
    }

    free(s1);
    free(s2);
    return 0;
}

/**
 * @brief Open a file and return it's file descriptor
 * @param path The path to the input
 * @param fd The returned file descriptor
 * @param proc The preprocessor "object"
 * @return int32_t The return code
 */
int32_t open_input(string path, FILE **fd, struct CPreprocessor *const proc) {
    /* Find the file. */
    u_int32_t i;
    int32_t ret_val;

    if (strcmp(path, proc->input) == 0) {
        /* The input file must not be searched in the include directories */
        *fd = fopen(path, "r");

        if (*fd == NULL) {
            CERR(TRUE, "Couldn't open file");
            return 1;
        }
    } else {
        /* The included files need to be (eventually) searched */
        FILE *check;

        if ((check = fopen(path, "r"))) {
            fclose(check);
            *fd = fopen(path, "r");
        } else {
            /* Try the include folders */
            for (i = 0; i < proc->_c_includes; ++i) {
                string i_path = calloc(1, sizeof(char));
                if (i_path == NULL) {
                    CERR(TRUE, "Couldn't allocate memory");
                    return MALLOC_ERR;
                }

                /* Compute the "include" path */
                ret_val = concatentate(&i_path, proc->includes[i], "/");
                if (ret_val != 0) {
                    free(i_path);
                    return ret_val;
                }

                ret_val = concatentate(&i_path, i_path, path);
                if (ret_val != 0) {
                    free(i_path);
                    return ret_val;
                }

                /* Check that the "include" path exists */
                if ((check = fopen(i_path, "r"))) {
                    fclose(check);
                    *fd = fopen(i_path, "r");
                    free(i_path);
                    return 0;
                }

                free(i_path);
            }
        }
    }

    DEBUG_MSG("Couldn't find file");
    return 1;
}

/**
 * @brief Close a file
 * @param fd The file descriptor of the file
 * @return int32_t The return code
 */
int32_t close_file(FILE *fd) {
    int32_t ret_code = fclose(fd);

    if (ret_code != 0) {
        CERR(TRUE, "Couldn't close the file");
        return ret_code;
    }

    return 0;
}

/**
 * @brief Preprocess data from the input file descriptor and
 * write the processed data into the output file descriptor
 * @param i_fd The input file descriptor
 * @param o_fd The output file descriptor
 * @param proc The preprocessor "object"
 * @return int32_t the return code
 */
int32_t process_input(FILE *i_fd, FILE *o_fd,
                      struct CPreprocessor *const proc) {
    /* Process the file */
    return 0;
}

/**
 * @brief Parse the command line arguments
 * @param proc The preprocessor
 * @param argc The arguments count
 * @param argv The arguments vector
 * @return int32_t The return code
 */
int32_t parse_arguments(struct CPreprocessor *const proc, u_int32_t argc,
                        string argv[]) {
    int32_t ret_code = 0;
    u_int32_t i;
    u_int8_t i_specified = 0;

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'D': {
                    /* Add the defines */
                    ret_code = add_define(
                        proc, strlen(argv[i]) == 2 ? argv[++i] : argv[i] + 2);
                } break;
                case 'I': {
                    /* Set include directories */
                    ret_code = add_include(
                        proc, strlen(argv[i]) == 2 ? argv[++i] : argv[i] + 2);
                } break;
                case 'o': {
                    /* Set the output file */
                    ret_code = set_output(
                        proc, strlen(argv[i]) == 2 ? argv[++i] : argv[i] + 2);
                } break;
            }
        } else {
            /* A argument with no '-' before it. The first one
             * will be the
             * input, the second will be the output */
            if (i_specified == FALSE) {
                ret_code = set_input(proc, argv[i]);
                if (ret_code == 0) { i_specified = TRUE; }
            } else {
                ret_code = set_output(proc, argv[i]);
            }
        }

        /* Check the return codes for the last operation */
        if (ret_code < 0) {
            /* In the case of an error, clear the memory and
             * stop the program */
            proc->clear(proc);
            return ret_code;
        }
    }

    return ret_code;
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
    int32_t ret_code = 0;

    /* Allocate all the memory */
    this->_c_includes = 0;
    this->input = calloc(1, sizeof(char));
    this->output = calloc(1, sizeof(char));
    this->map = new_map;
    this->includes = calloc(1, sizeof(string));
    this->_in_set = FALSE;
    this->_out_set = FALSE;

    /* Check allocated pointers */
    if (this->input == NULL || this->output == NULL || this->includes == NULL) {
        CERR(TRUE, "Couldn't allocate memory for the string");
        free(this->input);
        free(this->output);
        free(this->includes);
        return MALLOC_ERR;
    }

    /* Check map initialization */
    ret_code = this->map.init(&this->map);
    if (ret_code != 0) {
        free(this->input);
        free(this->output);
        free(this->includes);
        return ret_code;
    }

    /* Parse the arguments */
    ret_code = parse_arguments(this, argc, argv);

    return ret_code;
}

int32_t cpreprocessor_start(struct CPreprocessor *const this) {
    u_int32_t i;
    FILE *input;

    printf("Input: %s\nOutput: %s\n",
           this->_in_set == TRUE ? this->input : "STDIN",
           this->_out_set == TRUE ? this->output : "STDOUT");
    printf("Includes: ");
    for (i = 0; i < this->_c_includes; ++i) {
        printf("%s ", this->includes[i]);
    }
    printf("\nDefines:\n");
    this->map.print(&this->map);

    if (open_input("data.h", &input, this) == 0) {
        printf("%d\n", input->_fileno);
        close_file(input);
    }

    this->clear(this);
    return 0;
}