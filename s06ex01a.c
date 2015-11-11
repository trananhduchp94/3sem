/*написать две программы, осуществляющие взаимодействие через разделяемую память. Первая программа должна создать сегмент разделяемой памяти и скопировать туда свой собственный код
исходный код. Вторая должна брать оттуда этот текст, печатать его на экране и удалять сегмент разделяемой памяти из системы.
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define size 11

/*
 * Хорошо. Это упражнение засчитано.
 * В данном семинаре были необходимо также сделать упражнение с семафорами, где два потока в цикле инкрементируют одну и ту же 
 * глобальную переменную.
 */

int main ()
{
   char    *array;
   int     shmid;
   int     new = 1 ;
   const char pathname[] = "in.txt";
   key_t   key;

   if((key = ftok(pathname , 0)) < 0){ 
     printf("Can\'t generate key\n");
     exit(-1);
   }
   if((shmid = shmget(key, size * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
   { 
     if(errno != EEXIST)
     {
            printf("Can't create shared memory\n");
            exit(-1);
     }else 
      {
        if((shmid = shmget(key, size * sizeof(char), 0)) < 0)
         {
            printf("Can't find shared memory\n");
            exit(-1);
         }
         new = 0;
      }
   }
   if((array = (char*)shmat(shmid, NULL, 0)) == (char*)(-1))
   {
      printf("Can't attach shared memory\n");
      exit(-1);
   }
   
   int fd = open(pathname, O_RDONLY);
   char buffer[31];
   int number, position = 0;
  
   while(number = read(fd, buffer, 30))
   {
      memcpy((void*)(array + position), (void*)buffer, number);
      position += number;
   }
   
   if(shmdt(array) < 0)
   {
      printf("Can't detach shared memory\n");
      exit(-1);
   }
   return 0;
}













