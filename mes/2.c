
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>

#define LAST_MESSAGE 255 
#define MAX_LENGTH_STRING 200
int main()
{
    int msqid; 

    char pathname[] = "mes"; 
    key_t key; 

    int i,len,len2; 



    struct mymsgbuf
  {
    long mtype;
    char mtext[81];
  }mybuf;

    struct mymsgbuf2
  {
    long mtype;
    char mtext[81];
  } mybuf2;




   if((key = ftok(pathname,0)) < 0){
     printf("Can\'t generate key\n");
     exit(-1);
   }


   if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
   }


   int maxlen;
   pid_t pid = fork();
   if(pid == 0)
   { 
     while(1)
     {
  

        mybuf.mtype = 2;
        fgets(mybuf.mtext, 81, stdin);
        len = strlen(mybuf.mtext)+1;



        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
        {
          printf("Can\'t send message to queue\n");
          msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
          exit(-1);
        }
     }
   }
   else 
   {
      while(1)
       {    
           maxlen = 81;

           if(( len2 = msgrcv(msqid, (struct msgbuf *) &mybuf2, maxlen, 1, 0)) < 0)
           {
              printf("Can\'t receive message from queue\n");
              exit(-1);
           }
           printf("%d : %s",mybuf2.mtype, mybuf2.mtext);
        }
   }


   mybuf.mtype = LAST_MESSAGE;
   len = 0;

   if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
   }

   return 0;
} 
