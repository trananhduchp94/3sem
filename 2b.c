#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

#define N 5

/*
 * Изначально у вас всё нормально, но т.к. не выводилась информация про id и parent_id, то сам немного запутался.
 * Сейчас всё хорошо.
 */

int main()
{
   int i, child_status, child_pid;
   for(i = 0; i < N; ++i)
   {
     int pid;
     pid = fork();
     switch (pid) {
       case -1:
         printf("Error\n");
       break;
       case  0:
         //sleep(i+1);
         printf("Child process - %d. Parent - %d\n", getpid(), getppid());
       break;
       default:
         child_pid = wait(&child_status);
         printf("Child process finished %d\n", child_pid);
         exit(0);    
       break;
     }
   }
   return 0;
}
