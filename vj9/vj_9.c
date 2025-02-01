#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define NO_MEMORY_ERROR -1
#define MIN 10
#define MAX 90

typedef struct treeNode* treePointer;
struct treeNode
{
    int value;
    treePointer left;
    treePointer right;
};
treePointer InsertElement(int, treePointer);
treePointer AllocateNew(int);
treePointer replace(treePointer);
int InOrder(treePointer, FILE*);
int Addition(treePointer);
int* GenerateRandomArray(int);

int main()
{
    int i, n, m, choice;
    FILE* fp1 = NULL;
    FILE* fp2 = NULL;
    fp1 = fopen("dat1.txt", "w");
    fp2 = fopen("dat2.txt", "w");


    while (1) {
        printf("----------\n");
        printf("Odaberi:\n");
        printf("0 - izlaz\n");
        printf("1 - zadani niz\n");
        printf("2 - niz random brojeva\n");
        printf("----------\n");
        printf("> ");
        scanf("%d", &choice);
        printf("----------\n");
        switch (choice)
        {
        case 1:
            treePointer root1 = NULL;

            int arr[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
            int n = sizeof(arr) / sizeof(arr[0]);
            for (i = 0; i < n; i++) {
                root1 = InsertElement(arr[i], root1);
            }

            printf("Vase stablo glasi: ");
            fprintf(fp1, "Prije funkcije (zad a)\n");
            InOrder(root1, fp1);

            replace(root1);
            printf("\n");

            printf("Nakon zamjene: ");
            fprintf(fp1, "\nNakon funkcije (zad b)\n");
            InOrder(root1, fp1);
            rewind(fp1);
            break;
        case 2:
            treePointer root2 = NULL;

            printf("Koliko random brojeva zelite?\n");
            scanf(" %d", &m);
            int* Arr = GenerateRandomArray(m);
            for (i = 0; i < m; i++) {
                root2 = InsertElement(Arr[i], root2);
            }
            free(Arr);
            printf("Vase stablo glasi: ");
            fprintf(fp2, "Prije funkcije (zad a)\n");
            InOrder(root2, fp2);

            replace(root2);

            printf("\n");

            printf("Nakon zamjene: ");
            fprintf(fp2, "\nNakon funkcije (zad b)\n");
            InOrder(root2, fp2);
            rewind(fp2);
            break;
        case 0:
            return 0;
        }
        printf("\n");
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}
treePointer InsertElement(int element, treePointer root)
{
    if (root == NULL) {
        return AllocateNew(element);
    }
    if (root->value > element) {
        root->right = InsertElement(element, root->right);
    }
    else if (root->value <= element) {
        root->left = InsertElement(element, root->left);
    }
    return root;
}


treePointer AllocateNew(int element)
{
    treePointer newNode = malloc(sizeof(struct treeNode));
    if (newNode == NULL) {
        printf("Nije moguce alocirati memoriju!");
        return NULL;
    }
    newNode->value = element;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}



int InOrder(treePointer root, FILE* fp)
{

    if (root == NULL) {
        return 0;
    }
    InOrder(root->left, fp);
    printf("%d ", root->value);
    fprintf(fp, "%d ", root->value);
    InOrder(root->right, fp);
    return 0;
}
int Addition(treePointer root) {

    if (!root)
        return 0;

    else
        return (root->value + Addition(root->left) + Addition(root->right));

}
treePointer replace(treePointer root) {

    if (!root) {
        return 0;
    }
    root->value = Addition(root->right) + Addition(root->left);

    replace(root->right);
    replace(root->left);


    return 0;

}
int* GenerateRandomArray(int m) {

    int i;
    int* arr;

    arr = (int*)malloc(m * sizeof(int));

    srand(time(NULL));
    for (i = 0; i < m; i++) {

        arr[i] = MIN + (rand() % (MAX - MIN + 1));

        printf("%d \n", arr[i]);
    }
    return arr;

}