#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

int sudoku[SIZE][SIZE];
int valid[11] = {0};

typedef struct {
    int row;
    int col;
} parameters;

void* check_rows(void* args) {
    for (int i = 0; i < SIZE; i++) {
        int seen[10] = {0};
        for (int j = 0; j < SIZE; j++) {
            int n = sudoku[i][j];
            if (n < 1 || n > 9 || seen[n]) {
                pthread_exit(NULL);
            }
            seen[n] = 1;
        }
    }
    valid[0] = 1;
    pthread_exit(NULL);
}

void* check_cols(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[10] = {0};
        for (int j = 0; j < SIZE; j++) {
            int n = sudoku[j][i];
            if (n < 1 || n > 9 || seen[n]) {
                pthread_exit(NULL);
            }
            seen[n] = 1;
        }
    }
    valid[1] = 1;
    pthread_exit(NULL);
}

void* check_sub(void* param) {
    parameters* p = (parameters*) param;
    int seen[10] = {0};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int n = sudoku[p->row + i][p->col + j];
            if (n < 1 || n > 9 || seen[n]) {
                free(p);
                pthread_exit(NULL);
            }
            seen[n] = 1;
        }
    }

    int index = 2 + (p->row / 3) * 3 + (p->col / 3);
    valid[index] = 1;
    free(p);
    pthread_exit(NULL);
}

int main() {
    int temp[9][9] = {
        {6,2,4,5,3,9,1,8,7},
        {5,1,9,7,2,8,6,3,4},
        {8,3,7,6,1,4,2,9,5},
        {1,4,3,8,6,5,7,2,9},
        {9,5,8,2,4,7,3,6,1},
        {7,6,2,3,9,1,4,5,8},
        {3,7,1,9,5,6,8,4,2},
        {4,9,6,1,8,2,5,7,3},
        {2,8,5,4,7,3,9,1,6}
    };

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            sudoku[i][j] = temp[i][j];

    pthread_t threads[11];
    pthread_create(&threads[0], NULL, check_rows, NULL);
    pthread_create(&threads[1], NULL, check_cols, NULL);

    int threadIndex = 2;
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < SIZE; j += 3) {
            parameters* data = (parameters*) malloc(sizeof(parameters));
            data->row = i;
            data->col = j;
            pthread_create(&threads[threadIndex++], NULL, check_sub, data);
        }
    }

    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }

    int isValid = 1;
    for (int i = 0; i < 11; i++) {
        if (valid[i] == 0) {
            isValid = 0;
            break;
        }
    }

    if (isValid)
        printf("Sudoku solution is valid.\n");
    else
        printf("Sudoku solution is invalid.\n");

    return 0;
}
