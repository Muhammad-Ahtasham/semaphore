#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int buffersize = 5;
int maxitem = 5;
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[5];
pthread_mutex_t mutex;

void *producer(void *pno) {
    int item[5] = {5,10,15,20,25};
    for (int i=0; i<maxitem; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item[i];
        printf("Producer %d: Insert Item %d at\t%d\n", *((int *)pno), buffer[in], in);
        in = (in+1)%buffersize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *cno) {
    for (int i=0; i<maxitem; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from\t%d\n", *((int *)cno), item, out);
        out = (out+1)%buffersize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
int main() {
    pthread_t pro[5], con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, buffersize);
    sem_init(&full, 0, 0);
    int a[5] = {1,2,3,4,5};
    for(int i=0; i<5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i=0; i<5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }
    for(int i=0; i<5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i=0; i<5; i++) {
        pthread_join(con[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
