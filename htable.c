#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"
#include "container.h"

#define DEFAULT_SIZE 3877

struct htablerec {
    unsigned int capacity;
    unsigned int num_keys;
    void **keys;
    container_t type;
};

htable htable_new(int capacity, char type) {
    int i;

    htable result = emalloc(sizeof *result);
    result->capacity = capacity == -1 ? DEFAULT_SIZE : capacity;
    result->num_keys = 0;
    result->keys = emalloc(capacity * sizeof result->keys[0]);
    result->type = type == 'f' ? FLEX_ARRAY : RED_BLACK_TREE;

    for (i = 0; i < capacity; i++) {        
        result->keys[i] = NULL;
    }

    return result;
}

/**
 * Converts string to integer.
 */
static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    
    return result;
}

static unsigned int hash_function(htable h, char *str) {
    return htable_word_to_int(str) % h->capacity;
}

void htable_insert(htable h, char *str) {
    unsigned int key_index = hash_function(h, str);
    unsigned int i = key_index;
    
        if (h->keys[i] == NULL) {
            h->keys[i] = container_new(h->type);
            container_add(h->keys[i], str);
            h->num_keys++;
            return;
        }

        container_add(h->keys[i], str);
}


int htable_search(htable h, char *str) {
    return container_search(h->keys[hash_function(h, str)], str);
}

void htable_print(htable h, void f(char *str)) {
    int i;

    for (i = 0; i < h->capacity; i++) {
        if (h->keys[i] != NULL) {
            printf("%d ", i);
            container_print(h->keys[i], f);
            printf("\n");
        }
    }
}

void htable_free(htable h) {
    int i;
    
    for (i = 0; i < h->capacity; i++) {
        container_free(h->keys[i]);
    }

    free(h->keys);
    free(h);
}
