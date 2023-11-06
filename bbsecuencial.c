#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getArraySize() {
    int arraySize;
    printf("\nIngrese el tamano del arreglo: ");
    scanf("%d", &arraySize);
    if (arraySize <= 0) {
        printf("\nEl tamano del arreglo debe ser mayor que 0.\n");
        return -1;
    }
    return arraySize;
}

int *initializeArray(int arraySize) {
    int *array = (int *)malloc(arraySize * sizeof(int));
    if (array == NULL) {
        perror("Error al asignar memoria para el arreglo");
        return NULL;
    }
    return array;
}

void fillArrayWithUniqueNumbers(int array[], int size) {
    for (int i = 0; i < size; i++) {
        int uniqueNumber;
        int isUnique;
        do {
            uniqueNumber = rand() % 1000;
            isUnique = 1;
            for (int j = 0; j < i; j++) {
                if (array[j] == uniqueNumber) {
                    isUnique = 0;
                    break;
                }
            }
        } while (!isUnique);
        array[i] = uniqueNumber;
    }
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void sortArray(int array[], int size) {
    qsort(array, size, sizeof(int), compare);
}

int getTarget() {
    int target;
    printf("\nIngrese el numero que desea buscar: ");
    scanf("%d", &target);
    return target;
}

int binarySearch(int array[], int target, int left, int right) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (array[mid] == target)
            return mid;
        if (array[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

void printSearchResults(int arr[], int target, int result, int ARRAY_SIZE) {
    if (result != -1) {
        printf("\nEl elemento %d se encuentra en la posicion %d.\n", target, result);
    } else {
        printf("\nEl elemento %d no se encuentra en el arreglo.\n", target);
    }
    printf("\nNumeros contenidos en el arreglo:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", arr[i]);
    }
}

int main() {
    int *array;
    int arraySize;
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    clock_t startSearchTime = clock();

    arraySize = getArraySize();
    if (arraySize == -1) {
        return 1;
    }

    array = initializeArray(arraySize);
    if (array == NULL) {
        return 1;
    }

    fillArrayWithUniqueNumbers(array, arraySize);
    sortArray(array, arraySize);
    int target = getTarget();
    int result = binarySearch(array, target, 0, arraySize - 1);
    printSearchResults(array, target, result, arraySize);

    clock_t endSearchTime = clock();
    double searchTime = (double)(endSearchTime - startSearchTime) / CLOCKS_PER_SEC;

    printf("\n\nTiempo total transcurrido en segundos: %4.15f\n", searchTime);
    free(array);
    return 0;
}