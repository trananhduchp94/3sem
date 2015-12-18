#include <sys/types.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_PORT 51000
#define OPERATION_LIMIT 1e6
#define CREDIT_LIMIT 1e6
#define INITIAL_BALANCE 1e10
#define MAX_MESSAGE_LENGTH 1024
#define MAX_NUMBER_LENGTH 128

double bankBalance;
int sockfd, newsockfd, semid;

/*
 * Засчитано, но ...
 * 1. Мы же с вами говорили уже, что вместо semaphore1 и semaphore2 лучше написать нечто более
 * осмысленное вроде lockSemaphore и unlockSemaphore.
 * 2. listen(... , 5) здесь 5 вполне можно также вынести в отдельную константу
 * 5 означает, что не более 5 клиентов могут одновременно общаться по сети с сервером.
 */

void semaphore1() {
    struct sembuf mybuf;
    mybuf.sem_op = -1;
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    semop(semid, &mybuf, 1);
}

void semaphore2() {
    struct sembuf mybuf;
    mybuf.sem_op = 1;
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    semop(semid, &mybuf, 1);
}

void* bankOperation(void* arg) {
	int* newsockfd = (int*)arg;
	char line[MAX_MESSAGE_LENGTH];
	char *line_ptr = line;
	char tmp[MAX_NUMBER_LENGTH];
	int n;
	double clientBalance = 0;
	while (n = read(*newsockfd, line, MAX_MESSAGE_LENGTH - 1) > 0) {
		double currentOperation = atof(line);
		bzero(line, MAX_MESSAGE_LENGTH);
		semaphore1();
		if (currentOperation < OPERATION_LIMIT && currentOperation > 0) {
			clientBalance += currentOperation;
			bankBalance += currentOperation;
			strcpy(line_ptr, "");
			strcat(line_ptr, "You have put ");
			sprintf(tmp, "%.2f", currentOperation);
			strcat(line_ptr, tmp);
			strcat(line_ptr, " $. You current balance is ");
			sprintf(tmp, "%.2f", clientBalance);
			strcat(line_ptr, tmp);
			strcat(line_ptr, "$. Thank you for using the services of our bank.");
		}
		else {
			if (currentOperation * (-1) < OPERATION_LIMIT && bankBalance > 0 && (currentOperation + clientBalance) < CREDIT_LIMIT) {
				clientBalance += currentOperation;
				bankBalance += currentOperation;
				strcpy(line_ptr, "");
				strcat(line_ptr, "You have taken ");
				sprintf(tmp, "%.2f", currentOperation);
				strcat(line_ptr, tmp);
				strcat(line_ptr, " $. You current balance is ");
				sprintf(tmp, "%.2f", clientBalance);
				strcat(line_ptr, tmp);
				strcat(line_ptr, "$. Thank you for using the services of our bank.");
			}

			else {
				strcpy(line_ptr, "");
				strcat(line_ptr, "Sorry, but you can't take money from our bank. ");
				strcat(line_ptr, " $. You current balance is ");
				sprintf(tmp, "%.2f", clientBalance);
				strcat(line_ptr, tmp);
				strcat(line_ptr, "$. Thank you for using the services of our bank.");
			}
		}
			
		semaphore2();
		printf("Current bank balance: %.2f\n", bankBalance);
		if ((n = write(*newsockfd, line_ptr, strlen(line_ptr) + 1)) < 0) {
			perror(NULL);
			free(newsockfd);
			close(*newsockfd);
			exit(1);
		}
	}

	close(*newsockfd);
	free(newsockfd);
	return NULL;
}

int main() {
	char line[MAX_MESSAGE_LENGTH];
	pthread_t thread_id;
	int clilen;
	struct sockaddr_in serverAddress, clientAddress;
	int n, result;
	key_t key;
	bzero(line, MAX_MESSAGE_LENGTH);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror(NULL);
		exit(1);
	}
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	if (listen(sockfd, 5) < 0) {
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	bankBalance = INITIAL_BALANCE;
	char* pathname = "server.c";
	if ((key = ftok(pathname, 0)) < 0) {
		printf("Can't generate key\n");
		exit(-1);
	}
	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
		printf("Can't get semid\n");
		exit(-1);
	}
	semaphore2();
	while(1) {
		clilen = sizeof(clientAddress);
		int* newsockfd = (int*)malloc(sizeof(int));
		if ((*newsockfd = accept(sockfd, (struct sockaddr*) &clientAddress, &clilen)) < 0) {
			perror(NULL);
			close(sockfd);
			exit(-1);
		}
		result = pthread_create(&thread_id, (pthread_attr_t*)NULL, bankOperation, (void*)newsockfd);
	}
	close(sockfd);
	semctl(semid, IPC_RMID, 0);
	return 0;

}
