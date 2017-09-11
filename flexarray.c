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

flexarray flexarray_new() {
    flexarray result = emalloc(sizeof *result);
    result->capacity = 2;
    result->itemcount = 0;
    result->items = emalloc(result->capacity * sizeof result->items[0]); 
    return result;
}

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

void flexarray_print(flexarray f) {
    int i;

    for (i = 0; i < f->itemcount; i++) {
        printf("%s\n", f->items[i]);
    }
}

static void insertion_sort(char **a, int n) {
    int i, j;
    char *key;

    /* Pull out item at position i and store in key */
    for (i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;

        /* Move each item that is greater than key and left of position i
         * one position to the right */
        while (j >= 0 && a[j] > strcmp(a[j], key) > 0) {
            a[j+1] = a[j];
            j--;
        }

        /* Put key in leftmost vacated position */
        a[j+1] = key;
    }
}

static void swap(char *x, char *y) {
    char *temp = x;
    x = y;
    y = temp;
}

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

void flexarray_sort(flexarray f) {
    quicksort(f->items, f->itemcount);
    insertion_sort(f->items, f->itemcount);
}

void flexarray_free(flexarray f) {
    int i;
    for (i = 0; i < f->itemcount; i++) {
        free(f->items[i]);
    }
    free(f->items);
    free(f);
}

int is_present(flexarray f, char *item) {
    int i;

    for (i = 0; i < f->itemcount; i++) {
        if (strcmp(f->items[i], item) == 0) {
            return 1; /* true */
        }
    }

    return 0; /* false */
}

void visit(flexarray f, void func(char *item)) {
    int i;

    for (i = 0; i < f->itemcount; i++) {
        func(f->items[0]);
    }
}
