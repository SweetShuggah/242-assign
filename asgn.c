#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "container.h"
#include "htable.h"
#include "mylib.h"

/**
 * asgn.c
 *
 * Group 28
 * @author Nick Greene
 * @author Connor McIntyre
 *
 * This is the application program, which works as a spell checker. This
 * program takes command line options, and creates a hash table that will chain
 * with either flexarrays or red-black trees (dependent on user input). The
 * hash table will store a dictonary of words (a file specified on the command
 * line) and compare these to words from stdin. Words not found in the dictonary
 * will be sent to stdout.
 */

/**
 * Prints a string. Passed as a parameter to the function container_print.
 * @param str The string to be printed.
 */
void print(char *str) {
    printf("%s ", str);
}

/**
 * Main takes command line options, creates the hash table dictonary, compares
 * words from stdin, and prints the results to stdout.
 *
 * @param argc A count of the command line arguments.
 * @param argv The array that holds the command line arguments.
 */
int main(int argc, char **argv) {
    char *optstring = "rs:pih"; /* Defines the charaters accepted as options */
    char option; /* Stores the current command line option */
    char type = 'f'; /* The container type is FLEX_ARRAY by default */
    int size = -1; /* Size of the htable, less than 1 sets to default (3877) */
    void *h; /* The hash table to hold the dictonary of containers */
    char word[80]; /* Stores the current word being read */
    int i; /* An index variable for loops */
    FILE *dict; /* The dictonary of words to be stored in the hash table */
    int print_table = 0; /* Boolean that is 0 (false) by default */
    int print_time = 0; /* Boolean that is 0 (false) by default */
    clock_t fill_start, fill_end; /* Stores the time it takes to fill */
    clock_t search_start, search_end; /* Stores the time is takes to search */
    int unknown = 0; /* A count of the words not found in the hash table */

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                type = 'r'; /* RED_BLACK_TREE */
                break;
            case 's':
                size = atoi(optarg); /* Customly set hash table size */
                break;
            case 'p':
                print_table = 1; /* true */
                break;
            case 'i':
                print_time = 1; /* true */
                break;
            case 'h':
            default:
                fprintf(stderr, "HELP\n");
                fprintf(stderr, "-r       Use a robust chaining ");
                fprintf(stderr, "method. This tells your hash table\n    ");
                fprintf(stderr, "     to use a red-black tree as its ");
                fprintf(stderr, "container type.\n\n-s <hs>  Use hs as ");
                fprintf(stderr, "the size of your hash table.\n\n-p      ");
                fprintf(stderr, " Print your hash table to stdout one ");
                fprintf(stderr, "line per non-empty\n         container.");
                fprintf(stderr, " Nothing from stdin will be read and nothing");
                fprintf(stderr, "\n         else will be printed when this");
                fprintf(stderr, " option is selected.\n\n");
                fprintf(stderr, "-i       Print information about how");
                fprintf(stderr, " long it took to fill the hash\n        ");
                fprintf(stderr, " table, how long it took to search the ");
                fprintf(stderr, "hash table, and\n         how many ");
                fprintf(stderr, "unknown words were found.\n");
                return EXIT_SUCCESS;
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
            } /* end if */
        } /* end while */
        search_end = clock();

        if (print_time) {
            fprintf(stderr,
                    "Fill time : %f\nSearch time : %f\nUnknown words : %d\n",
                    (fill_end - fill_start) / (double) CLOCKS_PER_SEC,
                    (search_end - search_start) / (double) CLOCKS_PER_SEC,
                    unknown);
        } /* end if */
    } /* end else */

    htable_free(h);

    return EXIT_SUCCESS;
}
