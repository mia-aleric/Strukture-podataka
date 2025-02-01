#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MAX 1024

typedef struct treeNode* treePointer;
struct treeNode {
    char cityName[MAX];
    int population;
    treePointer left;
    treePointer right;
};

struct statelist;
typedef struct statelist* position;
typedef struct statelist {
    char stateName[MAX];
    treePointer nextTree;
    position next;
}linkedList;

treePointer CreateTreeRoot();
position CreateHeadList();
int PrintList(linkedList);
int InsertInList(position, position);
position CreateListEl(char[]);
int ReadStateFromFile(char[], linkedList);
int ReadCityFromFile(position, char[]);
int PrintInorder(treePointer);
treePointer CreateTreeEl(char[], int);
position FindState(linkedList, char[]);
treePointer InsertInTree(treePointer, char[], int);
int PrintCitiesLargerThan(treePointer, int);
int Menu(linkedList states);

int main() {

    linkedList states;
    states.next = CreateHeadList();
    states.nextTree = CreateTreeRoot();

    ReadStateFromFile("drzave.txt", states);

    //  PrintList(states);

    Menu(states);

    return 0;
}

treePointer CreateTreeRoot() {

    treePointer root = NULL;

    root = malloc(sizeof(struct treeNode));

    if (!root) {
        printf("Greska u alokaciji memorije!\n");
        return NULL;
    }

    strcpy(root->cityName, "");
    root->left = NULL;
    root->right = NULL;
    root->population = 0;

    return	root;
}

int InsertInList(position newElement, position head) {

    if (head->next == NULL) {
        head->next = newElement;
        newElement->next = NULL;
    }

    else {
        while (head->next != NULL && (strcmp(head->next->stateName, newElement->stateName) < 0))
            head = head->next;
        newElement->next = head->next;
        head->next = newElement;
    }

    return 0;
}

