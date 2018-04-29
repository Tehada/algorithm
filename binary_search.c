#include <stdio.h>


int binsearch(int array[], int size, int value) {
    int index = 0;
    while (size > 1) {
        if (array[index + (size + 1) / 2] <= value) {
            index += (size + 1) / 2;
            size /= 2;
        } else {
            size = (size + 1) / 2;
        }
    }
    if (array[index] == value) {
        return index;
    }
    return -1;
}


int main(int argc, char *argv[]) {
    int arr[9] = { 1, 2 ,3, 4, 5, 6 };
    //printf("%d\n", *argv[1] - 48);
    printf("%d\n", binsearch(arr, 6, 6));
}