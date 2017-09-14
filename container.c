#include<stdio.h>
#include<stdlib.h>
#include"mylib.h"
#include"flexarray.h"
#include"rbt.h"
#include"container.h"

struct containerrec {
    container_t type;
    void *contents;
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
    return c->type == RED_BLACK_TREE ? rbt_search(c->contents, word) :
        is_present(c->contents, word);
}

/**
 * Prints the contents of the container.
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
 * Removes a string from a container.
 *
 * @param c the container that the string will be removed from.
 * @param str is the string that is to be removed.
 */
void container_remove(container c, char *str) {
    if (c->type == FLEX_ARRAY) {
    } else {
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
    
