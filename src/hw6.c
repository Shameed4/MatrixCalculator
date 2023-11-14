#include "hw6.h"

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
    char *postfix = malloc(sizeof(char) * MAX_LINE_LEN+1);
    
    char *infixPtr = infix; // points to current infix character being evaluated
    char *postfixPtr = postfix; // points to where next element should be added
    char *stackPtr = stack; // points to where next element should be added
    
    while (*infixPtr != '\0') {
        if (*infixPtr == '(')
            *stackPtr++ = '(';
        else if (*infixPtr == ')') {
            stackPtr--;
            while (*stackPtr != '(') {
                *postfixPtr++ = *stackPtr;
                stackPtr--;
            }
        }
        else if (*infixPtr == '+' || *infixPtr == '*' || *infixPtr == '\'') {
            while (priority(*(stackPtr-1)) >= priority(*infixPtr)) {
                *postfixPtr++ = *(stackPtr-1);
                stackPtr--;
            }
            *stackPtr++ = *infixPtr;
        }
        else if (isalpha(*infixPtr)) {
            *postfixPtr++ = *infixPtr;
        }
        infixPtr++;
    }
    while (*(stackPtr-1) != '$') {
        *postfixPtr++ = *(stackPtr-1);
        stackPtr--;
    }
    
    *postfixPtr++ = '\0';

    return postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char *post = infix2postfix_sf(expr);
    char postfix[strlen(post)+1];
    strcpy(postfix, post);
    free(post);
    char *postfixPtr = postfix;
    
    fprintf(stderr, "Test1\n");
    matrix_sf *stack[MAX_LINE_LEN];
    matrix_sf **stackPtr = stack;

    while (*postfixPtr != '\0') {
        fprintf(stderr, "%c\n", *postfixPtr);
        if (*postfixPtr == '+') {
            matrix_sf *mat2 = *--stackPtr;
            if (!isalpha(mat2->name)) {
                printf("freed: %c\n", mat2->name);
                free(mat2);
            }
            matrix_sf *mat1 = *--stackPtr;
            if (!isalpha(mat1->name)) {
                printf("freed: %c\n", mat1->name);
                free(mat1);
            }
            matrix_sf *eval = add_mats_sf(mat1, mat2);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        else if (*postfixPtr == '*') {
            matrix_sf *mat2 = *--stackPtr;
            if (!isalpha(mat2->name)) {
                printf("freed: %c\n", mat2->name);
                free(mat2);
            }
            matrix_sf *mat1 = *--stackPtr;
            if (!isalpha(mat1->name)) {
                printf("freed: %c\n", mat1->name);
                free(mat1);
            }
            matrix_sf *eval = mult_mats_sf(mat1, mat2);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        else if (*postfixPtr == '\'') {
            matrix_sf *mat = *--stackPtr;
            if (!isalpha(mat->name)) {
                printf("freed: %c\n", mat->name);
                free(mat);
            }
            matrix_sf *eval = transpose_mat_sf(mat);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        else if (isalpha(*postfixPtr)) {
            matrix_sf *mat = find_bst_sf(*postfixPtr, root);
            *stackPtr++ = mat;
        }
        postfixPtr++;
    }
    return *stack;
}

matrix_sf *execute_script_sf(char *filename) {
   return NULL;
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
