#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

struct publisher
{
    int publisherID;
    int publisherType;
    int totalPublishedBook;

} typedef publisher;

struct packager
{
    int packagerID;
    int bookCount; // How many books has packager packaged into a single box
    int *bookID;   // To print out which books are filled
    int *bookType;
} typedef packager;

struct publisherType
{
    int publisherType;
    int bookCount;
    sem_t empty;
    sem_t full;
} typedef publisherType;

struct bufferElement
{
    int bookID;
    int publisherType;
} typedef bufferElement;

int isBufferFull(bufferElement *buffer, int bufferSize);
void addBookToBuffer(bufferElement *buffer, int bookID, int publisherType, int bufferSize);
void deleteBookFromBuffer(bufferElement *buffer, int *bookID, int *publisherType, int bufferSize);


int main(int argc, char *argv[])
{
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

        publisher allPublishers[publisherTypeCount][publisherThreadCount];
        bufferElement allBuffers[publisherTypeCount][initialBufferSize];
        publisherType allPublisherTypes[publisherTypeCount];
        packager allPackagers[packagerThreadCount];

        for (int i = 0; i < packagerThreadCount; i++)
        {
            allPackagers[i].bookID = malloc(sizeof(int) * packageSize);
            allPackagers[i].bookType = malloc(sizeof(int) * packageSize);
        }

        for (int i = 0; i < publisherTypeCount; i++)
        {
            for (int j = 0; j < publisherThreadCount; j++)
            {
                allPublishers[i][j].publisherID = j + 1;
                allPublishers[i][j].publisherType = i + 1;
            }
        }

        for (int i = 0; i < publisherTypeCount; i++)
        {
            for (int j = 0; j < publisherThreadCount; j++)
            {
                printf("PublisherID: %d ---- PublisherType: %d\n", allPublishers[i][j].publisherID, allPublishers[i][j].publisherType);
            }
        }
        printf("IsBufferFull: %d\n", isBufferFull(allBuffers[0], initialBufferSize));

        addBookToBuffer(allBuffers[0], 1, 1, initialBufferSize);

        printf("BookID: %d\n", allBuffers[0][0].bookID);
        
        int bID;
        int pType;
        deleteBookFromBuffer(allBuffers[0],&bID,&pType,initialBufferSize);

        printf("BookID: %d\n", bID);
    }
}

int isBufferFull(bufferElement *buffer, int bufferSize)
{
    if (buffer[bufferSize - 1].bookID == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void addBookToBuffer(bufferElement *buffer, int bookID, int publisherType, int bufferSize)
{
    int i;
    for (i = 0; i < bufferSize; i++)
    {
        if (buffer[i].bookID == 0)
        {
            break;
        }
    }
    if (i < bufferSize)
    {
        buffer[i].bookID = bookID;
        buffer[i].publisherType = publisherType;
    }
}

void deleteBookFromBuffer(bufferElement *buffer, int *bookID, int *publisherType, int bufferSize) {

   if (buffer[0].bookID == 0) {
       // TODO CHECK REMAINING THREADS
   }
   else {
       *bookID = buffer[0].bookID;
       *publisherType = buffer[0].publisherType; 
   }

    for (int i = 1; i < bufferSize; i++)
    {
        buffer[i-1] = buffer[i];
    }

    buffer[bufferSize - 1].bookID = 0;
    buffer[bufferSize - 1].publisherType = 0;
}