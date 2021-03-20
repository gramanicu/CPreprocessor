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

    d_key = strtok(key_value, "= ");
    d_value = strtok(NULL, "");

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
        return 0;
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
 * @brief Reads a line into the line buffer, from the input. (it takes into
 * account the line continuation character)
 * @param line The read line
 * @param input The input FILE
 * @return int32_t The return code (0 for EOF, 1 for Success, others for errors)
 */
int32_t read_line(string *line, FILE *input) {
    string buffer = calloc(BUFFER_SIZE, sizeof(char));
    string line_start;
    int32_t ret_code;
    u_int8_t had_multi = FALSE;

    free(*line);
    *line = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer == NULL) {
        CERR(TRUE, "Couldn't allocate memory");
        return MALLOC_ERR;
    }

    /* While we can read and the current line is not continued, keep reading */
    while ((fgets(buffer, BUFFER_SIZE, input) != NULL)) {
        size_t len = strlen(buffer);
        if (len > 1) {
            if (buffer[len - 2] == '\\') {
                had_multi = TRUE;

                /* Line must be continued */
                buffer[len - 2] = '\0';
                line_start = buffer;

                /* Remove tabs or spaces from the start of the line */
                while (line_start[0] == '\t' || line_start[0] == ' ') {
                    line_start++;
                }

                ret_code = concatentate(line, *line, line_start);

                if (ret_code != 0) {
                    free(buffer);
                    return ret_code;
                }

                /* Read next line */
                continue;
            }
        }

        line_start = buffer;
        /* Remove tabs or spaces from the start of the line */
        if (had_multi) {
            while (line_start[0] == '\t' || line_start[0] == ' ') {
                line_start++;
            }
        }

        /* The line is ended */
        ret_code = concatentate(line, *line, line_start);

        if (ret_code != 0) {
            free(buffer);
            return ret_code;
        }
        free(buffer);
        return 1;
    }

    free(buffer);
    return 0;
}

/**
 * @brief Helper functions used by the process_input function, to allocate the
 * memory for the different buffers/arrays
 * @param buffer The buffer used to store the original line
 * @param line The buffer for the line that will be tokenized
 * @param expansion The buffer for the expansion of a macro
 * @return int32_t The return code
 */
int32_t _allocate_process_data(string *buffer, string *line,
                               string *expansion) {
    *buffer = calloc(BUFFER_SIZE, sizeof(char));
    if (*buffer == NULL) {
        CERR(TRUE, "Couldn't allocate memory");
        return MALLOC_ERR;
    }

    *line = calloc(BUFFER_SIZE, sizeof(char));
    if (*line == NULL) {
        CERR(TRUE, "Couldn't allocate memory");
        free(*buffer);
        return MALLOC_ERR;
    }

    *expansion = calloc(BUFFER_SIZE, sizeof(char));
    if (*expansion == NULL) {
        CERR(TRUE, "Couldn't allocate memory");
        free(*buffer);
        free(*line);
        return MALLOC_ERR;
    }

    return 0;
}

/**
 * @brief Helper functions used by the process_input function, to free the
 * memory for the different buffers/arrays
 * @param buffer The buffer used to store the original line
 * @param line The buffer for the line that will be tokenized
 * @param expansion The buffer for the expansion of a macro
 * @return int32_t The return code
 */
int32_t _free_process_data(string *buffer, string *line, string *expansion) {
    free(*buffer);
    free(*line);
    free(*expansion);
    return 0;
}

/**
 * @brief Check if the key is defined
 * @param proc The processor that uses this function
 * @param key The key/word to search
 * @return int32_t The return code (0 - exists, 1 - doesn't exist, other -
 * error)
 */
int32_t is_defined(struct CPreprocessor *const proc, string key) {
    int32_t ret_code;
    StringsPair pair;
    ret_code = proc->map.get(&proc->map, key, &pair);
    if (ret_code < 0) { return ret_code; }

    if (strcmp(pair.first, key) == 0) {
        ret_code = 0;
    } else {
        ret_code = 1;
    }

    clear_spair(&pair);
    return ret_code;
}

/**
 * @brief Tokenize a string
 * @param source The string
 * @param delims The delimitators
 * @param delim The delimitator that was found
 * @return string The token
 */
