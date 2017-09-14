#include <stdio.h>
#include <stdlib.h>
#include "container.h"
#include "flexarray.h"
#include "rbt.h"
#include "mylib.h"

struct containerrec {
    container_t type;
    void *contents;
};

container container_new(container_t type) {
    container result = emalloc(sizeof *result);
    result->type = type;
    return result;
}

void container_add(container c, char *word) {
    if (c->type == RED_BLACK_TREE) {
        c->contents = rbt_insert(c->contents, word);
    } else {
        flexarray_append(c->contents, word);
    }
}

int container_search(container c, char *word) {
    return c->type == RED_BLACK_TREE ? rbt_search(c->contents, word) :
        is_present(c->contents, word);
}

void container_print(container c, void f(char *word)) {
    if (c->type == RED_BLACK_TREE) {
        rbt_preorder(c->contents, f);
    } else {
        visit(c->contents, f);
    }
}

void container_free(container c) {
    if (c->type == RED_BLACK_TREE) {
        c->contents = rbt_free((rbt) c);
    } else {
        flexarray_free((flexarray) c->contents);
    }
    free(c);
}
