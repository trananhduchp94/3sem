#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MaxStringSize = 300;
const int MaxDelimetersCount = 150;

char *delimiters = ",. ?!():-/;#$%& \t\n@^*";

void Split(char* string,char* delimiters, char*** tokens, int* tokensCount)
{
    *tokensCount = 0;
    char* pt = NULL;
    pt = strtok (string, delimiters);
    while (pt)
    {
        (*tokens)[(*tokensCount)] = pt;
        (*tokensCount)++;
        pt = strtok (NULL, delimiters);
    }
}

int main ()
{
    char *string = (char*)malloc(sizeof(char) * MaxStringSize);
    gets(string);
    char** tokens = (char**) malloc ( sizeof (char*) * MaxDelimetersCount);
    int tokensCount = 0;
    int i;
    Split (string, delimiters, &tokens, &tokensCount);
    printf("tokensCount = %d\n",tokensCount);
    for (i = 0; i < tokensCount; i++)
        printf ("%s\n", tokens[i]);
    
    free (tokens);

    return 0;
}


