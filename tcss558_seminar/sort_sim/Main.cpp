#include "CustomSortAlgo.h"
#include "SortArray.h"
#include "fileio.h"

int main(int argc, char const *argv[])
{
	read_csv("/home/jb/breast_cancer_age.csv");

	SortArray arr;
	arr.FillData(0, 100);
	arr.print();
	QuickSort(arr);
	arr.print();

	printf("shit is actually workign!\n");
	return 0;
}