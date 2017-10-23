/******************************************************************************
 * src/CustomSortAlgo.cpp
 * 
 * Contains five well-known sorting algorithms for SortArray
 * - Insertion Sort
 * - Selection Sort
 * - Bubble Sort
 * - Merge Sort
 * - Quick Sort
 *
 * SortArray's original source comes from 'sound-of-sorting':
 * https://github.com/bingmann/sound-of-sorting
 *
 * SortArray is a wrapper around std::vector with wxWidgets functionality to
 * watch the sorting in an application window. I have gutted the wxWidgets
 * dependencies so that I have compatible function headers when I use my sorting
 * algorithms in the following assignment to visualize sorting techniques.
 * 
 * For the sake of writing my own implementation, this is a separate file
 * which is not part of the 'sound-of-sorting' repository nor copied from it.
 * 
 *
 ******************************************************************************/


#include "SortArray.h"
#include <iostream>

/******************************************************************************
 * InsertionSort
 ******************************************************************************/

void InsertionSort(SortArray& A)
{
    int j;
    size_t n = A.size();
    for (int i = 1; i < n; i++) {
        A.mark(i);
        j = i;
        while (j > 0 && A[j - 1] > A[j]) {
            A.swap(j - 1, j);
            j -= 1;
        }
        A.unmark(i);
    }
}

/******************************************************************************
 * SelectionSort
 ******************************************************************************/

void SelectionSort(SortArray& A)
{
    size_t n = A.size();
    int i, j;

    for (j = 0; j < n - 1; j++) {
        int imin = j;

        for (i = j + 1; i < n; i++) {
            if (A[i] < A[imin]) {
                A.mark_swap(i, imin);
                imin = i;
            }
        }

        if (imin != j) {
            A.swap(j, imin);
        }
    }
    A.unwatch_all();
}

/******************************************************************************
 * BubbleSort
 ******************************************************************************/

void BubbleSort(SortArray& A)
{
    size_t n = A.size();
    bool swapped = false;
    do {
        swapped = false;
        for (int i = 1; i < n; i++) {
            if (A[i - 1] > A[i]) {
                A.swap(i - 1, i);
                swapped = true;
            }
        }
    } while(swapped);
}

/******************************************************************************
 * MergeSort
 ******************************************************************************/

void Merge(SortArray& A, int lo, int mid, int hi) {
    std::vector<ArrayItem> sorted_arr(hi - lo);

    A.mark(lo);
    A.mark(mid);
    A.mark(hi - 1);

    int i = lo;
    int j = mid;
    int idx = 0;

    while (i < mid && j < hi) {
        if (A[i] < A[j]) {
            sorted_arr[idx++] = A[i++];
        } else {
            sorted_arr[idx++] = A[j++];
        }
    }

    A.unmark(mid);

    while (i < mid) {
        sorted_arr[idx++] = A[i++];
    }

    while (j < hi) {
        sorted_arr[idx++] = A[j++];
    }

    for (i = 0; i < sorted_arr.size(); i++) {
        A.set(lo + i, sorted_arr[i]);
    }

    A.unmark(lo);
    A.unmark(hi - 1);
}

void MergeSort(SortArray& A, int lo, int hi)
{
    if (hi - lo > 1) {
        int mid = (hi + lo) / 2;

        MergeSort(A, lo, mid);
        MergeSort(A, mid, hi);

        Merge(A, lo, mid, hi);
    }
}

void MergeSort(SortArray& A)
{
    MergeSort(A, 0, A.size());
}

/******************************************************************************
 * QuickSort
 ******************************************************************************/

int Partition(SortArray& A, int lo, int hi) {
    ArrayItem pivot = A[hi];
    int i = lo - 1;

    A.mark(hi);

    for (int j = lo; j < hi; j++) {
        if (A[j] < pivot) {
            i += 1;
            A.swap(i, j);
        }
    }
    if (A[hi] < A[i + 1]) {
        A.swap(i + 1, hi);
    }

    A.unmark(hi);
    return i + 1;
}

void QuickSort(SortArray& A, int lo, int hi) {
    if (lo < hi) {
        int p = Partition(A, lo, hi);
        QuickSort(A, lo, p - 1);
        QuickSort(A, p + 1, hi);
    }
}

void QuickSort(SortArray& A)
{
    QuickSort(A, 0, A.size() - 1);
}