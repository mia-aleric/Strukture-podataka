#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS (0)
#define EXIT_FAILED (-1)

#define MAX_LENGTH (50)
#define MAX_DATE_LENGTH (11)
#define MAX_LINE (1024)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)

#define DATES_EQUAL (0)
#define DATE2_LATER (1)
#define DATE2_EARLIER (-1)


typedef struct _article* articlePosition;
typedef struct _article {
	char name[MAX_LENGTH];
	int quantity;
	int price;
	articlePosition next;
}article;

typedef struct _receipt* receiptPosition;
typedef struct _receipt {
	char receiptDate[MAX_DATE_LENGTH];
	article articleHead;
	receiptPosition next;

}receipt;


int readAndCreateReceiptList(receiptPosition receiptHead);
receiptPosition createAndFillReceiptNode(char* receiptName);
articlePosition createAndFillArticleNode(char* articleName, int articleQuantity, int articlePrice);
int articleSortedInput(articlePosition articleHead, articlePosition newArticleNode);
int receiptSortedInput(receiptPosition receiptHead, receiptPosition newReceiptNode);
int compareDates(char* date1, char* date2);
int mergeReceipts(receiptPosition newReceiptNode, receiptPosition currentReceiptInList);
int findAndPrintPriceAndQuantityOfArticleInTimeframe(receiptPosition receiptHead, char* wantedArticleName, char* dateFrom, char* dateTo);
int printPriceAndQuantityOfArticleInTimeFrame(receiptPosition startReceipt, receiptPosition endReceipt, char* wantedArticleName);


int main() {
	receipt receiptHead = { {0},{{0}, 0 , 0, NULL}, NULL };
	char dateFrom[MAX_LENGTH] = { '\0' };
	char dateTo[MAX_LENGTH] = { '\0' };
	char articleName[MAX_LENGTH] = { '\0' };

	readAndCreateReceiptList(&receiptHead);

	printf("\nZa saznati koliko je sveukupno novca potroseno i na koliku kolicina odredenog artikla u nekom vremenskom razdoblju upisite ime tog artikla i trazeno vremensko razdoblje:\nIME: ");
	scanf("%s", articleName);
	printf("Noviji datum: YYYY-MM-DD: ");
	scanf("%s", dateFrom);
	printf("Stariji datum: YYYY-MM-DD: ");
	scanf("%s", dateTo);

	findAndPrintPriceAndQuantityOfArticleInTimeframe(&receiptHead, articleName, dateFrom, dateTo);

	return EXIT_SUCCESS;
}


