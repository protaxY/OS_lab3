//вариант 14

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static unsigned long long g_seed;

static inline void fast_srand(int seed)
{
    g_seed = seed;
}

unsigned long long fast_rand(void)
{
    g_seed = (214013ull * g_seed + 2531011ull);
    return (g_seed >> 16ull) & 0x7FFFull;
}
typedef struct{
    int roundsPerThread;
    int successCounter;
    int cardDeck[52];
} deckCardData;

void shuffle(int* arr, int N)
{
    for (int i = N - 1; i >= 1; i--)
    {
        int j = fast_rand() % (i + 1);
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
    }
    return NULL;
}

int main(int argc, char* argv[]){
    fast_srand(time(NULL));
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
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int roundsPerThread = rounds / threadNumber;
    pthread_t* threads = (pthread_t*) malloc(threadNumber * sizeof(pthread_t));
    deckCardData* data = (deckCardData*) malloc(threadNumber * sizeof(deckCardData));
    for (int i = 0; i < threadNumber; ++i){
        data[i].roundsPerThread = roundsPerThread;
        if (pthread_create(&(threads[i]), NULL, threadFunc, (void*) &data[i])){
            printf("Error creating thread!\n");
            return -1;
        }
    }
    for (int i = 0; i < threadNumber; ++i){
        if (pthread_join(threads[i], NULL)) {
            printf("Error executing thread!\n");
            return -1;
        }
    }
    free(threads);
    int successSumCounter = 0;
    for (int i = 0; i < threadNumber; ++i){
        successSumCounter += data[i].successCounter;
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("credibility of success is %f\n", (double) successSumCounter / rounds);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("execution time is %lf s\n", elapsed);
    free(data);
    return 0;
}