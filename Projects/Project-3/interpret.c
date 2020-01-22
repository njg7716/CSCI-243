// Author: Nicholas Graca
// Date: 12/5/17

#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// reads a file that is in fred and calls the respective functions depending on
// the fred command used
void readFredFile(FILE* fp, Table* t){
	char line [100];
	if(t == NULL){
		t = create();
	}
	printf("\n>");
	while(fgets(line, 100, fp)){
		printf("::: %s", line);
		char* tok = strtok(line, " ,");
		if(strcmp(tok, "define") == 0){
			define(t, tok);
		}
		if(strcmp(tok, "let") == 0){
			let(t, tok);
		}
		if(strcmp(tok, "prt") == 0){
			print(tok);
		}
		if(strcmp(tok, "display") == 0){
			display(t, tok);
		}
		if(strcmp(tok, "if") == 0){
			ifState(t, tok);
		}
		printf("\n>");
	}
	fclose(fp);
}

//opens the command line to enter in fred commands. It gets teh commands and
//then calls the respective function based on the fred command
void readFredCommand(Table* t){
	char buf[256];
	char* def = "> ";
	printf("\n%s", def);
	while(fgets(buf, 256, stdin)){
		printf("::: %s", buf);
		char* tok = strtok(buf, " ,\n");
		if(strcmp(tok, "define") == 0){
		define(t, tok);
		}
		if(strcmp(tok, "let") == 0){
			let(t, tok);
		}
		if(strcmp(tok, "prt") == 0){
			print(tok);
		}
		if(strcmp(tok, "display") == 0){
			display(t, tok);
		}
		if(strcmp(tok, "if") == 0){
			ifState(t, tok);
		}
		printf("\n%s", def);
	}
}

//tells if the first operator has a greater precedence than the second
bool getPrecedence(char* one, char* two){
	if(strcmp(one, "(") == 0){
		return true;
	}
	else if(strcmp(two, "(") == 0){
		return false;
	}
	else if((strcmp(two, "*") == 0) || (strcmp(two, "/") == 0)){
		if((strcmp(one, "*") == 0) || (strcmp(one, "/") == 0)){
			return true;
		}
		return false;
	}
	else{
		if(((strcmp(one, "+") == 0) || (strcmp(one, "-") == 0)) && ((strcmp(two, "+") == 0) || (strcmp(two, "-") == 0))){
			return true;
		}
	}
	return false;
}

//Adds Node to the stack
Node* push(Node* oldHead, Node* newHead){
	newHead->next = oldHead;
	return newHead;
}

//Removes node from stack and returns the operator
Node* pop(Node* head){
	Node* temp = head;
	head = head->next;
	return temp;
}

// Creates a new node
Node* createNode(char* name){
	Node* head = (Node*)malloc(sizeof(Node));
	if(head == NULL){
		fprintf(stderr, "Failed to allocate memory.\n");
	}
	strcpy(head->name, name);
	head->next = NULL;
	return head;
}

//Converts Infix expression to postfix
char** infixToPostfix(char** equ, Table* t){
	char** final = calloc(15, 8);
	if(final == NULL){
		fprintf(stderr, "Failed to allocate memory.\n");
	}
	// allocate space in the array but also go through teh infix array and replace
	// the variable with the value of it
	for(int y = 0; y < 15; y++){
		final[y] = calloc(1, 8);
		if(final[y] == NULL){
			fprintf(stderr, "Failed to allocate memory.\n");
		}
		//if there is a variable used, replace it with the value
		if(has(t, equ[y])){
			Symbol* sym = get(t, equ[y]);
			if(sym->type == Integer){
				int val = sym->value.iVal;
				char strVal[10];
				sprintf(strVal, "%d", val);
				strcpy(equ[y], strVal);
			}
			else{
				float val = sym->value.fVal;
				char strVal[10];
				sprintf(strVal, "%f", val);
				strcpy(equ[y], strVal);

			}
		}
	}
	int x = 0;
	Node* head = NULL;
	Node* temp = NULL;
	for(int i = 0; i < 15; i++){
		if(equ[i] == NULL){
			break;
		}
		//  Checks if there it is an operator, if so, add it to stack
		if(strcmp(equ[i], "+") == 0 || strcmp(equ[i], "-") == 0 || strcmp(equ[i], "*") == 0 || strcmp(equ[i], "/") == 0 || strcmp(equ[i], "(") == 0){
			if(head == NULL){
				head = createNode(equ[i]);
			}
			//check if head is not left parenthesis and head has greater or equal
			// precendence than equ[i] operator, pop and add to list, then push new
			else if(strcmp(head->name, "(") != 0){
				if(getPrecedence(head->name, equ[i]) == true){
					temp = pop(head);
					head = head->next;
					strcpy(final[x], temp->name);
					free(temp);
					x++;
					temp = createNode(equ[i]);
	 				head = push(head, temp);
				}
				//add operator to stack
				else{
					temp = createNode(equ[i]);
					head = push(head, temp);
				}
			}
			//add value to list
			else{
				temp = createNode(equ[i]);
				head = push(head, temp);
			}
		}

		// Checks if it is a right paranthesis, if so, if stack is not empty and head is not
		// left parenthesis, pop operator and discard right and left parenthesis
		else if(strcmp(equ[i], ")") == 0){
			if(head != NULL && strcmp(head->name, "(") != 0){
				temp = pop(head);
				head = head->next;
				strcpy(final[x], temp->name);
				x++;
				free(temp);
			}
			//if the top of the stack is a left parenthesis, discard it
			if(strcmp(head->name, "(") == 0){
				temp = pop(head);
				head = head->next;
				free(temp);
			}
		}
		// It must just be a number so add it to output list
		else{
			strcpy(final[x], equ[i]);
			x++;
		}
	}
	//pop the remainding operators onto the list
	while(head != NULL){
		temp = pop(head);
		head = head->next;
		strcpy(final[x], temp->name);
		x++;
		free(temp);
	}	
	return final;
}

