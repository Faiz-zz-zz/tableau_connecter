#include <stdio.h>
#include <string.h>     /* for all the new-fangled string functions */
#include <stdlib.h>         /* malloc, free, rand */

int Fsize=50;
int cases=10;
int no_edges;
int no_nodes;
int i;
int j;    /*big enough for our formulas*/
char *slicer(char *g, int start);
int atomic_formula(char *g);
int filtering(char *g, int start);
int existential_formula(char *g);
int univeral_formula(char *g);
int parse(char *g);
int multilple_formulas(char *g);
char *parttwo(char *g);
char *partone(char *g);
char* add_negation(char *string);

/*typedef struct tableau tableau;*/

struct tableau {
    char *root;
    struct    tableau *left;
    struct tableau *right;
    struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;



/*put all your functions here.    You will need
1.
int parse(char *g) which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0
2.
void complete(struct tableau *t)
which expands the root of the tableau and recursively completes any child tableaus.
3.
int closed(struct tableau *t)
which checks if the whole tableau is closed.
Of course you will almost certainly need many other functions.

You may vary this program provided it reads 10 formulas in a file called "input.txt" and outputs in the way indicated below to a file called "output.txt".
*/

char *slicer(char *g, int start){
    char *part = malloc(Fsize);
    int i = 0;
    for(i = start; i < strlen(g); i++){
        part[j++] = g[i];
    }
    return part;
}


int filtering(char *g, int start){
    int i = 0, flag1 = 1, flag2 = 1, flag3 = 1;
    while(g[i] != '\0'){
        if (g[i] != 'p' && g[i] != 'q' && g[i] != 'r' &&
            g[i] != '(' && g[i] != ')' && g[i] != 'v' &&
            g[i] != '^' && g[i] != '>' && g[i] != '-'){
            flag1=0;
        }
    // if ((g[i] == 'p' || g[i] == 'q' || g[i] == 'r') && g[i+1] == ')') flag2 = 0;
    // if ((g[i] == 'E' || g[i] == 'X' || g[i] == 'A') && g[i+1] == ')') flag3 = 0;
        i++;
    }
        // printf("%i    %i    %i\n", flag1, flag2, flag3);
    return flag1 && flag2 && flag3;
}

int parse(char *g){
    /* returns 0 for non-formulas, 1 for atoms, 2 for negations, 3 for binary connective fmlas, 4 for existential and 5 for universal formulas.*/
    // printf("This is the filtering %i => %s\n", filtering(g, 0), g);
    if (filtering(g, 0) > 0){
        if ((g[0] == 'p' || g[0] == 'q' || g[0] == 'r') && strlen(g) == 1){
            // printf("This was called");
            return 1;
        } else if (g[0] == '('){
            // binary
            // printf("Partone = %s, %i, partttwo = %s, %i\n", partone(g), parse(partone(g)), parttwo(g), parse(parttwo(g)));
            if ((parse(partone(g)) && parse(parttwo(g))) > 0){
                return 3;
            }
            else return 0;
        } else if (g[0] == '-'){
            // negation
            if (parse(slicer(g, 1)) > 0) return 2;
            else return 0;
        }
    }
    return 0;
}


int multilple_formulas(char *g){
    int counter = 0;
    int i;
    for (i = 0; i < strlen(g); i++){
        if (g[i] == '(') counter++;
            else if(g[i] == ')') counter--;

            if ((g[i]== '>' || g[i] == '^' || g[i] == 'v') && counter == 1) return i;
    }
    return 0;
}

char *parttwo(char *g){
        char *part = malloc(Fsize);
        int j = 0;
        int i = 0;
        for(i = multilple_formulas(g) + 1; i < strlen(g) - 1; i++){
            part[j++] = g[i];
        }
        return part;
}

char *partone(char *g){
        char *part = malloc(Fsize);
        int j = 0;
        int i;
        for(i = 1; i < multilple_formulas(g); i++){
            part[j++] = g[i];
        }
        return part;
}

char bin(char *g){
        return g[multilple_formulas(g)];
}

void alpha_1(struct tableau *t, char *expanded){

    if (t->left){
        alpha_1(t->left, expanded);
    }

    if (t->right){
        alpha_1(t->right, expanded);
    }

    if(!(t->left) && !(t->right)){
        struct tableau *child = malloc(sizeof(*t));
        child->root = expanded;
        child->left = NULL;
        child->right = NULL;
        child->parent = t;
        t->left = child;
    }

}

void beta(struct tableau *t, char *left_string, char *right_string){

    if (t->left){
        beta(t->left, left_string, right_string);
    }

    if (t->right){
        beta(t->right, left_string, right_string);
    }

    if(!(t->left) && !(t->right)){
        struct tableau *left_child = malloc(sizeof(*t)), *right_child = malloc(sizeof(*t));

        // inititalise the left child
        left_child->root = left_string;
        left_child->left = NULL;
        left_child->right = NULL;
        left_child->parent = t;

        // initialise the right child
        right_child->root = right_string;
        right_child->left = NULL;
        right_child->right = NULL;
        right_child->parent = t;

        // add children to the parent node
        t->left = left_child;
        t->right = right_child;
    }
}


void alpha_2(struct tableau *t, char *left, char *right, char bin_conn){
    if (bin_conn == 'v'){
        alpha_1(t, add_negation(left));
        alpha_1(t, add_negation(right));
    } else if (bin_conn == '^'){
        beta(t, add_negation(left), add_negation(right));
    } else if (bin_conn == '>'){
        alpha_1(t, add_negation(add_negation(left)));
        alpha_1(t, add_negation(right));
    }
}


char* add_negation(char *string){
    char *negated_formula = malloc(Fsize);
    negated_formula[0] = '-';
    int i = 1;

    for(i = 0; i < strlen(string); i++){
        negated_formula[i + 1] = string[i];
    }
    return negated_formula;
}


int prop_position(char *prop){
    switch(prop[0]){
        case('p'): return 0; break;
        case('q'): return 1; break;
        case('r'): return 2; break;
        default: printf("Something wrong happened"); break;
    }
}


// returns 0 if its closed
int check_for_closed(int *array){
    if ((array[0] && array[1]) ||
        (array[2] && array[3]) ||
        (array[4] && array[5])){

            return 0;
        }
    return 1;
}


int *create_array_copy(int *array){
    int *array_copy = malloc(6*sizeof(int));

    int i;
    for(i = 0; i < 6; i++){
        array_copy[i] = array[i];
    }

    return array_copy;
}


void complete_recursive(struct tableau *t, int *array, int *result){
    if (strlen(t->root) == 1){
        array[prop_position(t->root) * 2] = 1;
    }

    if (strlen(t->root) == 2){
        array[prop_position(slicer(t->root, 1))*2 + 1] = 1;
    }

    // if we reached the end of a branch
    if (!(t->left) && !(t->right)){
        int is_closed = check_for_closed(array);
        result[0] = result[0] || is_closed;
    }
    // left tree exist, so pass a copy of the array to left tree
    if (t->left) {
        int *left_array = create_array_copy(array);
        complete_recursive(t->left, left_array, result);
    }

    // left tree exist, so pass a copy of the array to left tree
    if (t->right){
        int *right_array = create_array_copy(array);
        complete_recursive(t->right, right_array, result);
    }

}


void complete(struct tableau *t){
    // just a proposittion, no further expansion
    if (strlen(t->root) == 1);

    // just a negated proposition, ie., literals -> no further expansion
    else if (strlen(t->root) == 2 && (parse(t->root) == 2) && (parse(slicer(t->root, 1)) == 1));

    // double negated formula, use alpha-i expansion
    else if (strlen(t->root) > 2 && (parse(t->root) == 2) && (parse(slicer(t->root, 1)) == 2)) {
        char *expanded = slicer(t->root, 2);
        alpha_1(t, expanded);

    } else if (strlen(t->root) > 2 && (parse(t->root) == 3)){
        char *expanded_left = partone(t->root);
        char *expanded_right = parttwo(t->root);

        if (bin(t->root) == 'v'){
            beta(t, expanded_left, expanded_right);

        } else if (bin(t->root) == '^') {
            alpha_1(t, expanded_right);
            alpha_1(t, expanded_left);

        } else if (bin(t->root) == '>'){
            char *negated_left = add_negation(expanded_left);
            beta(t, negated_left, expanded_right);
        }

    } else if (strlen(t->root) > 2 && (parse(t->root) == 2) && (parse(slicer(t->root, 1)) == 3)){
        char *expanded_left = partone(slicer(t->root, 1));
        char *expanded_right = parttwo(slicer(t->root, 1));
        alpha_2(t, expanded_left, expanded_right, bin(slicer(t->root, 1)));
    }

    // recursively complete the left tree
    if(t->left){
        complete(t->left);
    }

    // recursively complete the right tree
    if(t->right){
        complete(t->right);
    }
}


int closed(struct tableau *t){
    /*
     * create an array of [0, 0, 0]
     * [p, -p, q, -q, r, -r]
     * increment the positional value by 1
     * if no left and right leaves, check if the branch had
     * a proposition and its negation, then the branch is closed
     * other wise the branch is open.
     * so before starting the exlporation,
     * create an array containing, False and pass doesn the regeerecne to the array
     * to every single complete recursive calls, and if any open branches is found,
     * update the False value to or. which will get updated on every leaves.
    */

    int *prop_presence = malloc(6*sizeof(int));

    int i;
    for(i = 0; i < 6; i++){
        prop_presence[i] = 0;
    }

    // 0 means not yet satisfiable, 1 means satisfiable
    int satisfiable[] = {0};

    complete_recursive(t, prop_presence, satisfiable);

    return !satisfiable[0];
}


int main(){ /*input a string and check if its a propositional formula */
    char *name = malloc(Fsize);
    FILE *fp, *fpout;

    /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

    int j;
    for(j=0;j<cases;j++){
      fscanf(fp, "%s",name);/*read formula*/

      switch (parse(name)){
          case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
          case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
          case(2): fprintf(fpout, "%s is a negation.  ", name);break;
          case(3):fprintf(fpout, "%s is a binary.  ", name);break;
          default:fprintf(fpout, "What the f***!  ");
      }

      /*make new tableau with name at root, no children, no parent*/

      struct tableau t={name, NULL, NULL, NULL};

      /*expand the root, recursively complete the children*/
      if (parse(name)!=0){
          complete(&t);
          if (closed(&t)) fprintf(fpout, "%s is not satisfiable.\n", name);
          else fprintf(fpout, "%s is satisfiable.\n", name);
      } else fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }
    fclose(fp);
    fclose(fpout);
    free(name);

    return(0);
}