int readAndCreateReceiptList(receiptPosition receiptHead) {
	FILE* fp = NULL;
	char receiptName[MAX_LENGTH] = { '\0' };
	receiptPosition newReceiptNode = NULL;

	fp = fopen("racuni.txt", "r");

	if (!fp) {
		printf("\nCan't open file!\n");
		return FILE_NOT_OPEN;
	}

	while (!feof(fp)) {
		if (1 != fscanf(fp, " %s", receiptName)) {
			continue;
		}
		newReceiptNode = createAndFillReceiptNode(receiptName);
		receiptSortedInput(receiptHead, newReceiptNode);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}


receiptPosition createAndFillReceiptNode(char* receiptName) {
	
	char receiptDate[MAX_LENGTH] = { 0 }, buffer[MAX_LINE] = { 0 }, articleName[MAX_LENGTH];
	int articleQuantity = 0, articlePrice = 0;
	receiptPosition newReceiptNode = NULL;
	articlePosition newArticleNode = NULL;

	FILE* filename = NULL;
	filename = fopen("racuni.txt", "r");

	if (!filename)
	{
		printf("Cannot open file %s\n", receiptName);
		return NULL;
	}

	fscanf(filename, " %s ", receiptDate);

	newReceiptNode = (receiptPosition)malloc(sizeof(receipt));

	strcpy(newReceiptNode->receiptDate, receiptDate);
	newReceiptNode->next = NULL;
	newReceiptNode->articleHead.next = NULL;

	while (!feof(filename)) {
		fscanf(filename, " %s %d %d ", articleName, &articleQuantity, &articlePrice);
		newArticleNode = createAndFillArticleNode(articleName, articleQuantity, articlePrice);
		articleSortedInput(&newReceiptNode->articleHead, newArticleNode);
	}

	fclose(filename);

	return newReceiptNode;
}


articlePosition createAndFillArticleNode(char* articleName, int articleQuantity, int articlePrice) {
	articlePosition newArticleNode = NULL;

	newArticleNode = (articlePosition)malloc(sizeof(article));

	strcpy(newArticleNode->name, articleName);
	newArticleNode->quantity = articleQuantity;
	newArticleNode->price = articlePrice;
	newArticleNode->next = NULL;

	return newArticleNode;
}


int articleSortedInput(articlePosition articleHead, articlePosition newArticleNode) {
	articlePosition current = articleHead;

	while (current->next != NULL && strcmp(newArticleNode->name, current->next->name) > 0)
		current = current->next;

	if (current->next != NULL && strcmp(newArticleNode->name, current->next->name) == 0) {
		current->next->quantity += newArticleNode->quantity;
		current->next->price += newArticleNode->price;
	}

	else {
		newArticleNode->next = current->next;
		current->next = newArticleNode;
	}

	return EXIT_SUCCESS;
}


int receiptSortedInput(receiptPosition receiptHead, receiptPosition newReceiptNode) {
	char receiptDate[MAX_LENGTH] = { '\0' };
	receiptPosition current = receiptHead;

	while (current->next != NULL && compareDates(newReceiptNode->receiptDate, current->next->receiptDate) == 1)
		current = current->next;


	if (current->next != NULL && compareDates(newReceiptNode->receiptDate, current->next->receiptDate) == 0) {
		mergeReceipts(newReceiptNode, current->next);
		free(newReceiptNode);
	}

	else {
		newReceiptNode->next = current->next;
		current->next = newReceiptNode;
	}

	return EXIT_SUCCESS;
}


int compareDates(char* date1, char* date2) 
{
	int year1 = 0, year2 = 0, month1 = 0, month2 = 0, day1 = 0, day2 = 0;

	sscanf(date1, " %d-%d-%d", &year1, &month1, &day1);
	sscanf(date2, " %d-%d-%d", &year2, &month2, &day2);

	if (year1 < year2)
		return DATE2_LATER;

	else if (year1 > year2)
		return DATE2_EARLIER;

	if (month1 < month2) 
		return DATE2_LATER;

	else if (month1 > month2)
		return DATE2_EARLIER;

	if (day1 < day2)
		return DATE2_LATER;

	else if (day1 > day2)
		return DATE2_EARLIER;

	return DATES_EQUAL;

}




int mergeReceipts(receiptPosition newReceiptNode, receiptPosition currentReceiptInList) {
	articlePosition current = &newReceiptNode->articleHead, temp = NULL;

	while (current->next != NULL) {
		temp = createAndFillArticleNode(current->next->name, current->next->quantity, current->next->price);

		articleSortedInput(&currentReceiptInList->articleHead, temp);

		current = current->next;
	}

	return EXIT_SUCCESS;
}


int findAndPrintPriceAndQuantityOfArticleInTimeframe(receiptPosition receiptHead, char* wantedArticleName, char* dateFrom, char* dateTo) {
	receiptPosition current = receiptHead, startReceipt = NULL, endReceipt = NULL;

	while (current->next != NULL) {

		if (compareDates(dateFrom, current->next->receiptDate) != 1) {
			startReceipt = current->next;
			break;
		}
		current = current->next;
	}

	current = receiptHead;

	while (current->next != NULL)
	{
		if (compareDates(dateTo, current->next->receiptDate) == 1)
			endReceipt = current->next;

		current = current->next;
	}


	if (startReceipt == NULL || endReceipt == NULL)
		printf("\nNema racuna u trazenom vremenskom razdoblju!\n");

	else
		printPriceAndQuantityOfArticleInTimeFrame(startReceipt, endReceipt, wantedArticleName);

	return EXIT_SUCCESS;
}


int printPriceAndQuantityOfArticleInTimeFrame(receiptPosition startReceipt, receiptPosition endReceipt, char* wantedArticleName)
{
	receiptPosition receiptCurrent = startReceipt;
	articlePosition articleCurrent = NULL;
	int totalQuantity = 0, totalPrice = 0;

	if (startReceipt == endReceipt)
	{
		articleCurrent = &receiptCurrent->articleHead;

		while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) != 0)
			articleCurrent = articleCurrent->next;

		if (articleCurrent->next != NULL)
			printf("\nUkupna kolicina artikla je bila %d komada i kostala je %d eura!\n", articleCurrent->next->quantity, articleCurrent->next->price);

		else
			printf("\nArtikl nije kupljen u trazenom vremenskom razdoblju!\n");
	}

	else
	{
		while (receiptCurrent != endReceipt->next)
		{
			articleCurrent = &receiptCurrent->articleHead;

			while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) != 0)
				articleCurrent = articleCurrent->next;


			if (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) == 0)
			{
				totalQuantity += articleCurrent->next->quantity;
				totalPrice += articleCurrent->next->price;
			}

			receiptCurrent = receiptCurrent->next;
		}

		if (totalQuantity != 0)
			printf("\nUkupna kolicina artikla je bila %d komada i kostala je %d eura.\n", totalQuantity, totalPrice);

		else
			printf("\nArtikl nije kupljen u trazenom vremenskom razdoblju.\n");
	}

	return EXIT_SUCCESS;
}