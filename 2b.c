#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdlib.h>

#define N 3

/*
 * У вас неверно создаются процессы: нужно, чтобы процесс создал одного ребёнка, этот ребенок тоже создал одного ребёнка 
 * и т.д.
 * У вас же родительский процесс создал одного ребенка, дождался его завершения, потом он же создал ещё одного и так далее.
 */

int main()
{
   int i, child_status;
   for(i = 0; i < N; ++i) {
      pid_t pid = fork();
      switch(pid)
      {
         case -1: printf("Error");
                  exit(1);
                  break;
         case 0: printf("Child process %d\n", i);
                 sleep(i + 1);
                 exit(0);
                 break;
         default:
                 wait(&child_status);  
                 printf("Child process terminated : PID = %d\n", pid);     
                 printf("Parent process %d\n", i);
                 break;
      }
   }
   return 0;
}
