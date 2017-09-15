#include<stdio.h>
#include<stdlib.h>
#include"mylib.h"
#include"flexarray.h"
#include"rbt.h"
#include"container.h"

/**
 * container.c
 *
 * Group 28
 * @author Nick Greene
 * @author Connor McIntyre
 * @author Connor Duncan
 *
 * Acts a a wrapper for either a red-black tree or a flexarray.
 */

/**
 * Defines the container.
 */
struct containerrec {
    container_t type; /* Defines what the container is acting as a wrapper for*/
    void *contents; /* Holds the structure that the container contains.*/
};

/**
 * Creates a new container.
 *
 * @return a new empty container.
 */
container container_new(container_t type) {
    container result = emalloc(sizeof *result);
    result->type = type;
    if (type == FLEX_ARRAY) {
        result->contents = flexarray_new();
    } else {
        result->contents = rbt_new();
    }
    return result;
}

/**
 * Inserts a String into a container.
 *
 * @param c is the container that the String is being inserted into.
 * @param str is the string that will be inserted.
 */
void container_add(container c, char *str) {
    if (c->type == FLEX_ARRAY) {
        flexarray_append(c->contents, str);
    } else {
        c->contents = rbt_insert(c->contents, str);
    }
}

/**
 * Checks if a string is in a container.
 *
 * @param str is the string being searched for.
 * @return zero if the string is not in the container, non-zero if it is.
 */
int container_search(container c, char *word) {
    if (c == NULL) {
        return 0;
    }
    return c->type == RED_BLACK_TREE ? rbt_search(c->contents, word) :
        is_present(c->contents, word);
}

/**
 * Calls a give function on the contents of the container.
 *
 * @param c the container that is to be printed.
 * @param f the function that to be taken on each item in the container.
 */
void container_print(container c, void f(char *str)) {
    if (c->type == RED_BLACK_TREE) {
        rbt_preorder(c->contents, f);
    } else {
        visit(c->contents, f);
    }
}

/**
 * Deletes all content in the container and frees the memory it was using.
 *
 * @param c is the container that will be deleted.
 */
void container_free(container c) {
    if (c->type == FLEX_ARRAY) {
        flexarray_free(c->contents);
    } else {
        c->contents = rbt_free(c->contents);
    }
    free(c);
}
    
