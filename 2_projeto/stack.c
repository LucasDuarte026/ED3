#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Estrutura auxiliar para o armazenamento dos vertices para a funcionalidade 11
Stack* createStack(unsigned capacity) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*) malloc(stack->capacity * sizeof(int));
    return stack;
}

void push(Stack* stack, int item) {
    if (stack->top == stack->capacity - 1)
        return;
    stack->array[++stack->top] = item;
}

int pop(Stack* stack) {
    if (stack->top == -1)
        return -1;
    return stack->array[stack->top--];
}

int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void freeStack(Stack* stack) {
    if (stack != NULL) {
        free(stack->array);
        free(stack);
    }
}
