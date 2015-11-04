/*Есть большое количество экспериментальных 
данных (нужно самим сгенерировать)
вещественных чисел. Необходимо с 
помощью 
N 
потоков найти их среднее и дисперсию*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#define Elements 1000000
#define N 1

long double *array;

long double sum[N];
long double disp[N];
long double average, dispersion;

/*
 * FIXIT:
 * Дык, ваша программа "падает" с ошибкой segmentation fault.
 * Вероятно, это связано с проблемой, о которой написал ниже.
 */

struct Task
{
  int b, a, index;
};

struct Thread
{
  pthread_t id;
};

void* suma(void* task) 
{
    long int i;
    
    long double sum1 = 0;
    pthread_t my_thread_id;
    for(i = ((struct Task*)task) -> a; i < ((struct Task*)task) -> b; i++)
    {
        sum1 = sum1 + array[i]; 
    } 
    sum[((struct Task*)task)->index] = sum1;
}

void* dispersion1(void* task) 
{
    long int i;
    long double sum2 = 0;
    pthread_t my_thread_id;
    for (i = ((struct Task*)task) -> a; i < ((struct Task*)task) -> b; i++)
    {
        sum2 = sum2 + (array[i] - average) * (array[i] - average);
    } 
    disp[((struct Task*)task)->index] = sum2;
    return NULL;
}


int main()
{
    int i;
    /*
     * FIXIT:
     * Вы на стеке пытаетесь выделить огромный кусок памяти. 
     * Такие большие куски можно выделить только в куче с помощью malloc`а.
     */
    struct Task tasks[Elements];
    struct Thread threads[Elements];
    
    for(i = 0; i < N; i++)
    {
        tasks[i].index = i;
        tasks[i].a =  i * Elements / N;
        tasks[i].b = (i + 1) * Elements / N;
    }

    if (Elements % N != 0)
    {
        tasks[N - 1].b = Elements;
    }

    array = (long double *)malloc(Elements * sizeof(long double));
    
    for (i = 0; i < Elements; i++)
    array[i] = 2 + rand() % 2;  
    
   
    
    
    
    
    for(i = 0; i < N; i++)
    {
        sum[i] = pthread_create(&(threads[i].id) ,
                                     (pthread_attr_t *)NULL ,
                                      suma,
                                      &tasks[i]);
    }  
     
    for(i = 0; i < N; i++)
    {
        pthread_join(threads[i].id , (void **) NULL);
    }
      
   
    for (i = 0; i < N; i++)
    {
       average = average + sum[i];
    } 
    
    average = average / Elements;
    
    for(i = 0; i < N; i++)
    {
        disp[i] = pthread_create(&(threads[i].id) ,
                                     (pthread_attr_t *)NULL ,
                                      dispersion1 ,
                                      &tasks[i]);
    }  
     
    for(i = 0; i < N; i++)
    {
        pthread_join(threads[i].id , (void **) NULL);
    }
   
    
    for (i = 0; i < N - 1; i++)
    {
        dispersion = dispersion + disp[i];
    }
    
    
    dispersion = dispersion / Elements;
    
   
    
    printf("Average = %Lf\n" , average);
    printf("Dispersion = %Lf\n" , dispersion);
    return 0;
}

