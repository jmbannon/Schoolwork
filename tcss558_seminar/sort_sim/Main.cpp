#include "CustomSortAlgo.h"
#include "SortArray.h"
#include "fileio.h"
#include <string>
#include <cmath>



void print_metrics(std::string name, SortArray& A, void (*SortAlgo)(SortArray&)) {
	// Partition the data by increments of 25%

	size_t size = A.size();

	for (int i = 1; i <= 4; i++) {
		double perc = 0.25 * i;
		double elapsed[5] = { 0.0 };
		double elapsed_mean = 0.0;
		double sd = 0.0;

		SortArray A_;
		for (int j = 0; j < 5; j++) {
			clock_t start = clock();

			A_ = A.subset(0.25 * i);
			A_.SetCalcInversions(true);
			SortAlgo(A_);
			clock_t stop = clock();

			elapsed[j] = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
			elapsed_mean += elapsed[j];
		}
		elapsed_mean /= 5;
		
		for (int j = 0; j < 5; j++) {
			sd += pow(elapsed[j] - elapsed_mean, 2);
		}
		sd = sqrt(sd / 5);

    	printf("%s,%f,%f,%f,%d\n", name.c_str(), perc, elapsed_mean, sd, A_.GetInversions());
	}
}

int main(int argc, char const *argv[])
{
	read_csv("/Users/jbannon/workspace/Schoolwork/tcss558_seminar/sort_sim/data/real-world/shares.csv");

	SortArray arr[6];

	arr[0].FillData(-1, 330);
	BubbleSort(arr[0]);
	// print_metrics("InsertionSort", arr[0], InsertionSort);
	// arr[0].UpdateConsole(290);


	// printf("\033[%d;%dH", 20, 0);
	// arr[0].UpdateConsole(290);


	// arr[0].FillData(0, 1000); // random
	// arr[1].FillData(1, 800); // ascending
	// arr[2].FillData(2, 800); // descending
	// arr[3].FillData(5, 800); // 800-8 same elements, shuffled
	// arr[4] = SortArray(read_csv("/Users/jbannon/workspace/Schoolwork/tcss558_seminar/sort_sim/shares2.csv"));
	// arr[5] = SortArray(read_csv("/Users/jbannon/workspace/Schoolwork/tcss558_seminar/sort_sim/breast_cancer_age.csv"));
	// std::string data_names[6] = { "Random", "Ascending", "Descending", "Equal-8", "OnlineArticleShares", "BreastCancerAge" };

	// void (*SortAlgos[5])(SortArray&) = { InsertionSort, SelectionSort, BubbleSort, MergeSort, QuickSort };

	// std::string algo_names[5] = { "InsertionSort", "SelectionSort", "BubbleSort", "MergeSort", "QuickSort" };

	// for (int i = 0; i < 6; i++) {
	// 	printf("%s:\n", data_names[i].c_str());

	// 	for (int j = 0; j < 5; j++) {
	// 		print_metrics(algo_names[j], arr[i], SortAlgos[j]);
	// 	}
	// }
	return 0;
}