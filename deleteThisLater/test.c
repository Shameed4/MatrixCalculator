#include "../include/hw6.h"
#define TEST_TIMEOUT 10
#define TEST_INPUT_DIR "tests.in"
#define TEST_OUTPUT_DIR "tests.out"
static char test_log_outfile[100];

bst_sf* build_bst() {
    matrix_sf *A = copy_matrix(3, 5, (int[]){-4, 18, 6, 7, 10, -14, 29, 8, 21, -99, 0, 7, 5, 2, -9});
    A->name = 'A';
    matrix_sf *B = copy_matrix(3, 5, (int[]){10, 9, -2, -33, 22, 44, 10, 12, 72, 52, -88, 17, 16, 14, -9});
    B->name = 'B';
    matrix_sf *C = copy_matrix(1, 4, (int[]){-123, 47, -4, 140});  
    C->name = 'C';
    matrix_sf *D = copy_matrix(1, 4, (int[]){-16, 122, 135, 107});
    D->name = 'D';
    matrix_sf *E = copy_matrix(6, 4, (int[]){83, -22, 56, -1, 97, 94, 135, -10, 84, 40, -83, -4, 79, 28, 52, -101, 138, 146, 99, 0, -23, -73, -39, -47});
    E->name = 'E';
    matrix_sf *F = copy_matrix(4, 7, (int[]){-77, -20, 111, -2, 41, 117, 118, 21, -29, -45, 135, 98, 54, 131, 54, 1, 80, 143, -127, 148, 114, -81, 87, -33, -2, -6, 115, 59});
    F->name = 'F';
    matrix_sf *G = copy_matrix(7, 1, (int[]){-38, 4, 46, -14, -102, -72, -27});
    G->name = 'G';
    matrix_sf *H = copy_matrix(1, 5, (int[]){52, 65, -94, -73, -48});
    H->name = 'H';
    matrix_sf *I = copy_matrix(4, 4, (int[]){-7, 78, -87, -113, -144, -94, 22, -75, -137, -130, -113, -106, 85, -120, 50, 55});
    I->name = 'I';
    matrix_sf *J = copy_matrix(6, 3, (int[]){121, -1, 128, 78, -138, 138, -61, 51, -35, -84, 125, -83, -78, 138, 2, 81, -5, -36});
    J->name = 'J';
    bst_sf* Anode = malloc(sizeof(bst_sf));
    Anode->mat = A;
    Anode->left_child = NULL;
    Anode->right_child = NULL;
    bst_sf* Cnode = malloc(sizeof(bst_sf));
    Cnode->mat = C;
    Cnode->left_child = NULL;
    Cnode->right_child = NULL;
    bst_sf* Hnode = malloc(sizeof(bst_sf));
    Hnode->mat = H;
    Hnode->left_child = NULL;
    Hnode->right_child = NULL;
    bst_sf* Jnode = malloc(sizeof(bst_sf));
    Jnode->mat = J;
    Jnode->left_child = NULL;
    Jnode->right_child = NULL;
    bst_sf* Bnode = malloc(sizeof(bst_sf));
    Bnode->mat = B;
    Bnode->left_child = Anode;
    Bnode->right_child = Cnode;
    bst_sf* Fnode = malloc(sizeof(bst_sf));
    Fnode->mat = F;
    Fnode->left_child = NULL;
    Fnode->right_child = NULL;
    bst_sf* Inode = malloc(sizeof(bst_sf));
    Inode->mat = I;
    Inode->left_child = Hnode;
    Inode->right_child = Jnode;
    bst_sf* Dnode = malloc(sizeof(bst_sf));
    Dnode->mat = D;
    Dnode->left_child = Bnode;
    Dnode->right_child = NULL;
    bst_sf* Gnode = malloc(sizeof(bst_sf));
    Gnode->mat = G;
    Gnode->left_child = Fnode;
    Gnode->right_child = Inode;
    bst_sf* Enode = malloc(sizeof(bst_sf));
    Enode->mat = E;
    Enode->left_child = Dnode;
    Enode->right_child = Gnode;
    return Enode;
}

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    bst_sf *new = malloc(sizeof(bst_sf));
    new->mat = mat;
    new->left_child = NULL;
    new->right_child = NULL;
    if (root == NULL)
        return new;
    
    bst_sf *curr = root;
    while (1) {
        if (mat->name < curr->mat->name) {
            if (curr->left_child == NULL) {
                curr->left_child = new;
                return root;
            }
            curr = curr->left_child;
        }
        else {
            if (curr->right_child == NULL) {
                curr->right_child = new;
                return root;
            }
            curr = curr->right_child;
        }
    }
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    while (root != NULL) {
        if (root->mat->name == name)
            return root->mat;
        
        if (name < root->mat->name)
            root = root->left_child;
        else
            root = root->right_child;
    }
    return NULL;
}

