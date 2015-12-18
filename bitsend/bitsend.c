#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h> 

#define STRING_TO_TRANSFER "Hello, world!"

char* result;
int numberOfBit = 0, numberOfCharacter = 0; 
pid_t ppid, cpid;

void waitHandler(int nsig) {
    printf("get bit \n");
}

void myHandler(int nsig) {
    int bit = 0;
    bit = (nsig == SIGUSR1) ? 1 : 0;
    if (bit == 1) {
        result[numberOfCharacter] += 1 << (7 - numberOfBit);
    }
    numberOfBit++;
    if (numberOfBit == 8) {
        numberOfCharacter++;
        numberOfBit = 0;    
        if (numberOfCharacter == strlen(STRING_TO_TRANSFER)) {
            result[numberOfCharacter] = '\0';
            printf("result : %s\n", result);
	    /*
	     * В этот момент можно было бы убить процессы.
	     * У вас же они продолжают работать.
	     */
        }
    }
    kill(cpid, SIGTERM);
} 

int main() {
    char* original = STRING_TO_TRANSFER;
    pid_t pid;
    signal(SIGUSR1, myHandler);
    signal(SIGUSR2, myHandler);
    signal(SIGTERM, waitHandler);
    result = (char*) malloc(sizeof(char*) * strlen(original));
    ppid = getpid();
    pid = fork();
    if (pid != 0) {
        while(1);
    }
    else { 
        while(1) {
            cpid = getpid();
            int i;
            char* tmp = original;
            for (; *tmp != 0; ++tmp) {
                for (i = 7; i >= 0; --i) {
		    /*
		     * Тогда уж
		     * kill(ppid, (*tmp & 1 << i) ? SIGUSR1 : SIGUSR2);
		     */
                    (*tmp & 1 << i) ? kill(ppid, SIGUSR1) : kill(ppid, SIGUSR2);
                    pause();
                }
            }
        } 
    }
    free(result);
    return 0;
}
