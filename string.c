#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void GenerateString(int n, char *string)
{
   int i;
   string[0] = 'a';
   /*
    * FIXIT:
    * Замечание по форматированию кода. Вокруг любых бинарных операторов ставьте пробелы: 
    *   i=1; надо заменить на i = 1;
    *   int k = (1<<i); заменить на int k = (1 << i);
    * и так далее.
    * Следующая строка должна быть написана так:
    * for(i = 1; i <= n; i++)
    * Обратите внимание на расстановку пробелов и делайте аналогично.
    */
   for(i=1;i<=n;i++)
   {
      int k = (1<<i);
      string[k-1] = 'a' + i;
      memcpy(string+k, string, k-1);
   }
}

int main()
{
  /*
   * Нужно добавить пробел после запятой:
   * int n, c;
   */
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
   /*
    * FIXIT: программа падает с ошибкой
    * *** Error in `./a.out': free(): invalid next size (fast): 0x000000000249a010 ***
      Aborted (core dumped)
      при n >= 4.
    */
   return 0;
}
