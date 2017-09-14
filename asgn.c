#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "container.h"
#include "htable.h"
#include "mylib.h"

static void print(char *str) {
    printf("%s\n", str);
}

int main(int argc, char **argv) {
    char *optstring = "rs:pih";
    char option;
    container_t type = FLEX_ARRAY;
    int size = -1;
    void *h;
    char word[256];
    int i;
    FILE *dict;

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                type = RED_BLACK_TREE;
                break;
            case 's':
                size = atoi(optarg);
                break;
            case 'p':
                h = htable_new(size);
                dict = fopen(argv[optind], "r");
                while (1 == fscanf(dict, "%s", word)) {
                    htable_insert(h, word);
                }
                fclose(dict);
                printf("here\n");
                htable_print(h);
                return EXIT_SUCCESS;
            case 'i':
                printf("i option\n");
                break;
            case 'h':
                fprintf(stderr, "HELP\n");
            default:
                fprintf(stderr, "-r       Use a robust chaining ");
                fprintf(stderr, "method. This tells your hash table\n    ");
                fprintf(stderr, "     to use a red-black tree as its ");
                fprintf(stderr, "container type.\n\n-s <hs>  Use hs as ");
                fprintf(stderr, "the size of your hash table.\n\n-p      ");
                fprintf(stderr, " Print your hash table to stdout one ");
                fprintf(stderr, "line per non-empty\n         container.");
                fprintf(stderr, "\n\n-i       Print information about how");
                fprintf(stderr, " long it took to fill the hash\n        ");
                fprintf(stderr, " table, how long it took to search the ");
                fprintf(stderr, "hash table, and\n         how many ");
                fprintf(stderr, "unknown words were found.\n\n");
        }
    }

    return EXIT_SUCCESS;
}
