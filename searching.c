#include <stdio.h>

#define MAX 100

int linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key)
            return i;
    }
    return -1;
}




int main(void) {
    
    return 0;
}
