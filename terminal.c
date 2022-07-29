#define _CRT_SECURE_NO_WARNINGS
#include "terminal.h"
#include <stdio.h>
//#include <time.h>
#include <string.h>
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	//time_t t = time(NULL);
	//struct tm tm = *localtime(&t);
	//char date[10];
	//strcpy(date, tm.tm_mday);
	////strcat(date, tm.tm_mday, '/', tm.tm_mon, '/', tm.tm_year);
	//for (int i = 0; i < 10; i++) {
	//	printf(date[i]);
	//}
	//printf('\n');
	
	printf("Enter transaction date in the form DD/MM/YYYY: ");
	gets(termData->transactionDate);
	int tdateLength = strlen(termData->transactionDate);

	if (tdateLength == 10 &&
		(termData->transactionDate[0] - '0' >= 0 && termData->transactionDate[0] - '0' <= 3)
		&& (termData->transactionDate[1] - '0' >= 0 && termData->transactionDate[1] - '0' <= 9)
		&& termData->transactionDate[2] == '/'
		&& (termData->transactionDate[3] == '0' || termData->transactionDate[3] == '1')
		&& (termData->transactionDate[4] - '0' >= 0 && termData->transactionDate[4] - '0' <= 9)
		&& termData->transactionDate[5] == '/'
		&& (termData->transactionDate[6] - '0' >= 0 && termData->transactionDate[6] - '0' <=9)
		&& (termData->transactionDate[7] - '0' >= 0 && termData->transactionDate[7] - '0' <= 9)
		&& (termData->transactionDate[8] - '0' >= 0 && termData->transactionDate[8] - '0' <= 9)
		&& (termData->transactionDate[9] - '0' >= 0 && termData->transactionDate[9] - '0' <= 9)) {
		return TERMINAL_OK;
	}
	else {
		printf("ERROR: Please enter a valid date in the required format.\n");
		return WRONG_DATE;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	char* expDate_year;
	char* expDate_month;
	char* transDate_year;
	char* transDate_month;
	char* transDate_day;
	expDate_month = strtok(cardData.cardExpirationDate, "/");
	expDate_year = strtok(NULL, "/");
	transDate_day = strtok(termData.transactionDate, "/");  // Unused variable (just for eliminating the warning)
	transDate_month = strtok(NULL, "/");
	transDate_year = strtok(NULL, "/") + 2; // (+2) to remove the first 2 characters

	//---------Check card expiration date----------
	// Convert Dates to integers
	int expDate_year_int = atoi(expDate_year);
	int expDate_month_int = atoi(expDate_month);
	int transDate_year_int = atoi(transDate_year);
	int transDate_month_int = atoi(transDate_month);

	//// printing values for checking
	//printf("expDate_year_int = %d\n", expDate_year_int);
	//printf("expDate_month_int = %d\n", expDate_month_int);
	//printf("transDate_year_int = %d\n", transDate_year_int);
	//printf("transDate_month_int = %d\n", transDate_month_int);

	// Check for expiration
	if (expDate_year_int < transDate_year_int) {
		printf("Card Expired!\n");
		return EXPIRED_CARD;
	}
	else if (expDate_year_int == transDate_year_int) {
		if (expDate_month_int <= transDate_month_int) {
			printf("Card Expired!\n");
			return EXPIRED_CARD;
		}
		else {
			return TERMINAL_OK;
		}
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Enter transaction amount: ");
	scanf("%f", &termData->transAmount);
	if (termData->transAmount <= 0) {
		printf("Invalid amount!\n");
		return INVALID_AMOUNT;
	}
	else {
		printf("Transaction amount = %f\n", termData->transAmount);
		return TERMINAL_OK;
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	printf("Enter maximum transaction amount: ");
	scanf("%f", &termData->maxTransAmount);
	if (termData->maxTransAmount <= 0) {
		printf("Invalid amount!\n");
		return INVALID_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}

	/*termData->maxTransAmount = MAX_TRANS_AMOUNT;
	if (MAX_TRANS_AMOUNT <= 0) {
		printf("Invalid amount!\n");
		return INVALID_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}*/
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		printf("Exceeded Max. transaction amount!\n");
		return EXCEED_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}