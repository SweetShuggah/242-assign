#ifndef FLEXARRAY_H_
#define FLEXARRAY_H_

typedef struct flexarrayrec *flexarray;

extern void      flexarray_append(flexarray f, char *item);
extern void      flexarray_free(flexarray f);
extern flexarray flexarray_new();
extern void      flexarray_print(flexarray f);
extern void      flexarray_sort(flexarray f);
extern int       is_present(flexarray f, char *item);
extern void      visit(flexarray f, void func(char *item));

#endif
