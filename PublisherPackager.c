#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

struct publisher
{
    int publisherID;
    int *books;

} typedef publisher;

struct buffer
{
    int publisherID;
    int bookID;
} typedef buffer;

struct publisherType
{
    int publisherTypeID;
    publisher *publishers;
    buffer *buffer;
} typedef publisherType;

void initializePublishers(int publisherTypeCount, int publisherCount, int numberOfBooksThatShouldBePublished);

int main(int argc, char *argv[])
{

    if (argc != 10)
    {
        fprintf(stderr, "Input format: ./publisherPackager -n 2 3 4 -b 5 -s 6 7\n");
    }
    else
    {

        int publisherTypeCount = atoi(argv[2]);
        int publisherThreadCount = atoi(argv[3]);
        int packagerThreadCount = atoi(argv[4]);

        int numberOfBooksThatShouldBePublished = atoi(argv[6]);
        int packageSize = atoi(argv[8]);
        int initialBufferSize = atoi(argv[9]);

        initializePublishers(publisherTypeCount, publisherThreadCount, numberOfBooksThatShouldBePublished);

        // printf("publisherTypeCount: %d\n", publisherTypeCount);
        // printf("publisherThreadCount: %d\n", publisherThreadCount);
        // printf("packagerThreadCount: %d\n", packagerThreadCount);
        // printf("numberOfBooksThatShouldBePublished: %d\n", numberOfBooksThatShouldBePublished);
        // printf("packageSize: %d\n", packageSize);
        // printf("initialBufferSize: %d\n", initialBufferSize);
    }
}

void initializePublishers(int publisherTypeCount, int publisherCount, int numberOfBooksThatShouldBePublished)
{

    publisherType publisherType[publisherTypeCount];

    // Initialize Publishers and also their types with their books
    for (int i = 0; i < publisherTypeCount; i++)
    {
        publisherType[i].publishers = malloc(sizeof(publisher) * publisherCount);
        publisherType[i].publisherTypeID = i + 1;
        for (int j = 0; j < publisherCount; j++)
        {
            publisherType[i].publishers[j].publisherID = j + 1;
            publisherType[i].publishers[j].books = malloc(sizeof(int) * numberOfBooksThatShouldBePublished);
            for (int k = 0; k < numberOfBooksThatShouldBePublished; k++)
            {
                publisherType[i].publishers[j].books[k] = k + 1;
            }
        }
    }

    // Printing out
    for (int i = 0; i < publisherTypeCount; i++)
    {
        for (int j = 0; j < publisherCount; j++)
        {
            for (int k = 0; k < numberOfBooksThatShouldBePublished; k++)
            {
                printf("publisherType: %d --- publisherID: %d ---- numberOfBooks: %d\n", publisherType[i].publisherTypeID, publisherType[i].publishers[j].publisherID, publisherType[i].publishers[j].books[k]);
            }
        }
    }
}