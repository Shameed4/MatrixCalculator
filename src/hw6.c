#include "hw6.h"

/**
 * takes a matrix and inserts it in to the bst contained in root
*/
bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    // create a new bst containing the matrix
    bst_sf *new = malloc(sizeof(bst_sf));
    new->mat = mat;
    new->left_child = NULL;
    new->right_child = NULL;

    // when root is null, the new bst becomes the root
    if (root == NULL)
        return new;
    
    // otherwise search for the root
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

/**
 * Finds the matrix with the given name, if it exists. Otherwise return null.
*/
matrix_sf* find_bst_sf(char name, bst_sf *root) {
    // simple searching based on whether name is less, greater, or equal
    while (root != NULL) {
        if (root->mat->name == name)
            return root->mat;
        
        if (name < root->mat->name)
            root = root->left_child;
        else
            root = root->right_child;
    }

    // name not found
    return NULL;
}

void free_bst_sf(bst_sf *root) {
    // base case: root is null
    if (root == NULL) {
        return;
    }

    // free children
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);

    // free self
    free(root->mat);
    free(root);
}

/**
 * Adds 2 matrices (assumes dimensions are compatible)
*/
matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // allocate memory for new matrix
    matrix_sf *sum = malloc(sizeof(matrix_sf) + mat1->num_rows * mat1->num_cols * sizeof(int));

    // copy dimensions of mat1
    *sum = *mat1;

    // add values of 2 matrices in 1D (since all 3 matrices have same size, no need to do it by row and column)
    for (unsigned int i = 0; i < sum->num_rows * sum->num_cols; i++) {
        sum->values[i] = mat1->values[i] + mat2->values[i];
    }
    return sum;
}

/**
 * Multiplies matrices mat1 and mat2
*/
matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // allocate necessary memory
    matrix_sf *product = malloc(sizeof(matrix_sf) + mat1->num_rows * mat2->num_cols * sizeof(int));

    // set rows and columns of new matrix
    product -> num_rows = mat1 -> num_rows;
    product -> num_cols = mat2 -> num_cols;

    // matrix multiplication algorithm
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

/**
 * Returns a transposed matrix
*/
matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    // allocate memory and set rows and columns
    matrix_sf *t = malloc(sizeof(matrix_sf) + mat->num_rows * mat->num_cols * sizeof(int));
    t->num_rows = mat->num_cols;
    t->num_cols = mat->num_rows;

    // swap rows and columns 
    for (unsigned int c = 0; c < mat->num_rows; c++) {
        for (unsigned int r = 0; r < mat->num_cols; r++) {
            t->values[r * t->num_cols + c] = mat->values[c * t->num_rows + r];
        }
    }
    return t;
}

/**
 * takes an expression and a name and uses it create a new matrix struct
*/
matrix_sf* create_matrix_sf(char name, const char *expr) {
    const char *curr = expr;

    // get the number of rows and columns, also setting curr to the point after the numbers
    int NR = strtol(curr, (char**)&curr, 10);
    int NC = strtol(curr, (char**)&curr, 10);
    
    // create the matrix 
    matrix_sf *m = malloc(sizeof(matrix_sf) + NR * NC * sizeof(int));
    m->name = name;
    m->num_rows = NR;
    m->num_cols = NC;

    while (*curr++ != '['); // move pointer to after the [
    int i = 0;

    // get each number of expression 
    while (*curr != ']') {
        while (isspace(*curr) || *curr == ';') {
            curr++;
        }
        if (*curr == '-' || isdigit(*curr)) {
            m->values[i] = strtol(curr, (char**)&curr, 10);
            i++;
        }
    }

    return m;
}

/**
 * Helper method for the conversion from postfix to infix. Returns priority of a token.
*/
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

