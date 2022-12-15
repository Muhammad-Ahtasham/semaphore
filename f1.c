#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int buffersize = 10;
int maxitem = 5;
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[10];
pthread_mutex_t mutex; 

#define prod_max 4000
#define con_max 1000

int counter[10]={0,0,0,0,0,0,0,0,0,0};
int allcounter=0;

void *producer(void *args) {
    
    for (int i=0; i<1000; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = *((int*)args);

        in = (in+1)%buffersize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *args) {
    for (int i=0; i<2500; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        counter[item]+=1;
        out = (out+1)%buffersize;
        allcounter+=1;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}


void display()
{
  for(int i=0;i<10;i++){
   printf("counter[%d]=%d\n",1,counter[i]);
 
  }
  printf("allcounter=%d\n",allcounter);
}



int main() {
    pthread_t pro[10], con[4];
    
  // pthread_mutex_init(&mutex, NULL);
  
    sem_init(&empty, 0, buffersize);
    sem_init(&full, 0, 0);
    
    int a[10] = {0,1,2,3,4,5,6,7,8,9,};
    for(int i=0; i<10; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i=0; i<4; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }
    for(int i=0; i<10; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i=0; i<4; i++) {
        pthread_join(con[i], NULL);
    }
    display();
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
