#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define MAX_LENGHT (1024)


typedef struct stackElement* position;

typedef struct stackElement {
	double number;
	position next;

}StackElement;


int readFromFileAndCalculatePostfix(position top, double* result, char* postfixEq);
int push(double number, position top);
int checkStackAndExtractResultUsingOperator(position top, char operator);
int pop(position top);

int main() {
	StackElement head = { 0, NULL };
	char postfix[MAX_LENGHT] = { 0 };
	double result = 0;

	if (readFromFileAndCalculatePostfix(&head, &result, postfix) == EXIT_SUCCESS)
		printf("\nRezultat je: %lf\n", result);


	return EXIT_SUCCESS;
}

int readFromFileAndCalculatePostfix(position top, double* result, char* postfix) {
	FILE* fileName = NULL;
	int numBytes = 0, status = 0;
	double number = 0;
	char operator='\0';

	fileName = fopen("postfix.txt", "r");

	if (!fileName) {
		printf("\nUnable to open file postfix.txt\n");

		return EXIT_FAILURE;
	}

	fgets(postfix, MAX_LINE, fileName);

	while (strlen(postfix) > 0) {

		status = sscanf(postfix, " %lf %n", &number, &numBytes);

		if (status == 1) {

			push(number, top);
		}

		else {

			sscanf(postfix, " %c %n", &operator, &numBytes);

			checkStackAndExtractResultUsingOperator(top, operator);

		}

		postfix += numBytes;
	}

	*result = pop(top);

	fclose(fileName);

	return EXIT_SUCCESS;
}

int checkStackAndExtractResultUsingOperator(position top, char operator) {
	double operandRes = 0;
	int counter = 0;

	switch (operator) {

	case '+':

		operandRes = 0;

		for (counter = 0; counter < 2; counter++) {

			operandRes += top->next->number;

			pop(top);
		}

		push(operandRes, top);

		break;

	case '-':

		operandRes = 0;

		for (counter = 0; counter < 2; counter++) {

			if (operandRes == 0)
				operandRes = top->next->number;

			else
				operandRes = top->next->number - operandRes;

			pop(top);
		}

		push(operandRes, top);

		break;

	case '*':

		operandRes = 0;

		for (counter = 0; counter < 2; counter++) {
			;

			if (operandRes == 0)
				operandRes = top->next->number;

			else
				operandRes *= top->next->number;

			pop(top);
		}

		push(operandRes, top);

		break;

	case '/':

		operandRes = 0;

		for (counter = 0; counter < 2; counter++) {

			if (operandRes == 0)
				operandRes = top->next->number;

			else
				operandRes = top->next->number / operandRes;

			pop(top);
		}

		push(operandRes, top);

		break;

	default:
		printf("\nOperation %c not supported yet!\r\n", operator);
		return EXIT_FAILURE;

	}


	return EXIT_SUCCESS;
}

int push(double number, position top) {
	position newElement = NULL;

	newElement = malloc(sizeof(StackElement));

	if (!newElement) {
		printf("\nUnable to allocate memory for newElement!\n");

		return EXIT_FAILURE;
	}

	newElement->number = number;

	newElement->next = top->next;
	top->next = newElement;


	return EXIT_SUCCESS;
}

int pop(position top) {
	position toDel = NULL;
	int poppedRes = 0;

	toDel = top->next;
	top->next = toDel->next;

	poppedRes = toDel->number;

	free(toDel);

	return poppedRes;
}