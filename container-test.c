#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "container.h"
#include "mylib.h"

void print(char *str) {
    printf("%s\n", str);
}

int main(void) {
    char word[50];
    container c = container_new(RED_BLACK_TREE);

    while (1 == scanf("%s", word)) {
        container_add(c, word);
    }

    printf("added\n");
    container_print(c, print);
    container_free(c);
    printf("freed\n");

    return EXIT_SUCCESS;
}
