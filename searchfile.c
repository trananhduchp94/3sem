#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
   DIR *dp;
   struct dirent *dirp;

   if (argc != 3)
   {
      printf("Error");
      exit(0);
   }

   if ((dp = opendir(argv[1])) == NULL)
   {
      printf("can't open %s\n", argv[1]);
      exit(1);
   }
   int i = 0;
   while ((dirp = readdir(dp)) != NULL)
   {
     if(strcmp(dirp->d_name,argv[2]) == 0) 
    {
     i++;
     printf("%s\n", dirp->d_name);
    }
   }
   if ( i == 0) printf("File is not found\n"); 
   closedir(dp);
   exit(0);
}
