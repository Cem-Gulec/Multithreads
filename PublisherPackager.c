#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
/*
    This program aims to create a Publisher & Packager Mechanism with multithreaded programming approaches. 
    Our mechanism is just an another variant of well-known problem "Producer & Consumer".

    Authors: Tuna Cinsoy & Enver Aslan & Cem Güleç
*/

// Structure for holding information about publishers
struct publisher
{
    int publisherID;
    int *books;
    int howManyBooksPublisherHasPublished;

} typedef publisher;

// This struct holds elements of buffers
struct bufferElement
{
    int publisherID;
    int bookID;
} typedef bufferElement;

// Our publishers are categorized underneath their types. For instance, "Novel" book type will have its own publishers.
struct publisherType
{
    int bufferSize;
    int publisherTypeID;
    int bookCount;
    publisher *publishers;
    bufferElement *buffer;
} typedef publisherType;

// Simple structure to hold information about books
struct book
{
    int publisherType;
    int bookID;
} typedef book;

//Packager structure, contains necessary attributes for further operations
struct packager
{
    int packagerID;
    int packageSize;
    book *books;
    int howManyBooksArePackaged;
} typedef packager;

// Function Prototypes
publisherType *initializePublishers(int publisherTypeCount, int publisherCount, int numberOfBooksThatShouldBePublished, int initialBufferSize);
book getBookFromBuffer(publisherType publisherType);
int selectBufferTypeRandomly(int publisherTypeCount);

int main(int argc, char *argv[])
{
    // Checking argument count as part of error checking
    if (argc != 10)
    {
        fprintf(stderr, "Input format: ./publisherPackager -n 2 3 4 -b 5 -s 6 7\n");
    }
    else
    {
        // Retrieving variables from input arguments
        int publisherTypeCount = atoi(argv[2]);
        int publisherThreadCount = atoi(argv[3]);
        int packagerThreadCount = atoi(argv[4]);

        int numberOfBooksThatShouldBePublished = atoi(argv[6]);
        int packageSize = atoi(argv[8]);
        int initialBufferSize = atoi(argv[9]);

        //Sending variables to start the creation of structs
        publisherType *publisherType = initializePublishers(publisherTypeCount, publisherThreadCount, numberOfBooksThatShouldBePublished, initialBufferSize);

        publisherType[0].buffer[0].bookID = 1;
        publisherType[0].buffer[0].publisherID = 1;
        publisherType[0].buffer[1].bookID = 2;
        publisherType[0].buffer[1].publisherID = 2;
        publisherType[0].buffer[2].bookID = 3;
        publisherType[0].buffer[2].publisherID = 3;
        publisherType[0].buffer[3].bookID = 4;
        publisherType[0].buffer[3].publisherID = 4;
        publisherType[0].buffer[4].bookID = 5;
        publisherType[0].buffer[4].publisherID = 5;

        // printf("BEFORE\n");
        // for (int i = 0; i < publisherType[0].bufferSize; i++)
        // {

        //     printf("Book%d_%d\n", publisherType[0].buffer[i].publisherID, publisherType[0].buffer[i].bookID);
        // }

        // book a = getBookFromBuffer(publisherType[0]);

        // printf("AFTER\n");
        // for (int i = 0; i < publisherType[0].bufferSize; i++)
        // {
        //     printf("Book%d_%d\n", publisherType[0].buffer[i].publisherID, publisherType[0].buffer[i].bookID);
        // }

        // for (int i = 0; i < 10; i++)
        // {
        //     printf("Random Buffer Type: %d\n", selectBufferTypeRandomly(3));
        // }

        // printf("publisherTypeCount: %d\n", publisherTypeCount);
        // printf("publisherThreadCount: %d\n", publisherThreadCount);
        // printf("packagerThreadCount: %d\n", packagerThreadCount);
        // printf("numberOfBooksThatShouldBePublished: %d\n", numberOfBooksThatShouldBePublished);
        // printf("packageSize: %d\n", packageSize);
        // printf("initialBufferSize: %d\n", initialBufferSize);
    }
}

publisherType *initializePublishers(int publisherTypeCount, int publisherCount, int numberOfBooksThatShouldBePublished, int initialBufferSize)
{
    // First of all, we are declaring publisherType just as many as publisherTypeCount
    publisherType *publisherType = malloc(sizeof(publisherType) * publisherTypeCount);

    // Initializing the attributes of publisherType structure, and also allocating buffer and publishers array
    for (int i = 0; i < publisherTypeCount; i++)
    {
        publisherType[i].publisherTypeID = i + 1;
        publisherType[i].buffer = malloc(sizeof(bufferElement) * initialBufferSize);
        publisherType[i].bufferSize = initialBufferSize;
        publisherType[i].publishers = malloc(sizeof(publisher) * publisherCount);
        publisherType[i].bookCount = 0;
        // Initializing the attributes of publishers structure, and also allocating books array
        for (int j = 0; j < publisherCount; j++)
        {
            publisherType[i].publishers[j].publisherID = j + 1;
            publisherType[i].publishers[j].books = malloc(sizeof(int) * numberOfBooksThatShouldBePublished);
            publisherType[i].publishers[j].howManyBooksPublisherHasPublished = 0;
        }
    }

    // Printing out for debugging purposes
    for (int i = 0; i < publisherTypeCount; i++)
    {
        for (int j = 0; j < publisherCount; j++)
        {

            printf("publisherType: %d --- publisherID: %d ---- howManyBookPublisherHasPublished: %d ---- howManyBooksPublisherHasToPublish: %d\n", publisherType[i].publisherTypeID, publisherType[i].publishers[j].publisherID, publisherType[i].publishers[j].howManyBooksPublisherHasPublished, numberOfBooksThatShouldBePublished);
        }
    }

    return publisherType;
}

int selectBufferTypeRandomly(int publisherTypeCount)
{
    return rand() % (publisherTypeCount);
}

book getBookFromBuffer(publisherType publisherType)

{
    book retrievedBook;
    retrievedBook.publisherType = publisherType.publisherTypeID;

    retrievedBook.bookID = publisherType.buffer[0].bookID;

    for (int i = 1; i < publisherType.bufferSize; i++)
    {
        publisherType.buffer[i - 1] = publisherType.buffer[i];
    }

    publisherType.buffer[publisherType.bufferSize - 1].bookID = 0;
    publisherType.buffer[publisherType.bufferSize - 1].publisherID = 0;

    return retrievedBook;
}

void addBookToBuffer(publisherType publisherType, int publisherID, int bookID)
{
    int i;
    for (i = 0; i < publisherType.bufferSize; i++)
    {
        if (publisherType.buffer[i].bookID == 0 && publisherType.buffer[i].publisherID == 0)
        {
            break;
        }
    }

    publisherType.buffer[i].bookID = bookID;
    publisherType.buffer[i].publisherID = publisherID;
}