//takes a string in postfix notation adn evaluates the value
float evaluatePostfix(char** final){
	Node* head = NULL;
	Node* temp = NULL;
	for(int i = 0; i < 15; i++){
		//checks if the string can be converted to a long and if it can then
		//put it on the stack
		if(strtoul(final[i], NULL, 10)){
			//if there is no stack, create one
			if(head == NULL){
				head = createNode(final[i]);
			}
			else{
				temp = createNode(final[i]);
				head = push(head, temp);
			}
		}
		//checks to see if there is an operator, if so pop the first two values
		//from the stack and then check to see what operator it is and do that
		else if(strcmp(final[i], "+") == 0 || strcmp(final[i], "-") == 0 || strcmp(final[i], "*") == 0 || strcmp(final[i], "/") == 0){
			temp = pop(head);
			head = head->next;
			float val1 = atof(temp->name);
			free(temp);
			temp = pop(head);
			head = head->next;
			float val2 = atof(temp->name);
			free(temp);
			char v[20];	
			if(strcmp(final[i], "+") == 0){
				float val = val1 + val2;
				sprintf(v, "%f", val);
				temp = createNode(v);
				head = push(head, temp);
			}
			else if(strcmp(final[i], "-") == 0){
				float val = val1 - val2;
				sprintf(v, "%f", val);
				temp = createNode(v);
				head = push(head, temp);
			}
			else if(strcmp(final[i], "/") == 0){
				float val = (float)(val2 / val1);
				sprintf(v, "%f", val);
				temp = createNode(v);
				head = push(head, temp);
			}
			else if(strcmp(final[i], "*") == 0){
				float val = val1 * val2;
				sprintf(v, "%f", val);
				temp = createNode(v);
				head = push(head, temp);
			}
		}	
	}
	//the last thing on the stack is the final answer
	float fin = atof(head->name);
	free(head);
	return fin;
}

//Handles the let command in fred
void let(Table* t, char* tok){
	tok = strtok(NULL, " :=");
	char* var = tok;
	tok = strtok(NULL, " :=");
	char** equ = calloc(15, 8);
	if(equ == NULL){
		fprintf(stderr, "Failed to allocate memory.\n");
	}
	for(int y = 0; y < 15; y++){
		equ[y] = calloc(1, 8);
		if(equ[y] == NULL){
			fprintf(stderr, "Failed to allocate memory.\n");
		}	
	}
	int x = 0;
	//this gets the equation to evaluate and the anwer will be the value of the variable
	while(tok != NULL){
		strcpy(equ[x], tok);
		x++;
		tok = strtok(NULL, " \n");
	}
	// evaluates the equation
	char** final = infixToPostfix(equ, t);
	float fin = evaluatePostfix(final);
	//checks to see if the variable was initialized already
	if(!has(t, var)){
		fprintf(stderr, "That variable has not been initialized.\n");
	}
	Symbol* v = get(t, var);
	Value value;
	// Sets the value depending on if it is a integer or float
	if(v->type == Integer){
		fin = (int)fin;
		value.iVal = fin;
	}
	else{
		value.fVal = fin;
	}
	v->value = value;
	for(int i = 0; i < 15; i++){
		free(final[i]);
		free(equ[i]);
	}
	free(final);
	free(equ);
}

