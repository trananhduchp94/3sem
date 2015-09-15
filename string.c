#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void GenerateString(int n, char *string)
{
   int i;
   string[0] = 'a';
   for(i=1;i<=n;i++)
   {
      int k = (1<<i);
      string[k-1] = 'a' + i;
      memcpy(string+k, string, k-1);
   }
}

int main()
{
   int n,c;
   scanf("%d", &n);
   c= (1<<n) + 1;
   char *string;
   string = (char*)malloc(c); 
   if(n == 0)
   {
      printf("a\n");
   }
   else 
   {
      GenerateString(n, string);
      printf("%s\n",string);
   }
   free(string);
   return 0;
}
