// Author: Nicholas Graca
// Date: 12/3/17

#include "symbolTable.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct Node{
	char name[8];
	struct Node* next;
} Node;

void readFredFile(FILE* fp, Table* t);

void readFredCommand();

char** infixToPostfix(char** equ, Table* t);

float evaluatePostfix();

void define(Table* t, char* tok);

void let(Table* t, char* tok);

bool getPrecedence(char* one, char* two);

Node* push(Node* oldHead, Node* newHead);

Node* pop(Node* head);

Node* createNode(char* name);

void let(Table* t, char* tok);

void define(Table* t, char* tok);

void print(char* tok);

void display(Table* t, char* tok);

void ifState(Table* t, char* tok);
