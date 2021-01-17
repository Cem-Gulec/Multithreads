#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h> 
#include <unistd.h>


int main(int argc, char *argv[]) {

    if (argc != 10) {
        fprintf(stderr, "Input format: ./publisherPackager -n 2 3 4 -b 5 -s 6 7");
    }
    
    int publisherTypeCount = atoi(argv[2]);
    int publisherThreadCount = atoi(argv[3]);
    int packagerThreadCount = atoi(argv[4]);

    int numberOfBooksThatShouldBePublished = atoi(argv[6]);
    int packageSize = atoi(argv[8]);
    int initialBufferSize = atoi(argv[9]);

    printf("publisherTypeCount: %d\n", publisherTypeCount);
    printf("publisherThreadCount: %d\n", publisherThreadCount);
    printf("packagerThreadCount: %d\n", packagerThreadCount);
    printf("numberOfBooksThatShouldBePublished: %d\n", numberOfBooksThatShouldBePublished);
    printf("packageSize: %d\n", packageSize);
    printf("initialBufferSize: %d\n", initialBufferSize);



}