#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_LENGTH 50

struct _person;
typedef struct _person* Position;
typedef struct _person
{
	char name[MAX_LENGTH];
	char lastName[MAX_LENGTH];
	int birthYear;

	Position next;
}Person;

int Menu();

Position CreatePerson();
Position FindLast(Position head);
Position FindPrev(Position head, Position current);
int InsertAfter(Position position, Position newPerson);

int AddToBegin(Position head);
int AddToEnd(Position head);
int AddAfter(Position head, const char* surname);
int AddBefore(Position head, const char* surname);
int DeletePerson(Position head, const char* surname);
Position FindBySurname(Position head, const char* surname);

int PrintList(Position head);
int WriteListInFile(Position head);
int ReadListFromFile();
int SortList(Position head);
void FreeList(Position head);

int main()
{
	Menu();

	return 0;
}

int Menu()
{
	Person head = { .name = "", .lastName = "", .birthYear = 0, .next = NULL };

	char choose = '\0';
	int exit = 1;

	char surname[MAX_LENGTH] = {" "};
	Position sname = NULL;

	while (exit)
	{
		printf("H. Add person at the beginning of the list. \n");
		printf("E. Add person at the end of the list. \n");
		printf("A. Add after specific last name. \n");
		printf("B. Add before specific last name. \n");
		printf("D. Delete person. \n");
		printf("F. Find person by surname. \n");
		printf("P. Print list. \n");
		printf("W. Write to file. \n");
		printf("R. Read from file \n");
		printf("S. Sort list. \n");
		printf("X. Exit Menu. \n");

		printf("Enter an upper letter: ");
		scanf(" %c", &choose);

		switch (choose)
		{

		case 'H':
			AddToBegin(&head);
			break;
		case 'E':
			AddToEnd(&head);
			break;
		case 'A':
			printf("\nEnter the last name after which the new person will be added: ");
			scanf(" %s", surname);
			AddAfter(&head, surname);
			break;
		case 'B':
			printf("\nEnter the last name before which the new person will be added ");
			scanf(" %s", surname);
			AddBefore(&head, surname);
			break;
		case 'D':
			printf("\nEnter the last name of the person you want to delete: ");
			scanf(" %s", surname);
			DeletePerson(&head, surname);
			break;
		case 'F':
			printf("\nEnter the last name of the person you want to find");
			scanf(" %s", surname);
			sname = FindBySurname(&head, surname);
			if (sname != NULL)
				printf("Person found.\n");
			else
				printf("Person not found.\n");
			break;
		case 'P':
			PrintList(&head);
			break;
		case 'W':
			WriteListInFile(&head);
			break;
		case 'R':
			ReadListFromFile();
			break;
		case 'S':
			SortList(&head);
			break;
		case 'X':
			exit = 0;
			break;

		default:
			printf("ERROR! Choose again: ");
			break;
		}

		printf("\n");
	}

	FreeList(&head);
	return 0;
}

Position CreatePerson()
{
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL)
	{
		printf("Malloc failed!\n");
		return NULL;
	}

	newPerson->next = NULL;
	printf("Enter name, surname and birth year: ");
	scanf(" %s %s %d", newPerson->name, newPerson->lastName, &newPerson->birthYear);

	return newPerson;
}

Position FindLast(Position head)
{
	Position last = head;
	while (last->next != NULL)
	{
		last = last->next;
	}
	
	return last;
}

Position FindPrev(Position head, Position current)
{
	Position prev = head;
	while (prev != NULL && prev->next != current)
	{
		prev = prev->next;
	}

	return prev;
}

int InsertAfter(Position position, Position newPerson)
{
	newPerson->next = position->next;
	position->next = newPerson;
}

int AddToBegin(Position head)
{
	Position temp = head;
	Position newPerson = CreatePerson();
	if (newPerson == NULL)
	{
		return -1;
	}

	InsertAfter(temp, newPerson);

	return 0;
}

int AddToEnd(Position head)
{
	Position last = FindLast(head);

	Position newPerson = CreatePerson();
	if (newPerson == NULL)
	{
		return -1;
	}

	InsertAfter(last, newPerson);
	return 0;
}

int AddAfter(Position head, const char* surname)
{
	Position newPerson = CreatePerson();
	Position position = FindBySurname(head, surname);
	InsertAfter(position, newPerson);

	return 0;
}

int AddBefore(Position head, const char* surname)
{
	Position newPerson = CreatePerson();
	Position position = FindBySurname(head, surname);
	Position prev = FindPrev(head, position);

	InsertAfter(prev, newPerson);

	return 0;
}

Position FindBySurname(Position head, const char* surname)
{
	Position temp = head->next;
	if (temp == NULL)
		return -1;

	while (temp != NULL && strcmp(temp->lastName, surname) != 0)
	{
		temp = temp->next;
	}

	return temp;
	
}

int DeletePerson(Position head, const char* surname)
{
	Position toDelete = FindBySurname(head, surname);
	Position prev = FindPrev(head, toDelete);

	prev->next = toDelete->next;
	free(toDelete);

	return 0;
}

int PrintList(Position head)
{
	Position temp = head->next;

	printf("\n");
	while (temp != NULL)
	{
		printf("Person:  %s %s %d\n", temp->name, temp->lastName, temp->birthYear);
		temp = temp->next;
	}
	
	return 0;
}

int WriteListInFile(Position head)
{
	Position temp = head->next; 

	FILE* f = NULL;
	f = fopen("persons.txt", "w");

	while (temp != NULL) {
		fprintf(f, "%s %s %d\n", temp->name, temp->lastName, temp->birthYear);
		temp = temp->next;
	}

	fclose(f);
	printf("List written successfully.\n");

	return 0;
}

int ReadListFromFile()
{
	FILE* f;
	f = fopen("persons.txt", "r");

	while (!feof(f)) {
		char name[MAX_LENGTH], lastName[MAX_LENGTH];
		int birthYear;
		fscanf(f, "%s %s %d\n", name, lastName, &birthYear);
		printf("%s %s %d\n", name, lastName, birthYear);
	}

	printf("List read successfully.\n");
	fclose(f);

	return 0;
}

int SortList(Position head) {
	bool swapped;
	Position current;

	do {
		swapped = false;
		current = head;

		while (current->next && current->next->next) {
			if (strcmp(current->next->lastName, current->next->next->lastName) > 0) {
				Position temp = current->next;
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
				swapped = true;
			}
			current = current->next;
		}
	} while (swapped);

	return 0;
}

void FreeList(Position head) {
	Position temp = NULL;

	while (head->next != NULL) {
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
}