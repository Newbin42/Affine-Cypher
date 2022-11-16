#pragma warning(disable:4996) //Disable deprecation warning
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef __TRUE_FALSE__
#define __TRUE_FALSE__
#define TRUE 1
#define FALSE 0
#endif

//File definitions
#define BUFFER_SIZE 10000
#define ASCII_UPPER_LOWER_OFFSET 32

//Encryption Definitions
#define NO_INVERSE -1
#define ERROR -2

#define ALPHABET_SIZE 26

#define ASCII_OFFSET 97
#define BUFFER_SIZE 1000

//Prototypes
int TryToOpen(char*);
int ReadStringFromFile(char*, char*);
int WriteStringToFile(char*, char*);
int CleanString(char*);

int EncryptString(char*, int, int, int);
int DecryptString(char*, int, int, int);
int ModularInverse(int, int);
int IsPrime(int);
int CoprimeTest(int, int);

int main() {
	//Declare variables
	char buffer[BUFFER_SIZE] = "";
	int success = FALSE;
	int inMenu = TRUE;
	int badInput = 0;
	int selection = 0;
	char phantomChar = ' ';

	//Default cipher variables
	char inputFile[100] = "input.txt";
	char outputFile[100] = "output.txt";
	int cipherKey_A = 19;
	int cipherKey_B = 4;
	int modularInverse = 11;

	while (inMenu) {
		//Draw the menu
		do {
			printf("The current input file is : %s\n", inputFile);
			printf("The current output file is : %s\n", outputFile);
			printf("Affine Cipher - e(x) = (%dx + %d)(mod %d)\n", cipherKey_A, cipherKey_B, ALPHABET_SIZE);
			printf("A = %d A^(-1) = %d\n", cipherKey_A, modularInverse);
			printf("B = %d\n", cipherKey_B);

			for (int i = 0; i < 25; i++) { printf("-"); }
			printf("\n");

			printf("1.) Select Input File\n");
			printf("2.) Select Output File\n");
			printf("3.) Select Affine Cypher Key\n");
			printf("4.) Encrypt Input File\n");
			printf("5.) Decrypt Input File\n");
			printf("6.) Get Directory Listing\n");
			printf("7.) Quit\n");
			printf(">>>");

			scanf("%d%c", &selection, &phantomChar);

			if (selection < 0 || selection > 7) {
				printf("%d is an invalid selection, please try again.\n Press any key to continue... ", selection);
				_getch();
				system("cls"); //Clear console
			}
		} while (badInput);

		switch (selection) {
			case 1: //Select input
				do {
					printf("Please enter the new filename for the input file (example_file.txt)...\n>>> ");
					scanf("%s", inputFile);

					success = TryToOpen(inputFile);
					if (success != TRUE) { //Failed to open the file
						printf("\nUnable to open file %s, most likely it doesn't exist\n", inputFile);
						printf("Press any key to continue... ");
						_getch();
					}
				} while (success != TRUE);

				printf("\nCommand executed... Press any key to continue... ");
				_getch();
				break;

			case 2: //Select output
				// There is no need to perform a read check since a new file
				// will be created for writing if one doesn't already exist.
				printf("Please enter the new filename for the output file (example_file.txt)...\n>>> ");
				scanf("%s", outputFile);

				printf("\nCommand executed... Press any key to continue... ");
				_getch();
				break;

			case 3: //Select cipher keys
				printf("Please enter the new key(s).");

				do { //Ask for key A at least once
					printf("Key A: ");
					scanf("%d%c", &cipherKey_A, &phantomChar);

					modularInverse = ModularInverse(cipherKey_A, ALPHABET_SIZE);
					if (modularInverse == NO_INVERSE || modularInverse == ERROR) {
						printf("%d is an invalid key for A with a mod of %d, please try again.\n Press any key to continue... ", cipherKey_A, ALPHABET_SIZE);
						_getch();
					}

				} while (modularInverse == NO_INVERSE); //Move to B if A is valid

				printf("Key B: ");
				scanf("%d%c", &cipherKey_B, &phantomChar);

				printf("\nCommand executed... Press any key to continue... ");
				_getch();
				break;

			case 4: //Encrypt input file
				success = ReadStringFromFile(inputFile, buffer);
				if (success) { //Successfully opened and read
					printf("Plain Text: %s\n", buffer);

					success = CleanString(buffer);
					if (success) { //Successfully cleaned
						printf("Sanitized Plain Text: %s\n", buffer);

						success = EncryptString(buffer, cipherKey_A, cipherKey_B, ALPHABET_SIZE);
						if (success) { //Successfully encrypted
							printf("Cipher Text: %s\n", buffer);

							success = WriteStringToFile(outputFile, buffer);
							if (success) { //Successfully wrote to file
								printf("\n Command executed... Press any key to continue... ");
								_getch();
							}
							else { //Unsuccessfully wrote to file
								printf("Unable to write to file %s.\n", outputFile);
								printf("Press any key to continue... ");
								_getch();
							}
						}
						else {//Error handled in EncryptString()
							printf("Press any key to continue... ");
							_getch();
						}
					}
					else { //Unsuccessfully cleaned
						printf("Unable to clean string, most likely there is an invalid character.\n");
						printf("Press any key to continue... ");
						_getch();
					}
				}
				else { //Unsuccessfully opened and read
					printf("Unable to read from file %s.\n", inputFile);
					printf("Press any key to continue... ");
					_getch();
				}
				break;
			
			case 5: //Decrypt input file
				success = ReadStringFromFile(inputFile, buffer);
				if (success) { //Successfully opened and read
					printf("Cipher Text: %s\n", buffer);

					success = DecryptString(buffer, cipherKey_A, cipherKey_B, ALPHABET_SIZE);
					if (success) { //Successfully decrypted
						printf("Deciphered Plain Text: %s\n", buffer);

						success = WriteStringToFile(outputFile, buffer);
						if (success) { //Successfully wrote to file
							printf("\n Command executed... Press any key to continue... ");
							_getch();
						}
						else { //Unsuccessfully wrote to file
							printf("Unable to write to file %s.\n", outputFile);
							printf("Press any key to continue... ");
							_getch();
						}
					}
					else { //Error handled in DecryptString
						printf("Press any key to continue... ");
						_getch();
					}
				}
				else { //Unsuccessfully opened and read
					printf("Unable to read from file %s.\n", inputFile);
					printf("Press any key to continue... ");
					_getch();
				}
				break;

			case 6:
				system("dir /p");

				printf("Command executed... Press any key to continue... ");
				_getch();
				break;

			case 7:
				printf("Thank you for using the Affine Cipher, have a good day.\n");
				printf("Press any key to exit... ");
				_getch();

				inMenu = FALSE;
				break;
		}

		system("cls"); //Refresh screen
	}

	return 0;
}

