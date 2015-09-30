#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

#define N 3

int main()
{
   int i, child_status;
   for(i = 0; i < N ; ++i)
   {
     pid_t pid = fork();
     if(pid == -1){
       printf("Error\n");
     }
     else if(pid == 0){
              sleep(i+1);
              printf("Child process %d\n", i);
           }
        
          else{
            wait(&child_status);
            printf("Child process finished %d\n", i);
            exit(0);    
          }
    }
   return 0;
}
