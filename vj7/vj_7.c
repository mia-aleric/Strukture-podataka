#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Directory {
    char name[50];
    struct Directory* subdirectories;
    struct Directory* next;
};

struct Directory* createDirectory(const char* name) {
    struct Directory* newDir = (struct Directory*)malloc(sizeof(struct Directory));
    strcpy(newDir->name, name);
    newDir->subdirectories = NULL;
    newDir->next = NULL;
    return newDir;
}

void addDirectory(struct Directory* currentDir, const char* name) {
    struct Directory* newDir = createDirectory(name);
    newDir->next = currentDir->subdirectories;
    currentDir->subdirectories = newDir;
}

void listDirectory(struct Directory* currentDir) {
    struct Directory* temp = currentDir->subdirectories;
    printf("Contents of directory %s:\n", currentDir->name);
    while (temp != NULL) {
        printf("- %s\n", temp->name);
        temp = temp->next;
    }
}

struct Directory* changeDirectory(struct Directory* currentDir, const char* name) {
    struct Directory* temp = currentDir->subdirectories;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    printf("Directory %s does not exist.\n", name);
    return currentDir;
}

int main() {
    struct Directory* rootDir = createDirectory("root");
    struct Directory* currentDir = rootDir;

    int choice;
    char dirName[50];

    do {
        printf("\nMenu:\n");
        printf("1 - md (create a new directory)\n");
        printf("2 - cd dir (change directory)\n");
        printf("3 - cd.. (move to the parent directory)\n");
        printf("4 - dir (list contents of the current directory)\n");
        printf("5 - exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("\nEnter the name of the new directory: ");
            scanf("%s", dirName);
            addDirectory(currentDir, dirName);
            break;
        case 2:
            printf("\nEnter the name of the directory: ");
            scanf("%s", dirName);
            currentDir = changeDirectory(currentDir, dirName);
            break;
        case 3:
            if (currentDir != rootDir) {
                currentDir = rootDir;
                printf("\nMoved to the root directory.\n");
            }
            else {
                printf("\nAlready in the root directory.\n");
            }
            break;
        case 4:
            listDirectory(currentDir);
            break;
        case 5:
            printf("\nExiting the program.\n");
            break;
        default:
            printf("\nUnknown option. Please choose again.\n");
        }
    } while (choice != 5);

    free(rootDir);

    return 0;
}