string simple_tok(string *source, string delims, string *delim) {
    size_t s_l, d_l;
    string start = *source;
    u_int32_t i, j;

    if (*source == NULL) { return NULL; }
    s_l = strlen(*source);
    d_l = strlen(delims);

    for (i = 0; i < s_l; ++i) {
        for (j = 0; j < d_l; ++j) {
            if ((*source + i)[0] == delims[j]) {
                (*source + i)[0] = '\0';
                (*delim)[0] = delims[j];
                *source = *source + i + 1;
                return start;
            }
        }
    }

    *source = NULL;
    return start;
}

/**
 * @brief Try to expand the specified string. If it can't code 1 is returned.
 * @param proc The processor that uses this function
 * @param key The key/word to expand
 * @param expansion The result
 * @return int32_t The return code (1 for no expansion, 0 for success)
 */
int32_t _expand(struct CPreprocessor *const proc, string key,
                string *expansion) {
    int32_t ret_code;
    StringsPair pair;
    string value_start;
    string value;
    string token;
    string f_exp = calloc(BUFFER_SIZE, 1);
    string l_exp = calloc(BUFFER_SIZE, 1);
    string delim = calloc(2, 1);
    ret_code = proc->map.get(&proc->map, key, &pair);

    if (ret_code < 0) { return ret_code; }

    /* If this is key exists in the hashmap */
    if (strcmp(pair.first, key) == 0) {
        /* Try to expand it too */
        value = strcpy(calloc(strlen(pair.second) + 1, 1), pair.second);
        if (value == NULL) {
            CERR(TRUE, "Couldn't allocate memory");
            return MALLOC_ERR;
        }
        value_start = value;

        token = simple_tok(&value, DELIMS, &delim);
        while (token) {
            if (strcmp(token, "") != 0 && strcmp(token, " ") != 0) {
                memset(l_exp, 0, BUFFER_SIZE);
                ret_code = _expand(proc, token, &l_exp);

                if (ret_code < 0) {
                    clear_spair(&pair);
                    return ret_code;
                } else if (ret_code == 0) {
                    concatentate(&f_exp, f_exp, l_exp);
                    concatentate(&f_exp, f_exp, delim);
                } else {
                    concatentate(&f_exp, f_exp, token);
                }
            } else {
                if (strcmp(token, "") == 0) {
                    concatentate(&f_exp, f_exp, delim);
                }
            }

            token = simple_tok(&value, DELIMS, &delim);
        }

        strcpy(*expansion, f_exp);
        free(value_start);
    } else {
        free(delim);
        free(f_exp);
        free(l_exp);
        clear_spair(&pair);
        return 1;
    }

    free(delim);
    free(f_exp);
    free(l_exp);
    clear_spair(&pair);
    return 0;
}

/**
 * @brief Process #define & #undef macros
 * @param proc The processor that uses this function
 * @param token The macro name
 * @param rest_of_line The arguments (if they exist)
 * @return int32_t The return code
 */
