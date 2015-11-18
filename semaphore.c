#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX 10000

struct sembuf mybuf;
int semid;

void* my_thread(void* dummy) 
{
    int *a = (int *) dummy;
    int i;
    for (i = 0; i < MAX; i++)
    {
        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;

        if (semop(semid , &mybuf , 1) < 0) 
        {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
        
        (*a)++;
        printf("a = %d\n", *a);
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        if (semop(semid , &mybuf , 1) < 0) 
        {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
    }
    
    return NULL;
}




int main()
{
    int a = 0;
    key_t key;
    const char pathname[] = "semaphore.c";
    if ( (key = ftok(pathname , 0)) < 0 ) 
    {
        printf("Can`t generate key\n");
        exit(-1);
    }
    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0)
    {
        printf("Can`t get semid\n");
        exit(-1);
    }
    mybuf.sem_op = 1;
    semop(semid, &mybuf, 1);
    pthread_t thread_id1, thread_id2;
    int result1, result2;
 
    result1 = pthread_create(&thread_id1, (pthread_attr_t *)NULL, my_thread, (void *)&a);
    result2 = pthread_create(&thread_id2, (pthread_attr_t *)NULL, my_thread, (void *)&a);
    if (result1) {
         printf("Can`t create thread, return value = %d\n" , result1);
         exit(-1);
     }
     if (result2) {
         printf("Can`t create thread, return value = %d\n" , result2);
         exit(-1);
     }

    

    pthread_join(thread_id1 , (void **) NULL);
    pthread_join(thread_id2 , (void **) NULL);
    printf("a = %d\n" , a);
    semctl(semid, IPC_RMID, 0);
    return 0;
}

