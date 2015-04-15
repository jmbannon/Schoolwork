/*
 ============================================================================
 Name        : HW1.c
 Author      : Jesse Bannon
 Version     :
 Copyright   : TCSS 333
 Description : Converts hex, alphabetic, and binary values amongst each other.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define INPUT_SIZE 256

/*
 * Returns char that determines the user's input type.
 * H: Hex
 * A: Alphabetic
 * B: Binary
 * Q: Quit
 */
char getInputType() {
    char userSelectedInput = ' ';
    while (userSelectedInput != 'H' && userSelectedInput != 'A'
	        && userSelectedInput != 'B' && userSelectedInput != 'Q') {

        printf("Choose Input (H,A,B,Q): ");
        scanf(" %c", &userSelectedInput);
    }
    return userSelectedInput;
}

/*
 * Returns char that determines the user's output type.
 * H: Hex
 * A: Alphabetic
 * B: Binary
 */
char getOutputType() {
	char userSelectedOutput = ' ';
	while (userSelectedOutput != 'H' && userSelectedOutput != 'A'
	        && userSelectedOutput != 'B') {

        printf("Choose Output (H,A,B): ");
        scanf(" %c", &userSelectedOutput);
	}
	return userSelectedOutput;
}

/*
 * Prints an unsigned 8-bit value of the integer number parameter.
 */
void printNumToBin(const int value) {

	int decimalValue = value;
    int binary = 128;
    int j = 0;

    // Inputs binary values left to right.
	while (j < 8) {
		if (decimalValue % binary == decimalValue)
			printf("0");
		else {
			printf("1");
			decimalValue-=binary;
		}
		++j;
		binary/=2;
	}
}

/*
 * Prints the hex value of the integer number parameter.
 */
void printNumToHex(const int value) {
	char hexValue[2];

	int decimalValue = value;
	int i = 1;
	int rem;

	// Inputs hex values right to left.
	while (i >= 0) {
		rem = decimalValue%16;
		switch(rem) {
			case 10: printf("A"); break;
			case 11: hexValue[i]='B'; break;
			case 12: hexValue[i]='C'; break;
			case 13: hexValue[i]='D'; break;
			case 14: hexValue[i]='E'; break;
			case 15: hexValue[i]='F'; break;
			default: hexValue[i] = rem+'0'; break;
		}
		--i;
		decimalValue /= 16;
	}

	for (i = 0; i < 2; ++i)
		printf("%c", hexValue[i]);
}

/*
 * Converts an unsigned 8-bit value in a char array into
 * decimal value.
 */
int binToNum(const char * binaryValue) {
	int i;
	int sum = 0;
	int value = 128;

	for (i = 0; i < 8; ++i) {
		if (binaryValue[i] == '1')
			sum += value;

		value /= 2;
	}
	return sum;
}

/*
 * Converts a hex number made of an array of size 2 into a
 * decimal number.
 */
int hexToNum(const char * binaryValue) {
    int i;
    int sum = 0;
    for (i = 0; i < 2; ++i) {
        switch (binaryValue[i]) {
        case 'A': sum += 10; break;
        case 'B': sum += 11; break;
        case 'C': sum += 12; break;
        case 'D': sum += 13; break;
        case 'E': sum += 14; break;
        case 'F': sum += 15; break;
        default: sum += binaryValue[i] - '0';
        }
        if (i == 0)
            sum *= 16;
    }
	return sum;
}

int main(void) {

    char userSelectedInput, userSelectedOutput;
    char input[INPUT_SIZE];

    printf("Welcome to the Coder!\n");
    while(1) {
        int inputIndex, inputOffset;

        userSelectedInput = getInputType();
        if (userSelectedInput == 'Q')
            return 0;

            userSelectedOutput = getOutputType();

        switch (userSelectedInput) {
            case 'H': printf("Enter your hex input.\n"); break;
            case 'A': printf("Enter your alpha input.\n"); break;
            case 'B': printf("Enter your bin input.\n"); break;
            default: printf("\n*** Invalid selection"); break;
        }

        inputIndex = 0;
        char temp;
        do {
            scanf("%c", &temp);
            if (temp != '\r' && temp != '\n') {
                input[inputIndex] = temp;
                ++inputIndex;
            }
        } while(temp != '$');

        // If the selected input equals the selected output
        if (userSelectedInput == userSelectedOutput) {
            int i = 0;
            while (input[i] != '$') {
                printf("%c", input[i]);
                ++i;
            }
            printf("\n");
            continue;
        }

        inputOffset = 0;

        // If the user input is hex
        if (userSelectedInput == 'H') {
            char hex[2];
            while (*(input + inputOffset) != '$') {
                if (inputOffset % 2 != 1)
                    hex[0] = input[inputOffset];
                else {
                    hex[1] = input[inputOffset];

                    if (userSelectedOutput == 'A')
                        printf("%c", hexToNum(hex));
                    else
                        printNumToBin(hexToNum(hex));
                }
                ++inputOffset;
            }

	    // If the user input is alphabetic
        } else if (userSelectedInput == 'A') {
            while(*(input + inputOffset) != '$') {
                if (userSelectedOutput == 'H')
                    printNumToHex((int)input[inputOffset]);
                else
                    printNumToBin((int)input[inputOffset]);

                ++inputOffset;
            }

        // If the user input is binary
        } else if (userSelectedInput == 'B') {
            char bin[8];
            while (*(input + inputOffset) != '$') {
                if (inputOffset % 8 != 7)
                    bin[inputOffset % 8] = input[inputOffset];
                else {
                    bin[inputOffset % 8] = input[inputOffset];
                    if (userSelectedOutput == 'H')
                        printNumToHex(binToNum(bin));
                    else
                        printf("%c", binToNum(bin));
                }
                ++inputOffset;
            }

        //If the user input is invalid
        } else
            printf("\n*** ERROR: Invalid Input");

        printf("\n");
    }
    return 0;
}
