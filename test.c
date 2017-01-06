#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Fsize = 50;
int cases = 10;
int i;
int j;

/*typedef struct tableau tableau;*/
struct tableau {
  char *root;
  struct  tableau *left;
  struct tableau *right;
  struct tableau *parent;
} *tab, *node, *node1, *kid, *pa;


char* negate_it_bitch(char* cool) {
  char* new_string = malloc(Fsize);
  new_string[0] = '-';
  for (int i = 0; i < strlen(cool); i++) {
    new_string[i+1] = cool[i];
  }
  return new_string;
}


int* copy_this_bitch(int* tracker) {
  int *copy = malloc(6*sizeof(int));
  for (int i = 0; i < 6; i++) {
    copy[i] = tracker[i];
  }
  return copy;
}


void beta(struct tableau *t, char *left, char *right) {
  if (!(t->left) && !(t->right)) {
    struct tableau *new_kid1 = malloc(sizeof(*t));
    new_kid1->root = left;
    new_kid1->left = NULL;
    new_kid1->right = NULL;
    new_kid1->parent = t;

    struct tableau *new_kid2 = malloc(sizeof(*t));
    new_kid2->root = right;
    new_kid2->left = NULL;
    new_kid2->right = NULL;
    new_kid2->parent = t;

    // add the kid to the parent
    t->left = new_kid1;
    t->right = new_kid2;

  } else {
    if ((t->left)) {
      beta(t->left, left, right);
    }
    if (t->right) {
      beta(t->right, left, right);
    }
  }
}


int findbinary(char *c) {
  int bracket = 0;
  for (int i=0; i<strlen(c); i++) {
    if ((*(c+i)) == '(') {
      bracket += 1;
    }
    if ((*(c+i)) == ')') {
      bracket -= 1;
    }
    if ((*(c+i)) == 'v' || (*(c+i)) == '^' || (*(c+i)) == '>') {
      if (bracket == 1) {
      	return i;
      }
    }
  }
  return -1;
}


char *partone(char *g) {
  int x = findbinary(g);
  char *array1 = malloc(sizeof(char) * 50);
  for (int j=1; j<x; j++) {
    array1[j-1] = g[j];
  }
  return array1;
}


char *parttwo(char *g) {
  int x = findbinary(g);
  char *array2 = malloc(sizeof(char) * 50);
  for (int k=x+1; k<(strlen(g)-1); k++) {
    array2[k-x-1] = g[k];
  }
  return array2;
}


char bin(char *g) {
  return *(g+findbinary(g));
}


int parse(char *g) {
  //proposition
  if ((strlen(g) == 1) && ((g[0] == 'p') || (g[0] == 'q') || (g[0] == 'r'))) {
      return 1;
  }
  //negated formula
  if (*(g) == '-') {
    if (parse((g+1)) != 0) {
      return 2;
    }
  }
  //binary connective formula
  if (*(g) == '(') {
    if (parse(partone(g)) != 0 && parse(parttwo(g)) != 0) {
      return 3;
    }
  }
  return 0;
}


void alpha_one(struct tableau *t, char *poo) {
  if (!(t->left) && !(t->right)) {
    //  printf("I create this %s, %s\n", poo, t->root);
    struct tableau *new_kid = malloc(sizeof(*t));
    new_kid->root = poo;
    new_kid->left = NULL;
    new_kid->right = NULL;
    new_kid->parent = t;

    // add the kid to the parent
    t->left = new_kid;
  } else {
    if ((t->left)) {
            // printf("this was called %s, %s\n", poo, t->root);
      alpha_one(t->left, poo);
    }
    if (t->right) {
      alpha_one(t->right, poo);
    }
  }
}


void alpha_two(struct tableau *t, char *thing) {
  char* left = partone(thing);
  char* right = parttwo(thing);
  if (bin(thing) == 'v') {
    alpha_one(t, negate_it_bitch(left));
    alpha_one(t, negate_it_bitch(right));
  } else if (bin(thing) == '^') {
    beta(t, negate_it_bitch(left), negate_it_bitch(right));
  } else {
   	alpha_one(t, left);
    alpha_one(t, negate_it_bitch(right));
  }
}


void complete(struct tableau *t) {
  if((strlen(t->root) == 1) && (parse(t->root) == 1)) {
  } else if ((strlen(t->root) == 2) && (parse(t->root) == 2)) {
  } else if ((*(t->root) == '-') && (parse(t->root + 1) == 2)) {
  	alpha_one(t, t->root + 2);
  } else if ((parse(t->root) == 2) && (parse(t->root + 1) == 3)) {
    alpha_two(t, t->root + 1);
  } else if (parse(t->root) == 3) {
    if (bin(t->root) == '^') {
      alpha_one(t, partone(t->root));
      alpha_one(t, parttwo(t->root));
    } else if (bin(t->root) == '>') {
      beta(t, negate_it_bitch(partone(t->root)), parttwo(t->root));
    } else {
      beta(t, partone(t->root), parttwo(t->root));
    }
  }
  if (t->left) {
    complete(t->left);
  }
  if (t->right) {
    complete(t->right);
  }
}


int whoami(char* yes) {
  switch(*yes) {
    case('p'):
      return 0;
      break;
    case('q'):
      return 1;
      break;
    case('r'):
      return 2;
      break;
    default:
      return -1;
      break;
  }
}


void dfs(struct tableau *t, int* tracker, int* answer) {
  if (strlen(t->root) == 1) {
    tracker[whoami(t->root)*2] = 1;
  }
  if ((strlen(t->root) == 2)) {
	tracker[whoami(t->root+1)*2+1] = 1;
  }

  if (t->left) {
	  int* thing = copy_this_bitch(tracker);
    dfs(t->left, thing, answer);
  }
  if (t->right) {
	  int* thing2 = copy_this_bitch(tracker);
    dfs(t->right, thing2, answer);
  }
  if (!(t->right) && !(t->left)) {
    if (!((tracker[0] == 1 && tracker[1] == 1) || (tracker[2] == 1 && tracker[3] == 1) || (tracker[4] == 1 && tracker[5] == 1))) {
      answer[0] = 1;
    }
  }
}


int closed(struct tableau *t) {
  int *tracker = malloc(6*sizeof(int));
  for(int i=0; i<6; i++) {
     tracker[i] = 0;
  }
  int answer[] = {0};

  dfs(t, tracker, answer);

  return !answer[0];
}


int main(){ /*input a string and check if its a propositional formula */
  char *name = malloc(Fsize);
  FILE *fp, *fpout;

  /* reads from input.txt, writes to output.txt*/
  if ((fp=fopen("input.txt","r"))==NULL){
    printf("Error opening file");
    exit(1);
  }
  if ((fpout=fopen("output.txt","w"))==NULL){
    printf("Error opening file");
    exit(1);
  }

  int j;
  for(j=0;j<cases;j++){
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name)){
        case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
        case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
        case(2): fprintf(fpout, "%s is a negation.  ", name);break;
        case(3): fprintf(fpout, "%s is a binary.  ", name);break;
        default: fprintf(fpout, "What the f***!  ");
      }

      /*make new tableau with name at root, no children, no parent*/

      struct tableau t={name, NULL, NULL, NULL};

      /*expand the root, recursively complete the children*/
      if (parse(name)!=0){
        complete(&t);
    if (closed(&t)) printf("%s is not satisfiable.\n", name);
    else printf("%s is satisfiable.\n", name);
      } else printf("I told you, %s is not a formula.\n", name);
  }

  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}
