/*
 ============================================================================

 Name        : HW4.c
 Author      : Jesse Bannon
 Version     : 1.0
 Copyright   : TCSS 333
 Description : Reads in all purchases and prints organized text file versions
               of all customers and purchases.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LENGTH 30

/*
============================================================================

Structs used in HW4.c:

    customerPurchases:
        Contains item name, price, and quantity.

    customer:
        Contains customer name, number of purchases, and array of
        ten purchasable items.

============================================================================
 */

struct str_purchases {
    char item[NAME_LENGTH];
    float price;
    int quantity;
};

typedef struct str_purchases customerPurchases;


struct str_customer {
    char name[NAME_LENGTH];
    int numOfPurchases;
    customerPurchases purchases[10];
};

typedef struct str_customer customer;

/*
============================================================================

Prototypes used in HW4.c

============================================================================
 */

void inputItem(customer *person, const char itemName[], const int quantity, const float price);
void inputCustomerPurchase(customer customers[], const char customerName[],
        const char itemName[], const int quantity, const float price, int *count);
void readInputFile(customer customers[], int *count);
void writeOutputFileTime(customer customers[], const int count);
float getItemValue(const customer person, const int purchaseIndex);
float getOrderTotal(const customer person, const int numOfPurchases);
void sortPurchasesByItemValue(customer *person);
void sortCustomers(customer customers[], const int count);
void writeOutputFileMoney(customer customers[], const int count);

/*
============================================================================

inputItem:
    Saves a purchased item to a customer's purchase array and
    increments the amount of purchases by one.

============================================================================
 */
void inputItem(customer *person, const char itemName[], const int quantity, const float price) {
    strcpy(person->purchases[person->numOfPurchases].item, itemName);
    person->purchases[person->numOfPurchases].quantity = quantity;
    person->purchases[person->numOfPurchases].price = price;
    ++person->numOfPurchases;
}

/*
============================================================================

inputCustomerPurchase:
    Saves a purchased item to a customer's purchase array and
    increments the amount of purchases by one.

============================================================================
 */
void inputCustomerPurchase(customer customers[], const char customerName[],
        const char itemName[], const int quantity, const float price, int *count) {

    int i;
    for (i = 0; i < 20; i++) {
        if (strcmp(customers[i].name, customerName) == 0) {
            inputItem(&customers[i], itemName, quantity, price);
            break;
        } else if (i == *(count)) {
            strcpy(customers[i].name, customerName);
            customers[i].numOfPurchases = 0;
            inputItem(&customers[i], itemName, quantity, price);
            ++*(count);
            break;
        }
    }
}

/*
============================================================================

readInputFile:
    Reads hw4input.txt by each line and saves values of each line in the
    format of <String customerName> <Int quantity> <String itemName> $<Float price>
    and calls inputCustomerPurchase to input the values into customers[] array.
    Function ends when the next line of hw4input.txt does not start with an
    alphabetic character.

============================================================================
 */
void readInputFile(customer customers[], int *count) {
    FILE *infile = fopen("hw4input.txt", "r");
    char line[NAME_LENGTH * 2];
    char customerName[NAME_LENGTH], itemName[NAME_LENGTH];
    int quantity;
    float price;

    while (fgets(line, NAME_LENGTH*2, infile) != NULL) {
        if (!isalpha(line[0]))
            break;

        sscanf(line, " %s %d %s %*c %f", customerName, &quantity, itemName, &price);
        inputCustomerPurchase(customers, customerName, itemName, quantity, price, count);
    }
}

/*
============================================================================

writeOutputFileTime:
    After reading in the names by the order of the file from readInputFile,
    creates text file hw4time.txt and stores names and purchases in chronological
    order of reading in the names and writes them in the format of

    <Customer Name>
    <Item Name> <Quantity> $<Item Price>
    <Item Name> <Quantity> $<Item Price>
    ...

    <Customer Name>
    ...

============================================================================
 */
void writeOutputFileTime(customer customers[], const int count) {
    FILE *outfile = fopen("hw4time.txt", "w");
    int i,j;
    for (i = 0; i < count; i++) {
        fprintf(outfile, "%s\n", customers[i].name);
        for (j = 0; j < customers[i].numOfPurchases; j++) {
            fprintf(outfile, "%s %d $%.02f\n", customers[i].purchases[j].item,
                    customers[i].purchases[j].quantity, customers[i].purchases[j].price);
        }
        fwrite("\n", sizeof(char), 1, outfile);
    }
}

