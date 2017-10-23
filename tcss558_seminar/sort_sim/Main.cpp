#include "CustomSortAlgo.h"
#include "SortArray.h"

int main(int argc, char const *argv[])
{
	SortArray arr;
	arr.FillData(0, 100);
	arr.print();
	QuickSort(arr);
	arr.print();

	printf("shit is actually workign!\n");
	return 0;
}