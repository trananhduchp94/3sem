#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
   int i, child_status;
   for(i = 0; i < 3; ++i){
      pid_t pid = fork();
      switch(pid)
      {
         case-1: printf("Error");
                 exit(1);
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
