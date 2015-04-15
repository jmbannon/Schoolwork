/*
 ============================================================================
 Name        : HW2.c
 Author      : Jesse Bannon
 Version     :
 Copyright   : TCSS333
 Description : Reads in test.bmp and writes to big.bmp (2*height x 2*width)
               and writes to small.bmp (0.5*height x 0.5*width)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {

    FILE *infile = fopen("test.bmp", "rb");
    FILE *outbig = fopen("big.bmp", "wb");
    FILE *outsmall = fopen("small.bmp", "wb");

    // Contents of the test.bmp header in consecutive order
    char firstField[2];     // 2 Bytes
    int imageFileSize = -1; // 4 Bytes
    char secondField[12];   // 12 Bytes
    int imageWidth = -1;    // 4 Bytes
    int imageHeight = -1;   // 4 Bytes
    char thirdField[8];     // 8 Bytes
    int imageSize = -1;     // 4 Bytes
    char fourthField[16];   // 16 Bytes
                            // *** 54 Bytes total

    // Read in header of test.bmp
    fread(firstField, sizeof(char), 2, infile);
    fread(&imageFileSize, sizeof(int), 1, infile);
    fread(secondField, sizeof(char), 12, infile);
    fread(&imageWidth, sizeof(int), 1, infile);
    fread(&imageHeight, sizeof(int), 1, infile);
    fread(thirdField, sizeof(char), 8, infile);
    fread(&imageSize, sizeof(int), 1, infile);
    fread(fourthField, sizeof(char), 16, infile);

    // If the header was not read in correctly
    if(imageFileSize <= 0 || imageWidth <= 0 || imageHeight <=0 || imageSize <= 0) {
        printf("* ERROR: did not read test.bmp correctly\n* closing");
        return -1;
    }

    // Initialize array of binary data
    char imageData[imageHeight][imageWidth*3];
    fread(imageData, sizeof(char), imageSize, infile);

    printf("* test.bmp initialized\n");

    // Initialize bigImage values
    int bigFileSize = (imageSize * 4) + 54;
    int bigWidth = imageWidth * 2;
    int bigHeight = imageHeight * 2;
    int bigSize = imageSize * 4;
    char bigData[bigHeight][bigWidth * 3];

    // Translate test.bmp to bigImage (2*height x 2*width)
    int i,j, heightIterator = 0, widthIterator = 0;
    for (i = 0; i < bigWidth * 3; ++i) {
        for (j = 0; j < bigHeight; ++j) {
            bigData[j][i] = imageData[heightIterator][widthIterator];
            if (j % 2 == 1) ++heightIterator;
        }

        if (i % 2 == 1) ++widthIterator;
        heightIterator = 0;
    }


    // Initialize smallImage values
    int smallFileSize = (imageSize / 4) + 54;
    int smallWidth = imageWidth / 2;
    int smallHeight = imageHeight / 2;
    int smallSize = imageSize / 4;
    char smallData[smallHeight][smallWidth * 3];

    // Translate test.bmp to smallImage (0.5*height x 0.5*width);
    heightIterator = 0, widthIterator = 0;
    for (i = 0; i < smallWidth * 3; ++i) {
        for (j = 0; j < smallHeight; ++j) {
            unsigned long p1 = imageData[heightIterator][widthIterator];
            unsigned long p1R = p1 & 0x0000ff00;
            unsigned long p1G = p1 & 0x00ff0000;
            unsigned long p1B = p1 & 0x000000ff;

            unsigned long p2 = imageData[heightIterator+1][widthIterator];
            unsigned long p2R = p2 & 0x0000ff00;
            unsigned long p2G = p2 & 0x00ff0000;
            unsigned long p2B = p2 & 0x000000ff;

            unsigned long p3 = imageData[heightIterator][widthIterator+1];
            unsigned long p3R = p3 & 0x0000ff00;
            unsigned long p3G = p3 & 0x00ff0000;
            unsigned long p3B = p3 & 0x000000ff;

            unsigned long p4 = imageData[heightIterator+1][widthIterator+1];
            unsigned long p4R = p4 & 0x0000ff00;
            unsigned long p4G = p4 & 0x00ff0000;
            unsigned long p4B = p4 & 0x000000ff;

            unsigned long aveR = (p1R+p2R+p3R+p4R)/4 & 0x0000ff00;
            unsigned long aveG = (p1G+p2G+p3G+p4G)/4 & 0x00ff0000;
            unsigned long aveB = (p1B+p2B+p3B+p4B)/4 & 0x000000ff;

            char ave = aveR | aveG | aveB;

            smallData[j][i] = ave;
            heightIterator += 2;
        }
        widthIterator += 2;
        heightIterator = 0;
    }

    printf("* writing to big.bmp...\n");

    // Write to bigImage
    fwrite(firstField, sizeof(char), 2, outbig);
    fwrite(&bigFileSize, sizeof(int), 1, outbig);
    fwrite(secondField, sizeof(char), 12, outbig);
    fwrite(&bigWidth, sizeof(int), 1, outbig);
    fwrite(&bigHeight, sizeof(int), 1, outbig);
    fwrite(thirdField, sizeof(char), 8, outbig);
    fwrite(&bigSize, sizeof(int), 1, outbig);
    fwrite(fourthField, sizeof(char), 16, outbig);
    fwrite(bigData, sizeof(char), bigSize, outbig);

    printf("* big.bmp created!\n");
    printf("* writing to small.bmp...\n");

    // Write to smallImage
    fwrite(firstField, sizeof(char), 2, outsmall);
    fwrite(&smallFileSize, sizeof(int), 1, outsmall);
    fwrite(secondField, sizeof(char), 12, outsmall);
    fwrite(&smallWidth, sizeof(int), 1, outsmall);
    fwrite(&smallHeight, sizeof(int), 1, outsmall);
    fwrite(thirdField, sizeof(char), 8, outsmall);
    fwrite(&smallSize, sizeof(int), 1, outsmall);
    fwrite(fourthField, sizeof(char), 16, outsmall);
    fwrite(smallData, sizeof(char), smallSize, outsmall);

    printf("* small.bmp created!\n");

    fclose(infile);
    fclose(outbig);
    fclose(outsmall);

    printf("* exiting\n");

    return 0;
}