/**
 * Converts an infix expression to postfix
*/
char* infix2postfix_sf(char *infix) {
    char stack[MAX_LINE_LEN+1] = {'$'};
    char *postfix = calloc(MAX_LINE_LEN + 1, sizeof(char));
    
    int infixIdx = 0; // index of current infix token
    int postfixIdx = 0; // length of postfix string so far
    int stackIdx = 1; // index to add to stack (1 because of $)
    
    while (infix[infixIdx] && infix[infixIdx] != '\n') {
        // add left parenthesis immediately
        if (infix[infixIdx] == '(')
            stack[stackIdx++] = '(';
        
        // when encountering right parenthesis, remove everything between 2 parenthesis
        else if (infix[infixIdx] == ')') {
            stackIdx--;
            while (stack[stackIdx] != '(') {
                postfix[postfixIdx++] = stack[stackIdx--];
            }
        }
        // when encountering an operation, check its priority
        else if (infix[infixIdx] == '+' || infix[infixIdx] == '*' || infix[infixIdx] == '\'') {
            while (priority(stack[stackIdx - 1]) >= priority(infix[infixIdx])) {
                postfix[postfixIdx++] = stack[--stackIdx];
            }
            stack[stackIdx++] = infix[infixIdx];
        }
        // when encountering a letter, add it directly to postfix string
        else if (isalpha(infix[infixIdx])) {
            postfix[postfixIdx++] = infix[infixIdx];
        }
        // if none of the if-statements are called, then it was probably a space
        infixIdx++;
    }
    // keep adding to stack until it is empty
    while (stack[--stackIdx] != '$') {
        postfix[postfixIdx++] = stack[stackIdx];
    }

    return postfix;
}

/**
 * returns a named matrix that is evaluated given the infex expression
*/
matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char *post = infix2postfix_sf(expr); // convert infix to postfix
    char postfix[strlen(post)+1];
    strcpy(postfix, post); // transfer memory from heap to stack
    free(post);
    char *postfixPtr = postfix;
    
    matrix_sf *stack[MAX_LINE_LEN];
    matrix_sf **stackPtr = stack;

    while (*postfixPtr != '\0') {
        // when encountering addition, pop 2 elements from the stack and add them, adding result to stack
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
        // when encountering multiplication, pop 2 elements from the stack and multliply them, adding result to stack
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
        // when encountering transpose, pop 1 element from the stack and transpose it, adding result to stack
        else if (*postfixPtr == '\'') {
            matrix_sf *mat = *--stackPtr;
            matrix_sf *eval = transpose_mat_sf(mat);
            if (!isalpha(mat->name))
                free(mat);
            *stackPtr++ = eval;
            eval->name = '?';
        }
        // when encountering a letter, find the pointer to the matrix and add it to stack
        else if (isalpha(*postfixPtr)) {
            matrix_sf *mat = find_bst_sf(*postfixPtr, root);
            *stackPtr++ = mat;
        }
        postfixPtr++;
    }
    
    if (!isalpha((*stack)->name)) {
        (*stack)->name = name;
        return *stack;
    }

    matrix_sf *ret = copy_matrix((*stack)->num_rows, (*stack)->num_cols, (*stack)->values);
    ret->name = name;
    return ret;
}

/**
 * Takes a script and executes all commands in it
*/
matrix_sf *execute_script_sf(char *filename) {
    FILE *f = fopen(filename, "r");
    
    char *line = NULL;
    size_t lineLength = 0;
    
    matrix_sf *last = NULL;
    bst_sf *root = NULL;
    while (getline(&line, &lineLength, f) != -1) {
        if (!strchr(line, '=')) {
            free(line);
            line = NULL;
            lineLength = 0;
            continue; 
        }
        int lineIdx = 0;
        char name;
        
        // find the name of the matrix
        while (!isalpha(line[lineIdx]))
            lineIdx++;   
        name = line[lineIdx++];
        
        // get past equal sign
        while (line[lineIdx++] != '=');
        
        if (strchr(line, '[')) { // creating a new matrix from scratch
            last = create_matrix_sf(name, &line[lineIdx]);
        }
        else { // evaluating a matrix expression
            last = evaluate_expr_sf(name, &line[lineIdx], root);
        }
        free(line);
        line = NULL;
        lineLength = 0;
        root = insert_bst_sf(last, root);
    }
    free(line);
    fclose(f);
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
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}