#ifndef RBT_H_
#define RBT_H_

/**
 * rbt.h
 *
 * Group 28
 * @author Nick Greene
 * @author Connor McIntyre
 *
 * Header file which defines the functions that must be implemented in rbt.c.
 */

/**
 * Defines a rbt node.
 */
typedef struct rbtnode *rbt;

/**
 * Deletes a node with a specified value from a red-black tree.
 *
 * @param r The red-black tree to have a node deleted.
 * @param str The specified value of the node to be deleted.
 *
 * @return A reference to the red-black tree post-deletion.
 */
extern rbt  rbt_delete(rbt r, char *str);

/**
 * Frees all the memory that has been dynamically allocated to a red-black tree.
 *
 * @param r The red-black tree to be freed.
 *
 * @return An empty red-black tree (NULL).
 */
extern rbt  rbt_free(rbt r);

/**
 * Performs an inorder traversal on a red-black tree.
 * 
 * @param r The red-black tree to be traversed.
 * @param f A function that defines what happens to a node when it is visited.
 */
extern void rbt_inorder(rbt r, void f(char *str));

/**
 * Inserts an item into a red-black tree.
 *
 * @param r The red-black tree that the item will be inserted into.
 * @param str The item to be inserted.
 *
 * @return A reference to the red-black tree post-insertion.
 */
extern rbt  rbt_insert(rbt r, char *str);

/**
 * Ensures that a new red-black tree is in the appropriate starting state.
 *
 * @return A new and empty red-black tree (NULL).
 */
extern rbt  rbt_new();

/**
 * Performs an preorder traversal on a red-black tree.
 * 
 * @param r The red-black tree to be traversed.
 * @param f A function that defines what happens to a node when it is visited.
 */
extern void rbt_preorder(rbt r, void f(char *str));

/**
 * Identifies whether a red-black tree contains a specified value.
 *
 * @param r The red-black tree the be searched.
 * @param str The value to be searched for.
 *
 * @return Whether the value is in the tree (non-zero) or not (zero).
 */
extern int  rbt_search(rbt r, char *str);

#endif
