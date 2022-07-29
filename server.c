#include "server.h"
#include <string.h>
ST_accountsDB_t accountsDB[255] = { {10000, "ABCDEFGH123456789"}, { 7450.8, "ABCDEFGH123456781" },
	{ 12500.5, "ABCDEFGI123456789" }, { 9500, "ABCDEFGI123356781" },
	{ 3824, "ABCDEFGJ123456789" }, { 4862, "ABCDEFGJ123456781" },
	{ 12375, "ABCDEFGK123456789" }, { 500, "ABCDEFGK123456781" },
	{ 132, "ABCDEFGL123456789" }, { 423, "ABCDEFGL123456781" } };

ST_transaction_t trasactionsDB[255] = { {{0,0,0},{0,0,0},0,0}, {{0,0,0},{0,0,0},0,0},
{ {0,0,0},{0,0,0},0,0}, {{0,0,0},{0,0,0},0,0},
{{0,0,0},{0,0,0},0,0}, {{0,0,0},{0,0,0},0,0},
{{0,0,0},{0,0,0},0,0}, {{0,0,0},{0,0,0},0,0},
{{0,0,0},{0,0,0},0,0}, {{0,0,0},{0,0,0},0,0} };

EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
	for (int i = 0; i < 10; i++) {  // 10 elements in accountsDB
		//cardInProgress = *cardData;
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber)) { // doesn't exist
			continue;
		}
		else {
			indxOfAccount = i;
			return SERVER_OK;
		}
	}
	printf("Account not found!\n");
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
	if (termData->transAmount > accountsDB[indxOfAccount].balance) {
		printf("Not Enough Balance!\n");
		return LOW_BALANCE;
	}
	else {
		return SERVER_OK;
	}
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	// Set sequence number
	static uint32_t seqNo = 0;
	seqNo++;
	transData->transactionSequenceNumber = seqNo;
	// Save transaction in transactionsDB
	static uint8_t transIndx = 0;
	if (transIndx < 255) {
		trasactionsDB[transIndx] = *transData;
		transIndx++;
		return SERVER_OK;
	}
	else {
		return SAVING_FAILED;
	}
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	EN_serverError_t transSaveState;
	if (isValidAccount(&transData->cardHolderData)) {
		transData->transState = DECLINED_STOLEN_CARD;
		transSaveState = saveTransaction(transData);
		if (transSaveState == SAVING_FAILED) {
			return INTERNAL_SERVER_ERROR;
		}
		else {
			return DECLINED_STOLEN_CARD;
		}
	} else if (isAmountAvailable(&transData->terminalData)) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		transSaveState = saveTransaction(transData);
		if (transSaveState == SAVING_FAILED) {
			return INTERNAL_SERVER_ERROR;
		}
		else {
			return DECLINED_INSUFFECIENT_FUND;
		}
	}
	else {
		transData->transState = APPROVED;
		float oldBalance = accountsDB[indxOfAccount].balance;
		accountsDB[indxOfAccount].balance -= transData->terminalData.transAmount;
		float newBalance = accountsDB[indxOfAccount].balance;
		printf("Old Balance = %f\nNew Balance = %f\n", oldBalance, newBalance);
		transSaveState = saveTransaction(transData);
		if (transSaveState == SAVING_FAILED) {
			return INTERNAL_SERVER_ERROR;
		}
		else {
			return APPROVED;
		}
	}
}