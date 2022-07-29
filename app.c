#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "app.h"
void appStart(void) {
	//--------CARD MODULE----------
	ST_cardData_t cardData;
	EN_cardError_t nameError;
	EN_cardError_t dateError;
	EN_cardError_t panError;
	do {
		nameError = getCardHolderName(&cardData);
	} while (nameError == WRONG_NAME);
	do {
		dateError = getCardExpiryDate(&cardData);
	} while (dateError == WRONG_EXP_DATE);
	do {
		panError = getCardPAN(&cardData);
	} while (panError == WRONG_PAN);
	//---------TERMINAL MODULE--------
	EN_terminalError_t tDateError;
	EN_terminalError_t tAmountError;
	ST_terminalData_t terminalData;
	do {
		tDateError = getTransactionDate(&terminalData);
	} while (tDateError == WRONG_DATE);
	tDateError = isCardExpired(cardData, terminalData);
	if (tDateError == EXPIRED_CARD) {
		return;
	}
	do {
		tAmountError = getTransactionAmount(&terminalData);
	} while (tAmountError == INVALID_AMOUNT);
	/*tAmountError = setMaxAmount(&terminalData);
	while (tAmountError == INVALID_MAX_AMOUNT) {
		printf("Enter maximum transaction amount: ");
		scanf("%f", &terminalData.maxTransAmount);
		tAmountError = setMaxAmount(&terminalData);
	}*/
	do {
		tAmountError = setMaxAmount(&terminalData);
	} while (tAmountError == INVALID_MAX_AMOUNT);
	tAmountError = isBelowMaxAmount(&terminalData);
	if (tAmountError == EXCEED_MAX_AMOUNT) {
		return;
	}
	//------------SERVER MODULE-----------
	EN_serverError_t serverError;
	serverError = isValidAccount(&cardData);
	if (serverError == ACCOUNT_NOT_FOUND) {
		return;
	}
	serverError = isAmountAvailable(&terminalData);
	if (serverError == LOW_BALANCE) {
		return;
	}
	// Saving transaction data
	EN_transState_t transactionState;
	ST_transaction_t transaction;
	transaction.cardHolderData = cardData;
	transaction.terminalData = terminalData;

	transactionState = recieveTransactionData(&transaction);
	if (transactionState == INTERNAL_SERVER_ERROR) {
		printf("Internal Server Error! Transaction Not saved.\n");
	}
	else if (transactionState == DECLINED_STOLEN_CARD) {
		printf("Declined! Stolen card!\n");
	}
	else if (transactionState == DECLINED_INSUFFECIENT_FUND) {
		printf("Declined! Insufficient Fund!\n");
	}
	else {
		printf("Transaction approved and saved.\n");
	}
}

int main() {
	appStart();
}