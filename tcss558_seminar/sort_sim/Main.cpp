#include "CustomSortAlgo.h"
#include "SortArray.h"
#include "fileio.h"
#include <string>
#include <cmath>

int main(int argc, char const *argv[])
{
	SortArray arr[1];

	arr[0].FillData(0, 50);
	InsertionSort(arr[0]);
	arr[0].FillData(5, 50);
	InsertionSort(arr[0]);

	arr[0].FillData(0, 50);
	SelectionSort(arr[0]);
	arr[0].FillData(5, 50);
	SelectionSort(arr[0]);

	arr[0].FillData(0, 50);
	BubbleSort(arr[0]);
	arr[0].FillData(5, 50);
	BubbleSort(arr[0]);

	arr[0].FillData(0, 50);
	MergeSort(arr[0]);
	arr[0].FillData(5, 50);
	MergeSort(arr[0]);

	arr[0].FillData(0, 50);
	QuickSort(arr[0]);
	arr[0].FillData(5, 50);
	QuickSort(arr[0]);
	return 0;
}