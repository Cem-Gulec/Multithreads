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
    pthread_t publisherThread;

} typedef publisher;

struct packager
{

    int packagerID;
    int totalPackagedBook;
    pthread_t packagerThread;

} typedef packager;

struct book
{

    int bookID;
    int bookType;

} typedef book;

struct buffer
{
    sem_t empty;
    sem_t full;
    pthread_mutex_t mutex;
    int lastPublishedBookIndex;
    book *books;
    int bufferSize;

} typedef buffer;

struct thread_arguments {

    publisher *publisher;
    buffer *buffer;
    int totalBook;
    
} typedef thread_arguments;

struct thread_arguments_2 {

    packager *packager;
    buffer *buffer;
    int totalBook;
    int bufferTypeCount;
    book *package;

}typedef thread_arguments_2;

void addBookToBuffer(buffer buffer[], int bufferIndex);
book retrieveBookFromBuffer (buffer buffer[], int bufferIndex);
void addBookToPackage (book package[], book addedBook, int packageSize);
void clearPackage (book package[], int packageSize);
void printPackage (book package[], int packageSize, int packagerID);
void *publisherBehaviour(void *arg);
int isPackageFull (book package[], int packageSize);
void *packagerBehaviour(void *arg);

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
        for (int i = 0; i < publisherTypeCount; i++) {
            for (int j = 0; j < publisherThreadCount; j++) {
                allPublishers[i][j].publisherID = j + 1;
                allPublishers[i][j].totalPublishedBook = 0;
                allPublishers[i][j].publisherType = i + 1;
            }
        }
        
        packager allPackagers[packagerThreadCount];
        for (int i = 0; i < packagerThreadCount; i++) {
            allPackagers[i].packagerID = i + 1;
            allPackagers[i].totalPackagedBook = 0;
        }

        book allPackages[packagerThreadCount][packageSize];
        for (int i = 0; i < packagerThreadCount; i++) {
            clearPackage(allPackages[i], packageSize);
        }
        
        buffer allBuffers[publisherTypeCount];
        for (int i = 0; i < publisherTypeCount; i++)
        {
            allBuffers[i].bufferSize = initialBufferSize;
            allBuffers[i].books = malloc(sizeof(book) * initialBufferSize);
            allBuffers[i].lastPublishedBookIndex = 0;
            sem_init(&(allBuffers[i].empty),0,initialBufferSize);
            sem_init(&(allBuffers[i].full),0,0);
            pthread_mutex_init(&(allBuffers[i].mutex), NULL);
        }

        
        thread_arguments thr_arg;
        for (int i = 0; i < publisherTypeCount; i++) {
            for (int j = 0; j < publisherThreadCount; j++)  {
                
                thr_arg.totalBook = numberOfBooksThatShouldBePublished;
                thr_arg.publisher = &(allPublishers[i][j]);
                thr_arg.buffer = allBuffers;
                pthread_create(&(allPublishers[i][j].publisherThread), NULL, (void *)publisherBehaviour,(void *) &thr_arg);
            }
        }

        thread_arguments_2 thr_arg_2;
         for (int i = 0; i < packagerThreadCount; i++) {
            thr_arg_2.totalBook = packageSize;
            thr_arg_2.buffer = allBuffers;
            thr_arg_2.packager = &(allPackagers[i]);
            thr_arg_2.package = allPackages[i];
            thr_arg_2.bufferTypeCount = publisherTypeCount;
            pthread_create(&(allPackagers[i].packagerThread), NULL,(void *)packagerBehaviour, &thr_arg_2);
        }
        

        for (int i = 0; i < publisherTypeCount; i++) {
            for (int j = 0; j < publisherThreadCount; j++)  {
               pthread_join(allPublishers[i][j].publisherThread, NULL);
            }
        }

        for (int i = 0; i < packagerThreadCount; i++) {
            pthread_join(allPackagers[i].packagerThread, NULL);
        }
        
        
        for (int i = 0; i < publisherTypeCount; i++) {
            pthread_mutex_destroy(&(allBuffers[i].mutex));
            sem_destroy(&(allBuffers[i].empty));
            sem_destroy(&(allBuffers[i].full));
        }
        
    
        // ------------------ TESTING -------------------------------
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        // addBookToBuffer(allBuffers, 1);
        
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);
        // addBookToBuffer(allBuffers, 2);

       publisherBehaviour((void*)&thr_arg);

        // printf("BEFORE\n");
        // for(int i = 0; i < allBuffers[0].bufferSize; i++) {
        //     printf("Book%d_%d\n", allBuffers[0].books[i].bookType,allBuffers[0].books[i].bookID);
        // }

        // for(int i = 0; i < allBuffers[1].bufferSize; i++) {
        //     printf("Book%d_%d\n", allBuffers[1].books[i].bookType,allBuffers[1].books[i].bookID);
        // }
        
        // book a = retrieveBookFromBuffer(allBuffers,0);
        
        // addBookToPackage(allPackages[0], a, packageSize);
        
        // // for (int i = 0; i < packageSize - 1; i++) {
            
        // //     a = retrieveBookFromBuffer(allBuffers,0);  
        // //     addBookToPackage(allPackages[0], a, packageSize);
        // // }
        // printPackage(allPackages[0], packageSize, 1);
        // printf("\n");
        // printf("PackagedBook: Book%d_%d\n", allPackages[0][0].bookType, allPackages[0][0].bookID);

        // printf("Retrieved: Book%d_%d\n",a.bookType, a.bookID);

        //     printf("AFTER\n");
        //  for(int i = 0; i < allBuffers[0].bufferSize; i++) {
        //     printf("Book%d_%d\n", allBuffers[0].books[i].bookType,allBuffers[0].books[i].bookID);
        // }
    }
}

