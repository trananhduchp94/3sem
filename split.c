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
    (*tokens)[(*tokensCount)++] = pt;

    while (pt)
    {
        pt = strtok (NULL, delimiters);
        (*tokens)[(*tokensCount)++] = pt;
    }
}


int main ()
{
    /*
     * FIXIT: Вместо чисел 300 и 150 необходимо объявить контанты
     * const int MaxStringSize = 300;
     * const int MaxDelimetersCount = 150;
     * В коде не должно быть никаких "магических" чисел.
     */
    char string[MaxStringSize] = {};
    gets(string);
    char** tokens    = (char**) malloc ( sizeof (char*) * MaxDelimetersCount);
    int tokensCount= 0;
    int i;
    Split (string, delimiters, &tokens, &tokensCount);
    
    for (i = 0; i < tokensCount; i++)
        printf ("%s\n", tokens[i]);
    
    free (tokens);
    
    /*
     * FIXIT
     * У вас в переменной tokensCount записано неверное значение.
     * Поэтому на моем компьютере программа падает с ошибкой.
     */

    return 0;
}