//Handles the define command in fred
void define(Table* t, char* tok){
	//tok should be the type of variable
	tok = strtok(NULL, " \n");
	char* varType = tok;
	Type type;
	if(strcmp(varType, "integer") == 0){
		type = Integer;
	}
	else if(strcmp(varType, "real") == 0 || strcmp(varType, "float") == 0){
		type = Float;
	}
	else if(strcmp(varType, "unknown") == 0){
		type = Unknown;
	}
	//This will loop if there are more than one variables being initialized
	tok = strtok(NULL, " ,\n");
	while(tok != NULL){
		char* name = tok;
		Value value;
		if(strcmp(varType, "Integer") == 0){
			value.iVal = 0;
		}
		else{
			value.fVal = 0;
		}
		//puts thevariable in the Symbol Table
		put(t, name, value, type);
		tok = strtok(NULL, " ,\n");
	}
}

//handles the print command in Fred
void print(char* tok){
	char line[100];
	//gets the line the user wants to print
	tok = strtok(NULL, "\n");
	int len = strlen(tok);
	//puts that line into a string
	strcpy(line, tok);
	/**Goes through the string and prints one character at a time
	This removes the quotes and allows me to see if there is a 
	new line character**/
	for(int i = 1; i < len - 2; i++){
		if(line[i] == '\\'){
			if(line[i+1] == 'n'){
				printf("\n");
				i = i + 2;
			}
		}
		else
			printf("%c", line[i]);
	}
}

// Handles the display command in Fred
void display(Table* t, char* tok){
	//gets the anem of the variable to display
	tok = strtok(NULL, " \n");
	//gets the symbol pointer accosiated with the variable name
	Symbol* sym = get(t, tok);
	//depending on the variable type, print it
	if(sym->type == Integer){
		printf("%d\n", sym->value.iVal);	
	}
	else{
		printf("%f\n", sym->value.fVal);
	}
}

// Handles if statements in Fred. Including nested if statements
void ifState(Table* t, char* tok){
	char** equ = calloc(15, 8);
	if(equ == NULL){
		fprintf(stderr, "Failed to allocate memory.\n");
	}
	for(int y = 0; y < 15; y++){
		equ[y] = calloc(1, 8);
		if(equ[y] == NULL){
			fprintf(stderr, "Failed to allocate memory.\n");
		}
	}
	//gets the left side of the comparison and stops at the comparitor
	tok = strtok(NULL, " ");	
	int x = 0;
	while(strcmp(tok, "=") != 0 && strcmp(tok, "<") != 0 && strcmp(tok, ">") != 0){
		strcpy(equ[0], tok);
		x++;
		tok = strtok(NULL, " ");
	}
	//saves the comparitor
	char* op = tok;
	//computes the value of the left side
	char** final = infixToPostfix(equ, t);
	float val1 = evaluatePostfix(final);
	for(int i = 0; i < 15; i++){
		free(final[i]);
		free(equ[i]);
	}
	free(final);
	free(equ);
	equ = calloc(15, 8);
	if(equ == NULL){
		fprintf(stderr, "Failed to allocate memory.\n");
	}
	for(int y = 0; y < 15; y++){
		equ[y] = calloc(1, 8);
		if(equ[y] == NULL){
			fprintf(stderr, "Failed to allocate memory.\n");
		}
	}
	x = 0;
	// gets the right side of the comparitor and stops at the "then"
	while(strcmp(tok, "then") != 0){
		strcpy(equ[0], tok);
		x++;
		tok = strtok(NULL, " ");
	}
	final = infixToPostfix(equ, t);
	//computes the right side
	float val2 = evaluatePostfix(final);
	for(int i = 0; i < 15; i++){
		free(final[i]);
		free(equ[i]);
	}
	free(final);
	free(equ);
	bool exp = false;
	//does the comparison based on the comparitor
	if(strcmp(op, "=") == 0){
		if(val1 == val2){
			exp = true;
		}
	}
	if(strcmp(op, "<") == 0){
		if(val1 < val2){
			exp = true;
		}
	}
	if(strcmp(op, ">") == 0){
		if(val1 > val2){
			exp = true;
		}
	}
	//if the comparison is false just return
	if(exp == false){
		return;
	}
	//if its true you need to get what is after the "then"
	else{
		//I copied this code from my readFredFile function
		tok = strtok(NULL, " ,\n");
		if(strcmp(tok, "define") == 0){
			define(t, tok);
		}
		if(strcmp(tok, "let") == 0){
			let(t, tok);
		}
		if(strcmp(tok, "prt") == 0){
			print(tok);
		}
		if(strcmp(tok, "display") == 0){
			display(t, tok);
		}
		//handles nesteed if statements
		if(strcmp(tok, "if") == 0){
			ifState(t, tok);
		}
	}
}