int32_t _process_definitions(struct CPreprocessor *const proc, string token,
                             string rest_of_line) {
    int32_t ret_code = 0;
    if (strncmp(token, "#define", 7) == 0) {
        ret_code = add_define(proc, rest_of_line);
    } else if (strncmp(token, "#undef", 6) == 0) {
        ret_code = proc->map.remove(&proc->map, rest_of_line);
    }

    if (ret_code < 0) { return ret_code; }
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
    string buffer;       /* Original read line */
    string line;         /* The "tokenizeable" line */
    string rest_of_line; /* Pointer to the rest of the line (after a token) */
    string unprocessed_pointer; /* Pointer used to write unprocessed data */
    string token;               /* Pointer to the extracted tokens */
    string processed_pointer;   /* Pointer used to write unprocessed data */
    string before; /* Pointer for unprocessed data that needs to be written */
    string expansion; /* Pointer used to store the expansion of a token */
    int32_t ret_code, read_code, offset;
    int8_t ifs[BUFFER_SIZE];
    int32_t opened_ifs = -1;

    /* Init memory for buffers/arrays */
    ret_code = _allocate_process_data(&buffer, &line, &expansion);
    if (ret_code != 0) { return ret_code; }

    memset(ifs, FALSE, BUFFER_SIZE * sizeof(int8_t));

    /* Read lines 1 by 1 */
    read_code = read_line(&buffer, i_fd);
    while (read_code == 1) {
        strcpy(line, buffer);
        unprocessed_pointer = buffer;

        /* To make sure there are no spaces before a #[directive], we must
         * extract a token. However, we can't tokenize the line, as it will
         * alter it. For this reason, I used the expansion array, which will be
         * anyway cleaned before use */
        strcpy(expansion, buffer);

        token = strtok(expansion, " ");
        /* Check if the line starts with a preprocessor directive. This
         * assumes that there are no characters before a directive. */
        if (token[0] == '#') {
            /* Lines with directives on them */
            token = strtok(line, " \n");
            rest_of_line = strtok(NULL, "\n");

            if (token[1] == 'e') {
                /* else, elif, endif. These terminate blocks. */
                printf("E DIR\n");
            } else if (opened_ifs == -1 || ifs[opened_ifs] == TRUE) {
                /* Everything here can be inside a block, so we must check the
                 * "if state" */

                if (strcmp(token, "#include") == 0) {
                    /* include is a special case of macro starting with 'i' */
                    printf("INCLUDE DIR\n");
                } else {
                    if (token[1] == 'd' || token[1] == 'u') {
                        /* define or undefine */
                        _process_definitions(proc, token, rest_of_line);
                    } else if (token[1] == 'i') {
                        /* if, ifndef, ifdef */
                        printf("I DIR\n");
                    }
                }
            }
        } else {
            /* Normal lines, without any directives */
            /* Ignored if the #if... macro was "false" */
            if (opened_ifs == -1 || ifs[opened_ifs] == TRUE) {
                /* Tokenize line to get words that could be macros
                 */
                token = strtok(line, DELIMS);
                while (token) {
                    /* Check if there are chars before the current
                     * token that weren't written to the output */
                    processed_pointer = strstr(unprocessed_pointer, token);
                    offset = processed_pointer - unprocessed_pointer;

                    /* If there is unprocessed that needs to be
                     * written*/
                    if (offset) {
                        before = strncpy(calloc(offset + 1, 1),
                                         unprocessed_pointer, offset);

                        if (before == NULL) {
                            CERR(TRUE, "Couldn't allocate memory");
                            _free_process_data(&buffer, &line, &expansion);
                            return MALLOC_ERR;
                        }

                        unprocessed_pointer = processed_pointer;
                        fwrite(before, sizeof(char), strlen(before), o_fd);
                        free(before);
                    }

                    /* Check if the token is a macro to be expanded
                     */
                    ret_code = _expand(proc, token, &expansion);

                    if (ret_code < 0) {
                        _free_process_data(&buffer, &line, &expansion);
                        return ret_code;
                    }

                    if (ret_code == 0) {
                        fwrite(expansion, sizeof(char), strlen(expansion),
                               o_fd);
                    } else {
                        /* Not a macro */
                        fwrite(token, sizeof(char), strlen(token), o_fd);
                    }

                    memset(expansion, 0, BUFFER_SIZE);
                    unprocessed_pointer += strlen(token);

                    token = strtok(NULL, DELIMS);
                }

                /* Check if it was EOF (in some cases, the file can
                 * end without newline, which can lead to some
                 * problems) */
                if (buffer[strlen(buffer) - 1] != '\n') {
                    /* As the newline character can be a token, no
                     * line (except the last one) can have
                     * unprocessed data after the tokenization. */
                    fwrite(unprocessed_pointer, sizeof(char),
                           strlen(unprocessed_pointer), o_fd);
                }
            }
        }

        /* Read next line */
        read_code = read_line(&buffer, i_fd);
    }

    _free_process_data(&buffer, &line, &expansion);

    if (read_code < 0) { return read_code; }
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
    int32_t ret_code;
    FILE *input, *output;

    if (this->_in_set == TRUE) {
        ret_code = open_input(this->input, &input, this);
        if (ret_code != 0) {
            this->clear(this);
            return ret_code;
        }
    } else {
        input = stdin;
    }

    if (this->_out_set == TRUE) {
        output = fopen(this->output, "w");
        if (output == NULL) {
            this->clear(this);
            close_file(input);
            CERR(TRUE, "Couldn't open file");
            return 1;
        }
    } else {
        output = stdout;
    }

    process_input(input, output, this);
    close_file(input);
    close_file(output);

    this->clear(this);
    return 0;
}