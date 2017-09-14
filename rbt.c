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
    rbt_colour colour; 
    rbt left;
    rbt right;
};

/**
 * Ensures that a new red-black tree is in the appropriate starting state.
 *
 * @return A new and empty red-black tree (NULL).
 */
rbt rbt_new() {
    return NULL;    
}

/**
 * Right rotates a red-black tree.
 *
 * @param r The red-black tree to be rotated.
 *
 * @return The rotated red-black tree.
 */
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

/**
 * Left rotates a red-black tree.
 *
 * @param r The red-black tree to be rotated.
 *
 * @return The rotated red-black tree.
 */
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
 * Fixes consecutive red violations in a red-black tree caused by insertion.
 *
 * @param r The red-black tree to be fixed.
 *
 * @return The fixed red-black tree.
 */
static rbt rbt_fix(rbt r) {

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

/**
 * Inserts an item into a red-black tree.
 *
 * @param r The red-black tree that the item will be inserted into.
 * @param str The item to be inserted.
 *
 * @return A reference to the red-black tree post-insertion.
 */
static rbt insert(rbt r, char *str) {
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
    } else {
        comparison = strcmp(str, r->key);
        if (comparison <= 0) {
            r->left = insert(r->left, str);
        }
        if (comparison > 0) {
            r->right = insert(r->right, str); 
        }
    }
    
    return rbt_fix(r);
}

/**
 * Calls an insert method and then sets the root to black.
 *
 * @param r The red-black tree that the item will be inserted into.
 * @param str The item to be inserted.
 *
 * @return A reference to the red-black tree post-insertion.
 */
rbt rbt_insert(rbt r, char *str) {
    int height = 0;
    r = insert(r, str);
    r->colour = BLACK;
    return r;
}

/**
 * Returns a reference to the node with the smallest value in a red-black tree.
 *
 * @param r The red-black tree to have the smallest node identified.
 *
 * @return A reference to the node with the smallest value.
 */
static rbt left_most(rbt r) {
    if (r->left == NULL) {
        return r;
    }
    
    return left_most(r->left);    
}

/**
 * Deletes a node with a specified value from a red-black tree.
 *
 * @param r The red-black tree to have a node deleted.
 * @param str The specified value of the node to be deleted.
 *
 * @return A reference to the red-black tree post-deletion.
 */
rbt rbt_delete(rbt r, char *str) {
    int comparison;
    rbt rbt_temp;
    rbt successor;
    char *key_temp;

    if (r == NULL || str == NULL) {
        return r;
    }

    comparison = strcmp(str, r->key);

    if (comparison < 0) {
        r->left = rbt_delete(r->left, str);
        return r;
    }

    if (comparison > 0) {
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
        rbt_temp = r;
        r = r->left;
        free(rbt_temp);
        return r;
    }

    /* If node has only right child */
    if (r->left == NULL) {
        free(r->key);
        rbt_temp = r;
        r = r->right;
        free(rbt_temp);        
        return r;
    }

    /* Node must have two children */
    successor = left_most(r->right);
    key_temp = r->key; 
    r->key = successor->key;
    successor->key = key_temp;
    r->right = rbt_delete(r->right, str);

    return r;
}

/**
 * Identifies whether a red-black tree contains a specified value.
 *
 * @param r The red-black tree the be searched.
 * @param str The value to be searched for.
 *
 * @return Whether the value is in the tree (non-zero) or not (zero).
 */
int rbt_search(rbt r, char *str) {
    int comparison;
    
    if (r == NULL || str == NULL) {
        return 0;
    }

    comparison = strcmp(str, r->key);

    if (comparison < 0) {
        return rbt_search(r->left, str);
    }

    if (comparison > 0) {
        return rbt_search(r->right, str);
    }

    /* key is equal to string */
    return 1;
}

/**
 * Frees all the memory that has been dynamically allocated to a red-black tree.
 *
 * @param r The red-black tree to be freed.
 *
 * @return An empty red-black tree (NULL).
 */
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

/**
 * Performs an inorder traversal on a red-black tree.
 * 
 * @param r The red-black tree to be traversed.
 * @param f A function that defines what happens to a node when it is visited.
 */
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

/**
 * Performs an preorder traversal on a red-black tree.
 * 
 * @param r The red-black tree to be traversed.
 * @param f A function that defines what happens to a node when it is visited.
 */
void rbt_preorder(rbt r, void f(char *str)) {
    if (r == NULL) {
        return;
    }
    
    f(r->key);

    if (r->left != NULL) {
        rbt_preorder(r->left, f);
    }

    if (r->right != NULL) {
        rbt_preorder(r->right, f);
    }
}
