#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *delimiters = ",. ?!():-/;#$%& \t\n@^*";

void Split(char* string,char* delimiters, char*** tokens, int* tokensCount);
{
    *tokensCount = 0;

    char* pt = NULL;

    pt = strtok (string, delimiters);
    (*tokens)[(*tokensCount)++] = pt;

    while (temp_str != NULL)
    {
        pt = strtok (NULL, delimiters);
        (*tokens)[(*tokensCount)++] = pt;
    }
}


int main ()
{
    char string[256] = {};
    gets(string);

    char** tokens    = (char**) malloc ( sizeof (char*) * 150);
    int tokensCount= 0;
    int i;
    Split (string, delimiters, &tokens, &tokensCount);

    for (i = 0; i < tokensCount; i++)
        printf ("%s\n", tokens[i]);

    free (tokens);

    return 0;
}


