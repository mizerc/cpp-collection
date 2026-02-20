#include <stdio.h>

// Function to swap two elements
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// Partition function (Lomuto partition scheme)
int partition(int arr[], int low, int high) {
  // Pick pivot as the last element
  int pivotValue = arr[high];
  // Initialize boundary to the element before the first element
  int boundary = low - 1;

  for (int j = low; j < high; j++) {
    if (arr[j] < pivotValue) { // smaller goes to left
      boundary++;
      swap(&arr[boundary], &arr[j]);
    }
  }
  
  // All elements smaller than pivot are to the left of boundary
  // so put the pivot in the correct position, at the boundary
  boundary++;
  swap(&arr[boundary], &arr[high]);
  return boundary;
}

// QuickSort recursion
void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

int main() {
  int arr[] = {10, 7, 8, 9, 1, 5};
  int n = sizeof(arr) / sizeof(arr[0]);
  quickSort(arr, 0, n - 1);
  printf("Sorted array: ");
  for (int i = 0; i < n; i++)
    printf("%d ", arr[i]);
  return 0;
}
