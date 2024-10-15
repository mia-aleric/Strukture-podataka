#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ERROR_OPENING_FILE 1
#define BUFFER_SIZE 1024
#define SIZE 128
#define MAX_POINTS 50

typedef struct _student {
	char name[SIZE];
	char surname[SIZE];
	int points;
} Student;

int count_rows(char* filename);
void read_students(Student* base, char* filename);
void print_students(Student* base, int rows);

int main()
{
	int rows = 0;
	Student* base;
	
	rows = count_rows("lista.txt");
	base = malloc(sizeof(Student) * rows);

	read_students(base, "lista.txt");
	print_students(base, rows);

	return 0;
}

int count_rows(char* filename)
{
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	int row_count = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Error opening file!\n");
		return ERROR_OPENING_FILE;
	}
	while (!feof(fp))
	{
		fgets(buffer, BUFFER_SIZE, fp);
		//sscanf() - dr
		++row_count;
	}

	fclose(fp);

	return row_count;
}

void read_students(Student* base, char* filename) {
	FILE* fp = NULL;
	int i = 0;

	fp = fopen(filename, "r");

	while (!feof(fp))
	{
		fscanf(fp, "%s %s %d", base[i].name, base[i].surname, &base[i].points);
		i++;
	}
	fclose(fp);
}

void print_students(Student* base, int rows)
{
	int i = 0;
	double relativePoints = 0.0;

	for (i = 0; i < rows; i++)
	{
		relativePoints = ((double)base[i].points / MAX_POINTS) * 100;
		printf("\nStudent %s %s je ostvario/la apsolutan br. bodova %d i relativan br. bodova %lf .\n", base[i].name, base[i].surname, base[i].points, relativePoints);

	}
}
