#include <stdio.h>

#define MAX 100

int linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key)
            return i;
    }
    return -1;
}
int binarySearch(int arr[], int n, int key) {
    int low = 0, high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}



int main(void) {

int arr[MAX], n, key, choice, result;

    printf("Enter the number of elements: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of elements.\n");
        return 1;
    }

    printf("Enter the roll numbers:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    do {
        printf("\n===== MENU =====\n");
        printf("1. Linear Search\n");
        printf("2. Binary Search\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter roll number to search: ");
                scanf("%d", &key);

                result = linearSearch(arr, n, key);

                if (result != -1)
                    printf("Roll number found at index %d.\n", result);
                else
                    printf("Roll number not found.\n");
                break;

            

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
    
    
    return 0;
}
