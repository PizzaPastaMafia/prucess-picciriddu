#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h> 
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int globoSock, row, col;

void initGraphics(){
	initscr();
	getmaxyx(stdscr,row,col); 
}

char *chatLine(){
	char *buff;
	int n = 0;

	mvprintw(LINES - 2, 0, "Enter the string: ");

	while ((buff[n++] = getch()) != '\n'){
		mvprintw(LINES - 2, 18+n, "%s", buff[n]);
	}

	return buff;
}

void *entry_point(void *value){
	int sockfd = globoSock;
	char buff[MAX];
	for (;;) {
		bzero(buff, sizeof(buff));
		if(read(globoSock, buff, sizeof(buff)) > 0){
			//printf("From Server : %s", buff);
		}	
		if ((strncmp(buff, "exit", 4)) == 0) {
			//printf("Client Exit...\n");
			break;
		}
	}

	return NULL;
}

void func(int sockfd){
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		strcpy(buff, chatLine());
		//printf("Enter the string : ");
		//n = 0;
		//while ((buff[n++] = getchar()) != '\n');
		write(sockfd, buff, sizeof(buff));
		
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	pthread_t listen;

	globoSock = sockfd;

	initGraphics();


	pthread_create(&listen, NULL, entry_point, "listen");

	func(sockfd);

	pthread_join(listen, NULL);


	// close the socket
	close(sockfd);
}
