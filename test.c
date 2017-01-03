#include <stdio.h>
  #include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>

struct tableau {
    char *root;
    struct tableau *left;
    struct tableau *right;
    struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;

void print(struct tableau *t){
    printf("naeme = %s\n", t->root);
    printf("size of this struct is %i\n", sizeof(*t));
}

void print_word(char *w){
    printf("name = %s\n", w);
}

char* add_negation(char *string){
  char *negated_formula = malloc(100);
  negated_formula[0] = '-';
  int i = 1;
  while(string[i - 1] != '\0'){
    negated_formula[i] = string[i - 1];
    i++;
  }
  negated_formula[i] = '\0';
  return negated_formula;
}

int main(void){
    // print(&t);
    char *word = malloc(100);
    word = "this is to be copied";
    char *new_word = add_negation(word);
    printf("copied is: %s\n", new_word);
}
