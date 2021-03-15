/**
 * @file main.c
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief
 * @copyright Copyright (c) 2021
 */

#include <data.h>
#include <error_handling.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int32_t ret_code = 0, i;
    string input = malloc(sizeof(char));
    string output = malloc(sizeof(char));
    Hashmap map = INIT_HASHMAP;
    map.init(&map);

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'D': {
                    /* Define */
                    string d_key;
                    string d_value;
                    StringsPair p;

                    if (strlen(argv[i]) == 2) {
                        d_key = strtok(argv[i + 1], "=");
                        d_value = strtok(NULL, "=");
                        i++;
                    } else {
                        d_key = strtok(argv[i] + 2, "=");
                        d_value = strtok(NULL, "=");
                    }

                    /* In case the definition had no value */
                    if (d_value == NULL) { d_value = ""; }

                    /* Create the pair */
                    ret_code = make_spair(d_key, d_value, &p);
                    if (ret_code < 0) { goto clear_memory; }

                    /* Add the pair in the map, then clear the memory used
                     * "locally" */
                    map.put(&map, p);
                    ret_code = clear_spair(&p);
                    if (ret_code < 0) { goto clear_memory; }
                } break;
                case 'I': {
                    /* Included folders */
                    string i_dir;
                    if (strlen(argv[i]) == 2) {
                        i_dir = argv[i + 1];
                        i++;
                    } else {
                        i_dir = argv[i] + 2;
                    }

                    /* Add the directory to the included directories array */
                    /* TODO */
                    printf("Include path: %s\n", i_dir);
                } break;
                case 'o': {
                    /* Output file */
                    string o_file;
                    if (strlen(argv[i]) == 2) {
                        o_file = argv[i + 1];
                        i++;
                    } else {
                        o_file = argv[i] + 2;
                    }

                    /* Set the output file */
                    free(output);
                    output = strcpy(malloc(strlen(o_file) + 1), o_file);
                } break;
            }
        } else {
            /* Input file */
            free(input);
            input = strcpy(malloc(strlen(argv[i]) + 1), argv[i]);
        }
    }

    printf("Input %s\n", input);
    printf("Output %s\n", output);

    map.print(&map);
clear_memory:
    map.clear(&map);
    free(input);
    free(output);
main_return:
    if (ret_code == MALLOC_ERR) { return -MALLOC_ERR; }
    return ret_code;
}