//вариант 14

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct{
    int roundsPerThread;
    int successCounter;
    int cardDeck[52];
} deckCardData;

void shuffle(int* arr, int N)
{
    srand(time(NULL));
    for (int i = N - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);
        int tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
    }
}

void* threadFunc(void* data){
    deckCardData* deckCard = (deckCardData*) data;
    deckCard -> successCounter = 0;
    for (int i = 0; i < 52; ++i){
        deckCard -> cardDeck[i] = i;
    }
    for (int i = 0; i < deckCard -> roundsPerThread; ++i){
        shuffle(deckCard -> cardDeck, 52);
        if (deckCard -> cardDeck[0]%13 == deckCard -> cardDeck[1]%13){
            ++(deckCard -> successCounter);
        }
        printf("%d\n", i);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    int threadNumber = 0;
    if (argc == 2){
        for (int i = 0; argv[1][i] > 0; ++i) {
            if (argv[1][i] >= '0' && argv[1][i] <= '9') {
                threadNumber = threadNumber * 10 + argv[1][i] - '0';
            }
        }
    } else if (argc == 1) {
        threadNumber = 1;
    }
    int rounds;
    printf("enter the number of rounds:");
    scanf("%d", &rounds);
    int roundsPerThread = rounds / threadNumber;
    printf("%d\n", roundsPerThread);
    pthread_t* threads = (pthread_t*) malloc(threadNumber * sizeof(pthread_t));
    deckCardData* data = (deckCardData*) malloc(threadNumber * sizeof(deckCardData));
    long double start = clock();
    for (int i = 0; i < threadNumber; ++i){
        data[i].roundsPerThread = roundsPerThread;
        pthread_create(&(threads[i]), NULL, threadFunc, (void*) &data[i]);
    }
    for (int i = 0; i < threadNumber; ++i){
        pthread_join(threads[i], NULL);
    }
    long double end = clock();
    int successSumCounter = 0;
    for (int i = 0; i < threadNumber; ++i){
        successSumCounter += data[i].successCounter;
    }

    printf("%f\n", (double) successSumCounter / rounds);
    printf("Execution time %Lf ms\n", (end - start) / 1000.0);
    free(data);
    return 0;
}