int TryToOpen(char* filename) {
	FILE* fileHandle = NULL;
	fileHandle = fopen(filename, "r");
	if (fileHandle == NULL) {
		return FALSE;
	}
	else {
		fclose(fileHandle);
		return TRUE;
	}
}

int ReadStringFromFile(char* filename, char* string) {
	int success = FALSE; //Initialize success variable and assume reading didn't work by default
	int charCounter = 0;
	char charHolder = ' ';

	FILE* inFile = NULL;
	inFile = fopen(filename, "r");

	if (inFile != NULL) {
		success = TRUE;

		//Read file by character
		while ((charHolder = fgetc(inFile)) != EOF) {
			string[charCounter] = charHolder;
			charCounter++;
		}

		fclose(inFile);
	}

	return success;
}

int WriteStringToFile(char* filename, char* string) {
	int success = FALSE;
	FILE* outFile = NULL;
	outFile = fopen(filename, "w");

	if (outFile != NULL) {
		fprintf(outFile, "%s", string);
		fclose(outFile);
		success = TRUE;
	}

	return success;
}

int CleanString(char* string) {
	int success = TRUE; //Assume successful by default
	char cleanBuffer[BUFFER_SIZE] = "";
	char* originalIterator = string;
	char* cleanIterator = cleanBuffer;

	while (*originalIterator != '\0') {
		if (*originalIterator >= 'a' && *originalIterator <= 'z') { //If lowercase, copy into secondary buffer
			*cleanIterator = *originalIterator;
			cleanIterator++;
		}
		else if (*originalIterator >= 'A' && *originalIterator <= 'Z') { //If uppercase, make it lowercase and copy into secondary buffer
			*cleanIterator = *originalIterator + 32;
			cleanIterator++;
		}

		//Next character
		originalIterator++;
	}

	//Terminate clean buffer pointer
	*cleanIterator = '\0';

	//Copy cleanBuffer into string
	strcpy(string, cleanBuffer);

	//Analyze cleaned string for invalid characters
	int i = 0;
	while (string[i] != '\0') {
		if (string[i] < 'a' || string[i] > 'z') {//Error out if an invalid character is found.
			success = FALSE;
			break;
		}
		i++;
	}

	return success;
}

