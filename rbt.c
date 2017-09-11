#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "rbt.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

typedef enum { RED, BLACK } rbt_colour;

struct rbtnode {
    char *key;
    unsigned int count; /* The number of times this key has been inserted */
    rbt_colour colour; 
    rbt left;
    rbt right;
};

rbt rbt_new() {
    return NULL;    
}

static rbt right_rotate(rbt r) {
    rbt temp;

    if (r == NULL || r->left == NULL) {
        return r;
    }

    temp = r;
    r = r->left;
    temp->left = r->right;
    r->right = temp;
    
    return r;
}

static rbt left_rotate(rbt r) {
    rbt temp;

    if (r == NULL || r->right == NULL) {
        return r;
    }

    temp = r;
    r = r->right;
    temp->right = r->left;
    r->left = temp;
    
    return r;
}

/**
 * Removes consecutive reds below root (R).
 */
static rbt_fix(rbt r) {

    /* If both children and a grandchild are red, colour root red and children
     * black. */
    if (IS_RED(r->left) && IS_RED(r->right) &&
        (IS_RED(r->left->left) || IS_RED(r->left->right) ||
         IS_RED(r->right->left) || IS_RED(r->right->right))) {
        r->colour = RED;
        r->left->colour = BLACK;
        r->right->colour = BLACK;
    }

    /* If the left child (A) is red, the right child (B) is black, and a left
     * grandchild is red. */ 
    if (IS_RED(r->left) && IS_BLACK(r->right) &&
        (IS_RED(r->left->left) || IS_RED(r->left->right))) {
        /* If the red grandchild is the right child of A, left rotate A. */
        if (IS_RED(r->left->right)) {
            r->left = left_rotate(r->left);
        }
        /* Regardless of which of the children of A are red, right rotate the
         * root (R), colour the new root black, and new child (R) red.  */
        r = right_rotate(r);
        r->colour = BLACK;
        r->right->colour = RED;
    }

    /* If the right child (B) is red, the left child (A) is black, and a right
     * grandchild is red. */ 
    if (IS_RED(r->right) && IS_BLACK(r->left) &&
        (IS_RED(r->right->left) || IS_RED(r->right->right))) {
        /* If the red grandchild is the left child of B, right rotate B. */
        if (IS_RED(r->right->left)) {
            r->right = right_rotate(r->right);
        }
        /* Regardless of which of the children of B are red, left rotate the
         * root (R), colour the new root black, and new child (R) red.  */
        r = left_rotate(r);
        r->colour = BLACK;
        r->left->colour = RED;
    }
    
    return r;
}

static rbt insert(rbt r, char *str, int is_root) {
    int comparison;

    /* No inserting NULL values */
    if (str == NULL) {
        return r;
    }

    if (r == NULL) {
        r = emalloc(sizeof *r);
        r->key = emalloc((strlen(str) + 1) * sizeof r->key[0]);
        strcpy(r->key, str);
        r->left = NULL;
        r->right = NULL;
        r->colour = RED;
        r->count = 0;
    }

    comparison = strcmp(r->key, str);

    if (comparison > 0) {
        r->left = insert(r->left, str, 0);   /* The node is not the root of */
    } else if (comparison < 0) {             /* the whole rbt, therefore */
        r->right = insert(r->right, str, 0); /* is_root is equal to 0 */
    } else /* comparison == 0 */ {
        r->count++;
    }

    r = rbt_fix(r);
    if (is_root) {
        r->colour = BLACK;
    }
    
    return r;
}

rbt rbt_insert(rbt r, char *str) {
    int is_root = 1; /* The current node is the root of the whole rbt */
    return insert(r, str, is_root);
}

static rbt left_most(rbt r) {
    if (r->left == NULL) {
        return r;
    }
    
    return left_most(r->left);    
}

rbt rbt_delete(rbt r, char *str) {
    int comparison;
    rbt store;
    rbt successor;
    char *temp;

    if (r == NULL || str == NULL) {
        return r;
    }

    comparison = strcmp(r->key, str);

    if (comparison > 0) {
        r->left = rbt_delete(r->left, str);
        return r;
    }

    if (comparison < 0) {
        r->right = rbt_delete(r->right, str);
        return r;
    }

    /* Therefore key is equal to str */

    /* If node is a leaf */
    if (r->left == NULL && r->right == NULL) {
        free(r->key);
        free(r);
        r = NULL;
        return r;
    }

    /* If node has only left child */
    if (r->right == NULL) {
        free(r->key);
        store = r;
        r = r->left;
        free(store);
        return r;
    }

    /* If node has only right child */
    if (r->left == NULL) {
        free(r->key);
        store = r;
        r = r->right;
        free(store);        
        return r;
    }

    /* Node must have two children */
    successor = left_most(r->right);
    temp = r->key; 
    r->key = successor->key;
    successor->key = temp;

    r->right = rbt_delete(r->right, str);

    return r;
}

int rbt_search(rbt r, char *str) {
    int comparison;
    
    if (r == NULL || str == NULL) {
        return 0;
    }

    comparison = strcmp(r->key, str);

    if (comparison > 0) {
        return rbt_search(r->left, str);
    }

    if (comparison < 0) {
        return rbt_search(r->right, str);
    }

    /* key is equal to string */
    return 1;
}

rbt rbt_free(rbt r) {
    if (r == NULL) {
        return r;
    }

    if (r->right != NULL) {
        r->right = rbt_free(r->right);
    }

    if (r->left != NULL) {
        r->left = rbt_free(r->left);
    }

    free(r->key);
    free(r);        
    return NULL;
}

void rbt_inorder(rbt r, void f(char *str)) {
    char *col;
    if (r == NULL) {
        return;
    }

    if (r->left != NULL) {
        rbt_inorder(r->left, f);
    }
    
    f(r->key);

    if (r->right != NULL) {
        rbt_inorder(r->right, f);
    }
}

void rbt_preorder(rbt r, void f(char *str)) {
    char *col;
    if (r == NULL) {
        return;
    }

    col = IS_RED(r) ? "red:" : "black:";
    printf("%-7s", col);
    
    f(r->key);

    if (r->left != NULL) {
        rbt_preorder(r->left, f);
    }

    if (r->right != NULL) {
        rbt_preorder(r->right, f);
    }
}
