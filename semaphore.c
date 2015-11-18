#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX 100000

/*
 * FIXIT:
 * Код примерно правильный, но есть две проблемы. Постарайтесь вникнуть в проблемы и осознать их.
 * 1) для работы с семафорами используется переменная struct sembuf mybuf;
 * которая общая для всех потоков. У вас была проблема, что несколько потоков работают с одной переменной. Вы её попытались решить, но 
 * создали аналогичную проблему.
 * 
 * 2) благодаря чему вы гарантируете, что к общей переменной а в каждый момент времени будет иметь доступ только один поток?
 * Вы будто бы "помещаете" код 
 * (*a)++;
 * в некоторую специальную секцию, в каждый момент в которой может находиться максимум 1 поток.
 * Изначально вы семафор инициализируете единицей - которая означает, что сейчас свободно одно место для работы со строкой (*a)++;
 * После того, как один из процессов зайдёт в эту секцию, он должен выполнить операцию D(S, -1), что переведет семафор в состояние S=0.
 * Если второй поток попытается выполнить ту же операцию, то он "повиснет" и будет ждать, пока первый поток не вернет его в состояние S=1.
 * 
 * У вас операции -1 и +1 перепутаны местами.
 */

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
       // printf("a = %d\n", *a);
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

    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
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

