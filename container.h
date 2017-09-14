#ifndef CONTAINER_H_
#define HTABLE_H_

typedef enum container_e {FLEX_ARRAY, RED_BLACK_TREE} container_t;
typedef struct containerrec *container;

extern container container_new(container_t type);
extern void container_add(container c, char *word);
extern int container_search(container c, char *word);
extern void container_print(container c, void f(char *word));
extern void container_free(container c);

#endif
