#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 51000
#define MAX_MESSAGE_LENGTH 1024

int main() {
	int sockfd;
	int n, i;
	char sendLine[MAX_MESSAGE_LENGTH], receiveLine[MAX_MESSAGE_LENGTH];
	struct sockaddr_in serverAddress;
	bzero(sendLine, MAX_MESSAGE_LENGTH);
	bzero(receiveLine, MAX_MESSAGE_LENGTH);
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror(NULL);
		exit(1);
	}
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	if (inet_aton(SERVER_IP, &serverAddress.sin_addr) == 0) {
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}
	if (connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	while (1) {
		printf("What operation do you want to do?\n");
		fflush(stdin);
		fgets(sendLine, MAX_MESSAGE_LENGTH, stdin);
		if (n = write(sockfd, sendLine, strlen(sendLine) + 1) < 0) {
			perror("Can't write\n");
			close(sockfd);
			exit(1);
		}
		if (n = read(sockfd, receiveLine, MAX_MESSAGE_LENGTH - 1) < 0) {
			perror("Can't read\n");
			close(sockfd);
			exit(1);
		}
		printf("%s\n", receiveLine);
	}
	close(sockfd);
	return(0);
}
