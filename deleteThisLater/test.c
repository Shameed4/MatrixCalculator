#include "../../include/hw6.h"

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
        else if ('A' <= *infixPtr && *infixPtr <= 'Z') {
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

void printMatrix(char *s) {
    printf("%s\n", s);
    free(s);
}

int main() {
    printMatrix(infix2postfix_sf("A*(B+C)"));
    printMatrix(infix2postfix_sf("A*(B+C)'"));
    printMatrix(infix2postfix_sf("A*(B+C)*D'"));
    printMatrix(infix2postfix_sf("((A+B)'*C)+D'"));
}