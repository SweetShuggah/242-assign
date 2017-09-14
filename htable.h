#ifndef HTABLE_H_
#define HTABLE_H_

typedef struct htablerec *htable;

extern void   htable_free(htable h);
extern void   htable_insert(htable h, char *str);
extern htable htable_new(int capacity, char type);
extern void   htable_print(htable h, void f(char *str));
extern int    htable_search(htable h, char *str);

#endif
