/*
 ============================================================================
 Name        : HW3.c
 Author      : Jesse Bannon
 Version     : 1.1
 Copyright   : TCSS 333
 Description : Creates summary of top 100 female names from 1920 - 2010.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAMES 500
#define NAME_LENGTH 40


/*
 * Writes 'summary.csv' which contains data of female names and rankings.
 */
void writeSummary(char *nameArray, int *rankArray) {
    FILE *outfile = fopen("summary.csv", "w");
    fwrite("Name,1920,1930,1940,1950,1960,1970,1980,1990,2000,2010\n",  // Writes top line
            sizeof(char), 56, outfile);                                 // of file

    int i,j;
    i = 0;
    while (*(nameArray + i*NAME_LENGTH) != '\0') {
        fprintf(outfile, "%s", &nameArray[i*NAME_LENGTH]);

        for (j = 0; j < 10; j++) {                  // Writes in format of
            fwrite(",", sizeof(char), 1, outfile);  // <name>,<1920 rank>, ... ,<2010 rank> \n
            if(rankArray[i*10 + j] != 0)
                fprintf(outfile, "%d", rankArray[i*10 + j]);

        }
        fwrite("\n", sizeof(char), 1, outfile);
        ++i;
    }
    fclose(outfile);
}


/*
 * Swaps names held within a character array.
 */
void nameSwap(char *nameOne, char *nameTwo) {
    char temp[NAME_LENGTH];
    strcpy(temp, nameOne);
    strcpy(nameOne, nameTwo);
    strcpy(nameTwo, temp);
}


/*
 * Swaps name ranks (ten) held within an integer array.
 */
void rankSwap(int *rankBaseOne, int *rankBaseTwo) {
    int temp,i;
    for (i = 0; i < 10; i++) {
        temp = *(rankBaseOne + i);
        *(rankBaseOne + i) = *(rankBaseTwo + i);
        *(rankBaseTwo + i) = temp;
    }
}


/*
 * Sorts names and ranks in alphabetical order of the names.
 */
void sortNames(char *nameArray, int *rankArray) {
    int i, j;
    i = 0;

    while (*(nameArray + i*NAME_LENGTH) != '\0') {          // Sorts names using bubble sort.
        j = 0;                                              // Compares to strings to determine
                                                            // which name is greater.
        while (*(nameArray + (j+1)*NAME_LENGTH) != '\0') {
            if (strcmp(&nameArray[j*NAME_LENGTH], &nameArray[(j+1)*NAME_LENGTH]) > 0) {
                nameSwap(&nameArray[j*NAME_LENGTH], &nameArray[(j+1)*NAME_LENGTH]);
                rankSwap(&rankArray[j*10], &rankArray[(j+1)*10]);
            }
            ++j;
        }
        ++i;
    }
}


/*
 * Inputs names read from the file into the array. Recognizes duplicates.
 */
void inputName(char *name, char *nameArray, int *rankArray, const int callNum, const int rank) {
    int i = 0;
    while (i < MAX_NAMES) {                     // If a name exists in the index, check if it
        if (nameArray[i*NAME_LENGTH] != '\0') { // equals to avoid duplicates.

            if (strcmp(name, &nameArray[i*NAME_LENGTH]) == 0) {
                rankArray[i*10 + callNum] = rank;
                return;
            } else
                i++;

        } else {
            strcpy(&nameArray[i*NAME_LENGTH], name);    // If a name does not exist in the index,
            rankArray[i*10 + callNum] = rank;           // copy it to top of array and exit the
            return;                                     // function.
        }
    }
}


/*
 * Reads in first 100 names from yob files and inputs them into array.
 */
void readYobName(const char *filename, char *nameArray, int *rankArray, const int callNum) {
    FILE *infile = fopen(filename, "r");
    char line[NAME_LENGTH * 2];
    int i,j;

    for (i = 0; i < 100; i++) {
        char tempName[NAME_LENGTH] = {'\0'};
        j = 0;

        fgets(line, NAME_LENGTH*2, infile);     // Reads the entire line by the file
        while (line[j] != ',') {                // and takes in all characters before the
            tempName[j] = line[j];              // first comma.
            j++;
        }
        inputName(tempName, nameArray, rankArray, callNum, i+1);    // Inputs name into the array
    }                                                               // Resets tempName to read
                                                                    // in the next name.
    fclose(infile);
}


/*
 * Reads all ten yob files.
 */
void readAllFiles(char *nameArray, int *rankArray) {
    readYobName("yob1920.txt", nameArray, rankArray, 0);
    readYobName("yob1930.txt", nameArray, rankArray, 1);
    readYobName("yob1940.txt", nameArray, rankArray, 2);
    readYobName("yob1950.txt", nameArray, rankArray, 3);
    readYobName("yob1960.txt", nameArray, rankArray, 4);
    readYobName("yob1970.txt", nameArray, rankArray, 5);
    readYobName("yob1980.txt", nameArray, rankArray, 6);
    readYobName("yob1990.txt", nameArray, rankArray, 7);
    readYobName("yob2000.txt", nameArray, rankArray, 8);
    readYobName("yob2010.txt", nameArray, rankArray, 9);
}


/*
 * Reads in yob files, sorts names and ranks alphabetically,
 * then writes them to 'summary.csv'.
 */
int main(void) {
    char names[MAX_NAMES * NAME_LENGTH];
    int nameRanks[MAX_NAMES * 10] = {0};

    printf("* Initialized memory for names and ranks.\n");
    printf("* Attempting to read in names from files...\n");

    readAllFiles(names, nameRanks);
    printf("* Names have been read! Sorting names...\n");

    sortNames(names, nameRanks);
    printf("* Sorted! Attempting to write summary of data...\n");

    writeSummary(names, nameRanks);
    printf("* Data has been written! Can be found in 'summary.csv'.");

    return 0;
}
