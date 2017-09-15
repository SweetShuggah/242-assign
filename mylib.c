#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "mylib.h"

/**
 * Allocates the requested memory and returns a pointer to it.
 *
 * @param s is the size of the memory block, in bytes.
 * @return a pointer to the allocated memory.
 */
void *emalloc(size_t s) {
    void *result = malloc(s);
    
    if (NULL == result) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

/**
 * Attempts to resize the memory block pointed to by p that was previously
 * allocated.
 *
 * @param p This is the pointer to a memory block previously allocated
 * to be reallocated.
 * @param s This is the new size for the memory block, in bytes.
 * @return This function returns a pointer to the newly allocated memory, 
 *         or NULL if the request fails.
 */
void *erealloc(void *p, size_t s) {
    p = realloc(p, s);

    if (NULL == p) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

/**
 * Reads a word from an InputStream and returns it as a char array.
 *
 * @param s a char array that the word will be read into.
 * @param limit the maximum size of s.
 * @param stream the InputStream that is being read.
 * @return a char array of the word that was read in all lowercase.
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;

    assert(limit > 0 && s != NULL && stream != NULL);

    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c);
    
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }
    
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    
    return w - s;
}
