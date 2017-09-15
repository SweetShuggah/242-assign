#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"
#include "container.h"

/**
 * Group 28
 * @author Nick Greene
 * @author Connor McIntyre
 *
 * Holds all the method and structures for the Hash Table.
 */

#define DEFAULT_SIZE 3877

/**
 * A structure for handling the hash table.
 */
struct htablerec {
    unsigned int capacity; /* The max size of the hash table*/
    unsigned int num_keys; /* The number of keys currently in use*/
    void **keys; /* An array that contains all the values stored in the keys*/
    container_t type; /* The type of chaining method used (flexarray or rbt)*/
};

/**
 * Creates a new hash table.
 *
 * @param capacity is the size of the hash table.
 * @param type is the type of container that will be used for each cell
 *        in the hash table.
 * @return a new empty hash table.
 */
htable htable_new(int capacity, char type) {
    int i; /* for iterating throught the cells of the htable */

    htable result = emalloc(sizeof *result);
    result->capacity = capacity < 1 ? DEFAULT_SIZE : capacity;
    result->num_keys = 0;
    result->keys = emalloc(result->capacity * sizeof result->keys[0]);
    result->type = type == 'f' ? FLEX_ARRAY : RED_BLACK_TREE;

    for (i = 0; i < capacity; i++) {        
        result->keys[i] = NULL;
    }

    return result;
}

/**
 * Converts string to integer.
 *
 * @param word is the string that will be converted.
 * @return an unsigned int representing the word.
 */
static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    
    return result;
}

/**
 * Creates a hash for a given word that relates to a given hash table.
 *
 * @param h is the hash table the word is going to be assigned to.
 * @param str is the word that is going to be hashed.
 * @return an unsigned int of the words hash value in the given table.
 */
static unsigned int hash_function(htable h, char *str) {
    return htable_word_to_int(str) % h->capacity;
}

/**
 * Adds a string to the given hash table.
 *
 * @param h is the hash table that will be added to.
 * @param str is the string that will be added to the hash table.
 */
void htable_insert(htable h, char *str) {
    unsigned int key_index = hash_function(h, str); /* Generartes a hash for
                                                       the string*/
    
    if (h->keys[key_index] == NULL) {
        h->keys[key_index] = container_new(h->type);
        container_add(h->keys[key_index], str);
        h->num_keys++;
        return;
    }
    container_add(h->keys[key_index], str);
}

/**
 * Searches a hash table for a string.
 * Asks the container at the hash value if it contains the string.
 *
 * @param h is the hash table being searched.
 * @param str in the string being searched for.
 * @return 1 if the string is found, otherwise 0.
 */
int htable_search(htable h, char *str) {
    return container_search(h->keys[hash_function(h, str)], str);
}

/**
 * Calls a given function on everything stored in the hash table.
 *
 * @param h the hash table being operated on.
 * @param str the fuction to call on.
 */
void htable_print(htable h, void f(char *str)) {
    unsigned int i;

    for (i = 0; i < h->capacity; i++) {
        if (h->keys[i] != NULL) {
            printf("%d ", i);
            container_print(h->keys[i], f);
            printf("\n");
        }
    }
}

/**
 * Frees all the memory that was being used by a given hash table.
 *
 * @param h is the hash table being unallocated.
 */
void htable_free(htable h) {
    unsigned int i;
    
    for (i = 0; i < h->capacity; i++) {
        if (h->keys[i] != NULL) {
            container_free(h->keys[i]);
        }
    }

    free(h->keys);
    free(h);
}