int EncryptString(char* encryptMe, int a, int b, int n) {
	int i = 0;
	int PTCodes[BUFFER_SIZE] = {};

	//Check to make sure we are safe for encryption
	int inverseCode = ModularInverse(a, n);
	if (inverseCode == NO_INVERSE || inverseCode == ERROR) {
		printf("You cannot encrypt with key (%d, %d) on a (mod %d) ring.\n", a, b, n);
		return FALSE;
	}

	//Copy the ASCII array codes into the PTCodes array
	i = 0;
	while (encryptMe[i] != '\0') {
		PTCodes[i] = encryptMe[i];
		i++;
	}
	PTCodes[i] = -1;

	//Reduce character ASCII codes to PT codes
	i = 0;
	while (PTCodes[i] != -1) {
		PTCodes[i] -= ASCII_OFFSET;
		i++;
	}

	//Encrypt PT codes
	i = 0;
	while (PTCodes[i] != -1) {
		PTCodes[i] = (PTCodes[i] * a + b) % n;
		i++;
	}

	//Revert PT codes to ASCII codes
	i = 0;
	while (PTCodes[i] != -1) {
		PTCodes[i] += ASCII_OFFSET;
		i++;
	}

	//Copy the PT codes array back into the ASCII codes array
	i = 0;
	while (PTCodes[i] != -1) {
		encryptMe[i] = PTCodes[i];
		i++;
	}
	encryptMe[i] = '\0';

	//Successfully encrypted
	return TRUE;
}

int DecryptString(char* decryptMe, int a, int b, int n) {
	int PTCodes[BUFFER_SIZE] = {};

	//Check to make sure we are safe for decryption (User may open wrong file)
	int inverseCode = ModularInverse(a, n);
	if (inverseCode == NO_INVERSE || inverseCode == ERROR) {
		printf("I'm sorry, but %s cannot be decrypted.\n", decryptMe);
		return FALSE;
	}

	//Copy the ASCII array codes into the PTCodes array
	int i = 0;
	while (decryptMe[i] != '\0') {
		PTCodes[i] = decryptMe[i];
		i++;
	}
	PTCodes[i] = -1;

	//Reduce character ASCII codes to PT codes
	i = 0;
	while (PTCodes[i] != -1) {
		PTCodes[i] -= ASCII_OFFSET;
		i++;
	}

	//Decrypt PT codes using inverse
	i = 0;
	while (PTCodes[i] != -1) {
		PTCodes[i] = inverseCode * (PTCodes[i] - b) % n;
		//Ensure positive integer.
		if (PTCodes[i] < 0) { PTCodes[i] += ALPHABET_SIZE; }
		i++;
	}

	//Revert PT codes to ASCII codes
	i = 0;
	while (PTCodes[i] != -1) {
		PTCodes[i] += ASCII_OFFSET;
		i++;
	}

	//Copy the PT codes array back into the ASCII codes array
	i = 0;
	while (PTCodes[i] != -1) {
		decryptMe[i] = PTCodes[i];
		i++;
	}
	decryptMe[i] = '\0';

	//Successfully encrypted
	return TRUE;
}

int ModularInverse(int target, int mod) {
	int inverse = 1;

	// determine if the target has an inverse in this
	// particular modular system
	if (CoprimeTest(target, mod) == FALSE) { return NO_INVERSE; }

	//Search for inverse of target
	for (inverse = 1; inverse <= mod; inverse++) {
		if ((target * inverse) % mod == 1) { return inverse; }
	}

	//No inverse found
	return ERROR;

	return 0;
}

int IsPrime(int amIPrime) {
	int i = 0;
	int isPrime = TRUE;

	//Check for less than or equal to 1
	if (amIPrime <= 1) { isPrime = FALSE; }

	//Force check for prime number
	for (i = 2; i < amIPrime; i++) {
		if (amIPrime % i == 0) { isPrime = FALSE; }
	}

	return isPrime;
}

int CoprimeTest(int numOne, int numTwo) {
	int i = 0;
	int areCoprime = TRUE;
	int larger = 0;
	int smaller = 0;

	// determine that larger of the two numbers
	if (numOne >= numTwo) {
		larger = numOne;
		smaller = numTwo;
	}
	else {
		larger = numTwo;
		smaller = numOne;
	}

	for (i = 1; i <= larger; i++) {
		//Find a prime factor for number one
		if (larger % i == 0 && IsPrime(i)) {
			//Is number two divivsible by number one
			if (smaller % i == 0) { areCoprime = FALSE; }
		}
	}

	return areCoprime;
}