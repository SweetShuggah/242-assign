#ifndef CONTAINER_H_
#define CONTAINER_H_

/**
 * container.h
 *
 * Group 28
 * @author Nick Greene
 * @author Connor McIntyre
 *
 * Defines the structure and functions of a container.
 */

typedef enum container_e {FLEX_ARRAY, RED_BLACK_TREE} container_t;
typedef struct containerrec *container;

/**
 * Creates a new container.
 *
 * @return a new empty container.
 */
extern container container_new(container_t type);

/**
 * Inserts a String into a container.
 *
 * @param c is the container that the String is being inserted into.
 * @param str is the string that will be inserted.
 */
extern void container_add(container c, char *str);

/**
 * Checks if a string is in a container.
 *
 * @param str is the string being searched for.
 * @return zero if the string is not in the container, non-zero if it is.
 */
extern int container_search(container c, char *str);

/**
 * Calls a give function on the contents of the container.
 *
 * @param c the container that is to be printed.
 * @param f the function that to be taken on each item in the container.
 */
extern void container_print(container c, void f(char *str));

/**
 * Deletes all content in the container and frees the memory it was using.
 *
 * @param c is the container that will be deleted.
 */
extern void container_free(container c);

#endif
