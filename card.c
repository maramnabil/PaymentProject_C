#include <stdio.h>
#include <string.h>
#include "card.h"
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	printf("Please Enter Card Holder Name (20-24 characters):\n");
	gets(cardData->cardHolderName);
	int nameLength = strlen(cardData->cardHolderName);
	if (nameLength == 0) {
		printf("ERROR: Please enter a valid name!\n");
		return WRONG_NAME;
	}else if (nameLength < 20) {
		printf("ERROR: Name must be MIN. 20 characters.\nYou entered %d characters.\n", nameLength);
		return WRONG_NAME;
	}
	else if (nameLength > 24) {
		printf("ERROR: Name must be MAX. 24 characters.\nYou entered %d characters.\n", nameLength);
		return WRONG_NAME;
	}
	else {
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	printf("Enter card expiry date in the format MM/YY: ");
	gets(cardData->cardExpirationDate);
	int dateLength = strlen(cardData->cardExpirationDate);

	if (dateLength == 5 &&
		(cardData->cardExpirationDate[0] == '0' || cardData->cardExpirationDate[0] == '1')
		&& (cardData->cardExpirationDate[1] - '0' >= 0 && cardData->cardExpirationDate[1] - '0' <= 9)
		&& cardData->cardExpirationDate[2] == '/'
		&& (cardData->cardExpirationDate[3] - '0' >= 0 && cardData->cardExpirationDate[3] - '0' <= 9)
		&& (cardData->cardExpirationDate[4] - '0' >= 0 && cardData->cardExpirationDate[4] - '0' <= 9)) {
		return CARD_OK;
	}
	else {
		printf("ERROR: Please enter a valid date in the required format.\n");
		return WRONG_EXP_DATE;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("Enter card Primary Account Number (PAN): ");
	gets(cardData->primaryAccountNumber);
	int panLength = strlen(cardData->primaryAccountNumber);
	if (panLength == 0 || panLength < 16 || panLength > 19) {
		printf("ERROR: Invalid PAN!\n");
		return WRONG_PAN;
	}
	else {
		return CARD_OK;
	}
}