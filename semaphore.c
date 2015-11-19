#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX 100000

int a = 0;
struct sembuf mybuf;
int semid;

void semaphore1() {
    struct sembuf mybuf;
    mybuf.sem_op = -1;
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    if (semop(semid , &mybuf , 1) < 0) 
        {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
}

void semaphore2() {
    struct sembuf mybuf;
    mybuf.sem_op = 1;
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    if (semop(semid , &mybuf , 1) < 0) 
        {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
}
void* my_thread1(void* args) {
    int i;
    for(i = 0; i < MAX; i++) {
        semaphore1();
        a++;
        semaphore2();
    }
    return NULL;
}

void* my_thread2(void* args) {
    int i;
    for(i = 0; i < MAX; i++) {
        semaphore1();
        a++;
        semaphore2();
    }
    return NULL;
}



int main()
{
    pthread_t thread_id1, thread_id2;
    int result;
    key_t key;
    char* pathname = "semaphore.c";
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get semid\n");
        exit(-1);
    }
    semaphore2();
    result = pthread_create(&thread_id1, (pthread_attr_t *)NULL, my_thread1, NULL);
    result = pthread_create(&thread_id2, (pthread_attr_t *)NULL, my_thread2, NULL);
    pthread_join(thread_id1, (void **) NULL);
    pthread_join(thread_id2, (void **) NULL);
    printf("%d\n", a);
    semctl(semid, IPC_RMID, 0);
    return 0;
}