void addBookToBuffer(buffer buffer[], int bufferIndex)
{

    buffer[bufferIndex - 1].lastPublishedBookIndex++;

    int firstEmptyIndex;
    for (firstEmptyIndex = 0; firstEmptyIndex < buffer[bufferIndex - 1].bufferSize; firstEmptyIndex++)
    {
        if (buffer[bufferIndex - 1].books[firstEmptyIndex].bookID == 0)
        {
            break;
        }
    }

    if (firstEmptyIndex == buffer[bufferIndex - 1].bufferSize)
    {

        buffer[bufferIndex - 1].bufferSize = buffer[bufferIndex - 1].bufferSize * 2;
        buffer[bufferIndex - 1].books = realloc(buffer[bufferIndex - 1].books, sizeof(book) * buffer[bufferIndex - 1].bufferSize);
    }

    buffer[bufferIndex - 1].books[firstEmptyIndex].bookID = buffer[bufferIndex - 1].lastPublishedBookIndex;
    buffer[bufferIndex - 1].books[firstEmptyIndex].bookType = bufferIndex;
}

book retrieveBookFromBuffer (buffer buffer[], int bufferIndex) {

    book retrievedBook;

    retrievedBook = buffer[bufferIndex].books[0];

    for (int i = 1; i < buffer[bufferIndex].bufferSize; i++)
    {
        buffer[bufferIndex].books[i-1] = buffer[bufferIndex].books[i];
    }

    buffer[bufferIndex].books[buffer[bufferIndex].bufferSize - 1].bookID = 0;
    buffer[bufferIndex].books[buffer[bufferIndex].bufferSize - 1].bookType = 0;

    return retrievedBook;
}

void addBookToPackage (book package[], book addedBook, int packageSize) {
    
    int i;
    for (i = 0; i < packageSize; i++) {
        if (package[i].bookID == 0) {
            break;
        }
    }

    package[i].bookID = addedBook.bookID;
    package[i].bookType = addedBook.bookType;
}

void clearPackage (book package[], int packageSize) {

    for (int i = 0; i < packageSize; i++) {
        package[i].bookID = 0;
        package[i].bookType = 0;
    }
}

void printPackage (book package[], int packageSize, int packagerID) {

    printf("Packager%d\tFinished preparing one package. The package contains:\n", packagerID);
    printf("\t\t");
    for (int i = 0; i < packageSize; i++) {
        printf("Book%d_%d ", package[i].bookType, package[i].bookID);
    }   
}

void *publisherBehaviour(void *arg) {
    thread_arguments *thr_arg = (thread_arguments *)arg;
    
    for (int i = 0; i < thr_arg->totalBook; i++) {
        //printf("for Loop: %d\n", i);
        sem_wait(&(thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].empty));
        pthread_mutex_lock(&(thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].mutex));
        // CRITICAL SECTION BEGIN
        // 1. Adding book to corresponding buffer
        addBookToBuffer((*thr_arg).buffer, (thr_arg->publisher->publisherType));
        printf("Publisher %d of type %d\t",thr_arg->publisher->publisherID, thr_arg->publisher->publisherType);
        printf("Book%d_%d is published and put into the buffer %d\n",thr_arg->publisher->publisherType,thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].lastPublishedBookIndex,thr_arg->publisher->publisherType);
        // CRITICAL SECTION END
        pthread_mutex_unlock(&(thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].mutex));
        sem_post(&(thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].full));
    }
    
   //printf("Book%d_%d\n",thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].books[i].bookType,thr_arg->buffer[(thr_arg->publisher->publisherType) - 1].books[j].bookID);
 
}

void *packagerBehaviour(void *arg) {
    thread_arguments_2 *thr_arg_2 = (thread_arguments_2 *)arg;
    
    
    for (int i = 0; i < thr_arg_2->totalBook; i++) {
        //printf("for loop: %d\n", i);
        int chosenIndex = rand() % (thr_arg_2->bufferTypeCount);
        sem_wait(&(thr_arg_2->buffer[chosenIndex].full));
        pthread_mutex_lock(&(thr_arg_2->buffer[chosenIndex].mutex));
        // CRITICAL SECTION
        book retrievedBook = retrieveBookFromBuffer(thr_arg_2->buffer,chosenIndex);
        // TODO: PRINT RETRIEVED BOOK 
        addBookToPackage(thr_arg_2->package, retrievedBook, thr_arg_2->totalBook);
        if (isPackageFull(thr_arg_2->package, thr_arg_2->totalBook)) {
            printPackage(thr_arg_2->package, thr_arg_2->totalBook, thr_arg_2->packager->packagerID);
            clearPackage(thr_arg_2->package, thr_arg_2->totalBook);
        }
        pthread_mutex_unlock(&(thr_arg_2->buffer[chosenIndex].mutex));
        sem_post(&(thr_arg_2->buffer[chosenIndex].empty));
    }
}

int isPackageFull (book package[], int packageSize) {

    if(package[packageSize - 1].bookID != 0) {
        return 1;
    } 
    else {
        return 0;
    }
}