/*
============================================================================

getItemValue:
    Returns the price of an item times the quantity from customer person's
    purchases array of index purchaseIndex. Returns -1 if index is out of
    range.

============================================================================
 */
float getItemValue(const customer person, const int purchaseIndex) {
    if (purchaseIndex >= person.numOfPurchases)
        return -1;
    else
        return person.purchases[purchaseIndex].price
                * person.purchases[purchaseIndex].quantity;
}

/*
============================================================================

getOrderTotal:
    Returns the total amount of money spent by a customer by adding all
    item values to a sum.

============================================================================
 */
float getOrderTotal(const customer person, const int numOfPurchases) {
    float orderTotal = 0;
    int i;
    for (i = 0; i < numOfPurchases; i++)
        orderTotal += getItemValue(person, i);

    return orderTotal;
}

/*
============================================================================

sortPurchasesByItemValue:
    Sorts the purchases array from highest itemValue to lowest.

============================================================================
 */
void sortPurchasesByItemValue(customer *person) {
    int i,j;
    for (i = 0; i < person->numOfPurchases; i++) {
        for (j = i + 1; j < person->numOfPurchases; j++) {
            if (getItemValue(*(person), i) < getItemValue(*(person), j)) {
                customerPurchases temp = person->purchases[i];
                person->purchases[i] = person->purchases[j];
                person->purchases[j] = temp;
            }
        }
    }
}

/*
============================================================================

sortCustomers:
    Sorts the customers from highest order total to lowest and calls
    sortPurchasesByItemValue on every customer to sort their purchase
    array as well.

============================================================================
 */
void sortCustomers(customer customers[], const int count) {
    int i,j;
    for (i = 0; i < count-1; i++) {
        for (j = i + 1; j < count; j++) {
            if (getOrderTotal(customers[i], customers[i].numOfPurchases)
                    < getOrderTotal(customers[j], customers[j].numOfPurchases)) {
                customer temp = customers[i];
                customers[i] = customers[j];
                customers[j] = temp;
            }
        }
        sortPurchasesByItemValue(&customers[i]);
    }
}

/*
============================================================================

writeOutputFileMoney:
    After sorting names and purchases from sortCustomers, writes to the
    text file hw4money.txt in the format of

    <Customer Name>, Total Order = $<Order Total>
    <Item Name> <Quantity> $<Item Price>, Item Value = $<Item Price * Quantity>
    <Item Name> <Quantity> $<Item Price>, Item Value = $<Item Price * Quantity>
    ...

    <Customer Name>, Total Order = $<Order Total>
    ...

============================================================================
 */
void writeOutputFileMoney(customer customers[], const int count) {
    FILE *outfile = fopen("hw4money.txt", "w");
    int i,j;
    for (i = 0; i < count; i++) {
        fprintf(outfile, "%s, Total Order = $%.02f\n", customers[i].name,
                getOrderTotal(customers[i], customers[i].numOfPurchases));

        for (j = 0; j < customers[i].numOfPurchases; j++) {
            fprintf(outfile, "%s %d $%.02f, Item Value = $%.02f\n",
                    customers[i].purchases[j].item, customers[i].purchases[j].quantity,
                    customers[i].purchases[j].price, getItemValue(customers[i], j));
        }
        fwrite("\n", sizeof(char), 1, outfile);
    }
}

/*
============================================================================

main:
    1st Creates customer array of size 20.
    2nd Reads in from input file (keeping track of customer count).
    3rd Writes text file of customer purchases in chronological order
    4th Sorts the customers and their purchases by order total.
    5th Writes text file of customer purchases in order of purchase order total.

============================================================================
 */
int main(void) {
	customer customers[20];
	int count = 0;

	printf("* Reading input file...\n");
	readInputFile(customers, &count);

	printf("* Read! Writing hw4time.text...\n");
	writeOutputFileTime(customers, count);

	printf("* Written! Sorting customers...\n");
	sortCustomers(customers, count);

	printf("* Sorted! Writing hw4money.text...\n");
	writeOutputFileMoney(customers, count);

	printf("* Written! Exiting program.");
	return 0;
}
