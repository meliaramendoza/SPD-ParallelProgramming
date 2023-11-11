#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS 4

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

void sortArray(int arr[], int size) {
    qsort(arr, size, sizeof(int), compare);
}

int getTarget() {
    int target;
    printf("\nIngrese el numero que desea buscar: ");
    scanf("%d", &target);
    return target;
}

int binarySearch(int arr[], int target, int left, int right) {
    if (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target)
            return mid;
        if (arr[mid] < target)
            return binarySearch(arr, target, mid + 1, right);

        return binarySearch(arr, target, left, mid - 1);
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

void performSearch(int arr[], int target, int ARRAY_SIZE) {
    int result = -1;
    #pragma omp parallel shared(result)
    {
        int localResult = -1;
        int thread_id = omp_get_thread_num();
        int chunk_size = ARRAY_SIZE / NUM_THREADS;
        int left = thread_id * chunk_size;
        int right = (thread_id == NUM_THREADS - 1) ? (ARRAY_SIZE - 1) : (left + chunk_size - 1);
        localResult = binarySearch(arr, target, left, right);
        if (localResult != -1) {
            #pragma omp critical
            {
                if (result == -1) {
                    result = result || localResult;
                }
            }
        }
    }
    #pragma omp single
    {
        printSearchResults(arr, target, result, ARRAY_SIZE);
    }
}

int main() {
    int *array;
    int arraySize;
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);
    double inicio, fin;

    arraySize = getArraySize();
    if (arraySize == -1) {
        return 1;
    }

    int *arr = initializeArray(arraySize);
    if (arr == NULL) {
        return 1;
    }

    srand((unsigned int)time(NULL));
    fillArrayWithUniqueNumbers(arr, arraySize);
    sortArray(arr, arraySize);
    int target = getTarget();
    omp_set_num_threads(NUM_THREADS);

    inicio = omp_get_wtime();
    
    performSearch(arr, target, arraySize);

    fin = omp_get_wtime();

    printf("\n\nTiempo total transcurrido en segundos: %f\n", (fin - inicio));
    free(arr);
    return 0;
}