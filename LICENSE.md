#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

void GenerateString(int n, char *string)
{
   int i;
   string[0] = 'a';
  
    for(i = 1 ; i <= n; i++)
   {
  	int k = pow(2,i);
    string[k-1] = 'a' + i;
    memcpy(string+k,string,k-1);
   }
  
  
}

int main ()
{
	int n,c;
	scanf("%d",&n);
	c= pow(2,n) + 1;
	char *string;
	string=(char*) malloc(c);
	if(n==0) printf("a");
    else {
       GenerateString(n,string);
	printf("%s ", string);
          }
	free(string);
	return 0;
}
