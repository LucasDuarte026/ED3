#ifndef STACK
#define STACK

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

Stack* createStack(unsigned capacity);
void push(Stack* stack, int item);
int pop(Stack* stack);
int isStackEmpty(Stack* stack);
void freeStack(Stack* stack);

#endif