#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "flexarray.h"

struct flexarrayrec {
    int capacity;
    int itemcount;
    char **items;
};

/**
 * Creates a new flexarray.
 *
 * @return a new empty flexarray.
 */
flexarray flexarray_new() {
    flexarray result = emalloc(sizeof *result);
    result->capacity = 2;
    result->itemcount = 0;
    result->items = emalloc(result->capacity * sizeof result->items[0]);
    return result;
}

/**
 * Adds a char array onto the end of a given flexarray.
 *
 * @param f is the flexarray that the char array will be added to.
 * @param item in the char array that you wish to add.
 */
void flexarray_append(flexarray f, char *item) {
    if (item == NULL) {
        return;
    }
  
    if (f->itemcount == f->capacity) {
        f->capacity *= 2;
        f->items = erealloc(f->items, f->capacity * sizeof f->items[0]);
    }

    f->items[f->itemcount] = emalloc((strlen(item) + 1)
                                     * sizeof f->items[0][0]);
    strcpy(f->items[f->itemcount], item);
    f->itemcount++;
}

/**
 * Prints all the items in a given flexarray in the order they are in the array.
 *
 * @param f is the flexarray that is to be printed.
 */
void flexarray_print(flexarray f) {
    int i;

    for (i = 0; i < f->itemcount; i++) {
        printf("%s\n", f->items[i]);
    }
}

/**
 * A sorting algorithm to sort the items in a flexarray.
 * Can also be used on any array of char arrays.
 * Uses insertion sort.
 *
 * @param a is the array that will be sorted.
 * @param n is the length of the array.
 */
static void insertion_sort(char **a, int n) {
    int i, j;
    char *key;

    /* Pull out item at position i and store in key */
    for (i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;

        /* Move each item that is greater than key and left of position i
         * one position to the right */
        while (j >= 0 && strcmp(a[j], key) > 0) {
            a[j+1] = a[j];
            j--;
        }

        /* Put key in leftmost vacated position */
        a[j+1] = key;
    }
}

/**
 * Swaps the position of two char arrays.
 * Used in sorting algorithms to swap positions.
 *
 * @param x the first char array that will be swapped.
 * @param y the second char array that will be swapped.
 */
static void swap(char **x, char **y) {
    void *temp = *x;
    *x = *y;
    *y = temp;
}

/**
 * A sorting algorithm to sort the items in a flexarray.
 * Can also be used on any array of char arrays.
 * Uses quick sort.
 *
 * @param a is the array that will be sorted.
 * @param n is the length of the array.
 */
static void quicksort(char **a, int n) {
    char *pivot;
    int i, j;
    
    if (n > 10) {
        pivot = a[rand() % n]; /* 'Random' pivot point */
        i = -1;
        j = n;
        for (;;) {
            do {
                i++;
            } while (strcmp(a[i], pivot) < 0);
            do {
                j--;
            } while (strcmp(a[j], pivot) > 0);
            if (i < j) {
                swap(a + i, a + j);
            } else {
                break;
            }
        }
        quicksort(a, j + 1); /* Left subarray */
        quicksort(a + j + 1, n - (j + 1)); /* Right subarray */
    }
}

/**
 * Calls a sorting function on a given flexarray.
 * The flexarray will be sorted.
 *
 * @param f is the flexarray that will be sorted.
 */
void flexarray_sort(flexarray f) {
    quicksort(f->items, f->itemcount);
    insertion_sort(f->items, f->itemcount);
}

/**
 * Unallocates the memory that a flexarray was using.
 * Deletes the flexarray.
 *
 * @param f is the flexarray that will be deleted and have its memory freed.
 */
void flexarray_free(flexarray f) {
    int i;
    for (i = 0; i < f->itemcount; i++) {
        free(f->items[i]);
    }
    free(f->items);
    free(f);
}

/**
 * Checks if a given item is in a given flexarray.
 *
 * @param f is the flexarray to be searched.
 * @param item is a char array that is being searche for.
 * @return 1 if the item is found, 0 otherwise.
 */
int is_present(flexarray f, char *item) {
    int i;

    for (i = 0; i < f->itemcount; i++) {
        if (strcmp(f->items[i], item) == 0) {
            return 1; /* true */
        }
    }

    return 0; /* false */
}

/**
 * Goes over each item in a flexarray and carries out a given function on the
 * item.
 *
 * @param f is the flexarray to be iterated over.
 * @param item is the name of the function that will be called.
 */
void visit(flexarray f, void func(char *item)) {
    int i;

    for (i = 0; i < f->itemcount; i++) {
        func(f->items[i]);
    }
}