void free_bst_sf(bst_sf *root) {
    if (root == NULL) {
        return;
    }
    free(root->mat);
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);
    free(root);
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    matrix_sf *sum = malloc(sizeof(matrix_sf) + mat1->num_rows * mat1->num_cols * sizeof(int));
    *sum = *mat1;
    for (unsigned int i = 0; i < sum->num_rows * sum->num_cols; i++) {
        sum->values[i] = mat1->values[i] + mat2->values[i];
    }
    return sum;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    matrix_sf *product = malloc(sizeof(matrix_sf) + mat1->num_rows * mat2->num_cols * sizeof(int));
    product -> num_rows = mat1 -> num_rows;
    product -> num_cols = mat2 -> num_cols;
    for (unsigned int r = 0; r < product->num_rows; r++) {
        for (unsigned int c = 0; c < product->num_cols; c++) {
            product->values[r * product->num_cols + c] = 0;
            for (unsigned x = 0; x < mat1->num_cols; x++) {
                product->values[r * product->num_cols + c] += mat1->values[r * mat1->num_cols + x] * mat2->values[x * mat2->num_cols + c];
            }
        }
    }
    return product;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    matrix_sf *t = malloc(sizeof(matrix_sf) + mat->num_rows * mat->num_cols * sizeof(int));
    t->num_rows = mat->num_cols;
    t->num_cols = mat->num_rows;

    for (unsigned int c = 0; c < mat->num_rows; c++) {
        for (unsigned int r = 0; r < mat->num_cols; r++) {
            t->values[r * t->num_cols + c] = mat->values[c * t->num_rows + r];
        }
    }
    return t;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    char *curr = expr;
    int NR = strtol(curr, &curr, 10);
    int NC = strtol(curr, &curr, 10);
    matrix_sf *m = malloc(sizeof(matrix_sf) + NR * NC * sizeof(int));
    m->name = name;
    m->num_rows = NR;
    m->num_cols = NC;

    while (*curr++ != '[');
    int i = 0;
    while (*curr != ']') {
        while (isspace(*curr) || *curr == ';') {
            curr++;
        }
        if (*curr == '-' || isdigit(*curr)) {
            m->values[i] = strtol(curr, &curr, 10);
            i++;
        }
    }

    return m;
}

char priority(char c) {
    switch (c) {
        case '$':
            return 0;
            break;
        case '(':
            return 1;
            break;
        case '+':
            return 2;
            break;
        case '*':
            return 3;
            break;
        case '\'':
            return 4;
            break;
        default:
            return -1; 
    }
}