position CreateListEl(char stateName[]) {

    position newElement = NULL;

    newElement = (position)malloc(sizeof(struct statelist));

    if (!newElement) {
        printf("Greska u alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newElement->stateName, stateName);
    newElement->next = NULL;
    newElement->nextTree = CreateTreeRoot();

    return newElement;
}

int ReadStateFromFile(char stateFile[], linkedList states) {

    FILE* fp = NULL;
    char stateName[MAX] = { 0 };
    char cityfile[MAX] = { 0 };
    position newElement = NULL;
    position current = states.next;

    newElement = (position)malloc(sizeof(linkedList));

    if (!newElement) {
        printf("Greska u alokacjii memorije!\n");
        return 0;
    }

    fp = fopen(stateFile, "r");

    if (!fp) {
        printf("Datoteka %s se ne moze otvoriti!\n", stateFile);
        return EXIT_FAILURE;
    }

    while (!feof(fp)) {
        fscanf(fp, "%[^,],%s\n", stateName, cityfile);
        newElement = CreateListEl(stateName);
        InsertInList(newElement, states.next);
        ReadCityFromFile(newElement, cityfile);
    }

    fclose(fp);

    return 0;
}

int ReadCityFromFile(position head, char cityfile[]) {

    FILE* fp = NULL;
    char cityName[MAX] = { 0 };
    int population = 0;
    treePointer newElement = NULL;
    treePointer root = head->nextTree;

    fp = fopen(cityfile, "r");

    if (!fp) {
        printf("Datoteka %s se ne moze otvoriti!\n", cityfile);
        return EXIT_FAILURE;
    }

    while (!feof(fp)) {
        fscanf(fp, "%[^,],%d\n", cityName, &population);
        newElement = CreateTreeRoot();
        newElement = InsertInTree(root, cityName, population);
    }

    fclose(fp);

    return 0;
}

int PrintInorder(treePointer root) {

    if (root == NULL)
        return 0;

    PrintInorder(root->left);
    if (root->population != 0)
        printf("    %s (%d population)\n", root->cityName, root->population);
    PrintInorder(root->right);
    return 0;
}

treePointer CreateTreeEl(char cityName[], int population) {

    treePointer newElement = NULL;

    newElement = (treePointer)malloc(sizeof(struct treeNode));

    if (!newElement) {
        printf("Greska u alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newElement->cityName, cityName);
    newElement->population = population;
    newElement->left = NULL;
    newElement->right = NULL;

    return newElement;
}

position FindState(linkedList states, char stateName[]) {

    position current = NULL;

    current = (position)malloc(sizeof(linkedList));

    if (!current) {
        printf("Greska u alokaciji memorije!\n");
        return NULL;
    }

    current = states.next;

    while (current->next != NULL && (strcmp(current->next->stateName, stateName)))
        current = current->next;

    if ((current->next == NULL || strcmp(stateName, current->next->stateName))) {
        return NULL;
    }

    current = current->next;

    return current;
}

int Menu(linkedList states) {

    int choice = 0;
    int population = 0;
    char stateName[MAX] = { 0 };
    char cityName[MAX] = { 0 };
    position currentList = NULL;
    treePointer currentTree = NULL;
    treePointer temp = NULL;

    currentList = (position)malloc(sizeof(linkedList));

    if (!currentList) {
        printf("Greska u alokacijij memorije!\n");
        return EXIT_FAILURE;
    }

    currentTree = (treePointer)malloc(sizeof(struct treeNode));

    if (!currentTree) {
        printf("Greska u alokaciji memorije!\n");
        return EXIT_FAILURE;
    }

    temp = (treePointer)malloc(sizeof(struct treeNode));

    if (!temp) {
        printf("Greska u alokaciji memorije!\n");
        return EXIT_FAILURE;
    }

    while (1) {
        printf("---------------------------------\n");
        printf("1 - pokrenuti program\n");
        printf("2 - izaci iz programa\n");
        printf("---------------------------------\n");
        printf("Izbor:");
        scanf(" %d", &choice);
        if (choice == 1) {
            printf("Unesite ime drzave: ");
            scanf(" %s", stateName);
            currentList = FindState(states, stateName);

            if (!currentList) {
                printf("Podaci za unesenu drzavu ne postoje!\n");
            }

            else {
                printf("Podaci za drzavu %s su pronadeni!\n", currentList->stateName);
                printf("Gradovi drzave su:\n");
                PrintInorder(currentList->nextTree);

                temp = currentList->nextTree;
                printf("\nUnesite broj stanovnika u gradu: ");
                scanf(" %d", &population);
                printf("Gradovi sa vise od %d stanovnika u gradu %s su:\n", population, stateName);
                PrintCitiesLargerThan(temp, population);
            }
            printf("\n");
        }
        else if (choice == 2) {
            printf("Uspjesan izlazak iz programa!\n\n\n");
            break;
        }
        else {
            printf("Greska u unosu, moguce znamenke su 1 i 2!\n\n");
        }
    }

    return 0;
}

treePointer InsertInTree(treePointer root, char cityName[], int population) {

    if (root == NULL) {

        root = (treePointer)malloc(sizeof(struct treeNode));
        strcpy(root->cityName, cityName);
        root->population = population;
        root->left = NULL;
        root->right = NULL;
    }

    if (root->population > population)
        root->left = InsertInTree(root->left, cityName, population);

    else if (root->population < population)
        root->right = InsertInTree(root->right, cityName, population);

    else if (strcmp(root->cityName, cityName) > 0)
        root->left = InsertInTree(root->left, cityName, population);

    else if (strcmp(root->cityName, cityName) < 0)
        root->right = InsertInTree(root->right, cityName, population);

    return root;
}

position CreateHeadList() {

    position head = NULL;
    head = (position)malloc(sizeof(linkedList));

    if (!head) {
        printf("Greska u alokaciji memorije!\n");
        return NULL;
    }

    strcpy(head->stateName, "");
    head->next = NULL;
    head->nextTree = NULL;
    head->next = NULL;

    return head;
}

int PrintList(linkedList states) {

    position current = states.next;

    while (current->next != NULL) {
        current = current->next;
        printf("State: %s \nCities:\n", current->stateName);
        PrintInorder(current->nextTree);
        printf("\n");
    }

    return 0;
}

int PrintCitiesLargerThan(treePointer current, int population) {

    if (current == NULL) {
        return 0;
    }

    PrintCitiesLargerThan(current->left, population);

    if (current->population >= population)
        printf("    %s (%d population)\n", current->cityName, current->population);

    PrintCitiesLargerThan(current->right, population);
    return 0;
}
