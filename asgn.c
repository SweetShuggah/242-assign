#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "container.h"
#include "htable.h"
#include "mylib.h"

void print(char *str) {
    printf("%s ", str);
}

int main(int argc, char **argv) {
    char *optstring = "rs:pih";
    char option;
    char type = 'f'; /* FLEX_ARRAY */
    int size = -1;
    void *h;
    char word[80];
    int i;
    FILE *dict;
    int print_table = 0; /* Boolean that is 0 (false) by default*/
    int print_time = 0; /* Boolean that is 0 (false) by default*/
    clock_t fill_start, fill_end, search_start, search_end;
    int unknown = 0; /* A count of the words not found in the dictonary */

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                type = 'r'; /* RED_BLACK_TREE */
                break;
            case 's':
                size = atoi(optarg);
                break;
            case 'p':
                print_table = 1; /* true */
                break;
            case 'i':
                print_time = 1;
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

    h = htable_new(size, type);
    dict = fopen(argv[optind], "r");

    fill_start = clock();
    while (getword(word, sizeof word, dict) != EOF) {
        htable_insert(h, word);
    }
    fill_end = clock();
    
    fclose(dict);

    if (print_table) {
        htable_print(h, print);
    } else {
        search_start = clock();
        while (getword(word, sizeof word, stdin) != EOF) {
            if (!htable_search(h, word)) {
                printf("%s\n", word);
                unknown++;
            }
        }
        search_end = clock();
    }

    if (print_time) {
        fprintf(stderr, "\nFill time: %f\nSearch time: %f\nUnknown words: %d\n",
                (fill_end - fill_start) / (double) CLOCKS_PER_SEC,
                (search_end - search_start) / (double) CLOCKS_PER_SEC, unknown);
    }

    htable_free(h);

    return EXIT_SUCCESS;
}