char* infix2postfix_sf(char *infix) {
    char stack[MAX_LINE_LEN+1] = {'$'};
    char *postfix = calloc(MAX_LINE_LEN + 1, sizeof(char));
    
    int infixIdx = 0;
    int postfixIdx = 0;
    int stackIdx = 1;
    
    while (infix[infixIdx] && infix[infixIdx] != '\n') {
        if (infix[infixIdx] == '(')
            stack[stackIdx++] = '(';
        
        else if (infix[infixIdx] == ')') {
            stackIdx--;
            while (stack[stackIdx] != '(') {
                postfix[postfixIdx++] = stack[stackIdx--];
            }
        }
        else if (infix[infixIdx] == '+' || infix[infixIdx] == '*' || infix[infixIdx] == '\'') {
            while (priority(stack[stackIdx - 1]) >= priority(infix[infixIdx])) {
                postfix[postfixIdx++] = stack[--stackIdx];
            }
            stack[stackIdx++] = infix[infixIdx];
        }
        else if (isalpha(infix[infixIdx])) {
            postfix[postfixIdx++] = infix[infixIdx];
        }
        infixIdx++;
    }
    while (stack[--stackIdx] != '$') {
        postfix[postfixIdx++] = stack[stackIdx];
    }

    return postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char *post = infix2postfix_sf(expr);
    char postfix[strlen(post)+1];
    strcpy(postfix, post);
    free(post);
    fprintf(stderr, "Postfix expression: %s\n", postfix);
    char *postfixPtr = postfix;
    
    matrix_sf *stack[MAX_LINE_LEN];
    matrix_sf **stackPtr = stack;

    while (*postfixPtr != '\0') {
        fprintf(stderr, "%c\n", *postfixPtr);
        if (*postfixPtr == '+') {
            matrix_sf *mat2 = *--stackPtr;
            matrix_sf *mat1 = *--stackPtr;
            matrix_sf *eval = add_mats_sf(mat1, mat2);
            if (!isalpha(mat1->name))
                free(mat1);
            if (!isalpha(mat2->name))
                free(mat2);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        else if (*postfixPtr == '*') {
            matrix_sf *mat2 = *--stackPtr;
            matrix_sf *mat1 = *--stackPtr;
            matrix_sf *eval = mult_mats_sf(mat1, mat2);
            if (!isalpha(mat1->name))
                free(mat1);
            if (!isalpha(mat2->name))
                free(mat2);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        else if (*postfixPtr == '\'') {
            matrix_sf *mat = *--stackPtr;
            matrix_sf *eval = transpose_mat_sf(mat);
            if (!isalpha(mat->name))
                free(mat);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        else if (isalpha(*postfixPtr)) {
            matrix_sf *mat = find_bst_sf(*postfixPtr, root);
            *stackPtr++ = mat;
        }
        postfixPtr++;
    }
    (*stack)->name = name;
    return *stack;
}

matrix_sf *execute_script_sf(char *filename) {
    FILE *f = fopen(filename, "r");
    
    char *line = NULL;
    size_t lineLength = 0;
    
    matrix_sf *last = NULL;
    bst_sf *root = NULL;
    while (getline(&line, &lineLength, f) != -1) {
        fprintf(stderr, "Test 1 Line: %s\n", line);
        if (!strchr(line, '='))
            break;
        fprintf(stderr, "Test 2\n");
        int lineIdx = 0;
        char name;
        while (!isalpha(line[lineIdx]))
            lineIdx++;
    
        name = line[lineIdx++];
        fprintf(stderr, "Test 3\n");
        while (line[lineIdx++] != '=');
        
        fprintf(stderr, "Line 3/4: %s\n", line+lineIdx);
        if (strchr(line, '[')) {
            fprintf(stderr, "Before creating matrix\n");
            last = create_matrix_sf(name, &line[lineIdx]);
            fprintf(stderr, "After creating. Name: %c\n", last->name);
        }
        else {
            fprintf(stderr, "Before evaluating expression. Expression: '%s'\n", &line[lineIdx]);
            last = evaluate_expr_sf(name, &line[lineIdx], root); // I believe new line is causing this issue?
            fprintf(stderr, "After evaluating expression\n");
        }
        fprintf(stderr, "Test 4\n");
        free(line);
        fprintf(stderr, "Test 5\n");
        line = NULL;
        lineLength = 0;
        fprintf(stderr, "Test 6\n");
        root = insert_bst_sf(last, root);
        fprintf(stderr, "Test 7\n");
    }
    matrix_sf *ret = copy_matrix(last->num_rows, last->num_cols, last->values);
    ret->name = last->name;

    free_bst_sf(root);

    return ret;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}

int main() {
    bst_sf *root = build_bst();
    matrix_sf* result = execute_script_sf("../tests.in/script01.txt");
    print_matrix_sf(result);
}