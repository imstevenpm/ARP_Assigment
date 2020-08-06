// 2019 ARP Assignment V1.0
// Steven Palma Morera - S4882385
// 2019 - 02 - 24

// This script contains the development
// of the Gn process

// Includes
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include<sys/wait.h>
#include <math.h>
// Port number for the socket
//#define PORT 8086

struct token_with_time{

	double token;
	double time1;
	struct timeval arrival_time;
};

// Main of the program
int main(int argc, char const *argv[]){

	int e;

	// Read values from config file
	// Variables for reading
	FILE * fp;
  char * line = NULL;
  size_t len = 0;

	// Variables read
	const char *ipuser= NULL;
	int port2 = 0;
	float refv = 0;

	// Open config file
	fp = fopen("/home/imstevenpm/EMARO_1ST_SEMESTER/ARP/ASSIGMENT/arp2/Config_StevenPalma.config", "r");
	if (fp==NULL){
		 perror("open");
		 return -1;
	 }

	 // Read first line (refv)
	 if ((e=getline(&line,&len,fp))!=-1){
			 refv=atof(line);
	 }

	 // Read second line (port2)
	 if ((e=getline(&line,&len,fp))!=-1){
			 port2=atoi(line);
	 }

	 // Read third line (delay)
	 if ((e=getline(&line,&len,fp))!=-1){
			 ipuser=line;
	 }

	 // Closes config file
	 fclose(fp);

	// File descriptor for nammed pipe
	int fd;

	// Creating nammed pipe for Pn+1
	char *myfifo2= "/tmp/yourpipe";
	e=mkfifo(myfifo2, S_IRUSR | S_IWUSR);
	// if (e<0){
	// 	perror("mkfifo");
	// 	return -1;
	// }

	// Variables for socket connection
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[255];

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		return -1;
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
		perror("setsockopt");
		return -1;
	}

	// Set address of socket
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port2 );

	// Attaching socket to the port 8084
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
		perror("bind");
		return -1;
	}

	// Listen to the socket
	if (listen(server_fd, 3) < 0){
		perror("listen");
		return -1;
	}

	char *myfifo1= "/tmp/mypipe2";
	mkfifo(myfifo1, S_IRUSR | S_IWUSR);

	struct token_with_time msg_to_send;


	sleep(10);

	msg_to_send.token=0;
	gettimeofday( &msg_to_send.arrival_time,NULL);

	char phrase[100];
	sprintf(phrase,"%f\n%ld.%06ld\n",msg_to_send.token,msg_to_send.arrival_time.tv_sec,msg_to_send.arrival_time.tv_usec);
	//printf("Initial message:\n%s",phrase);
	//fflush(stdout);

	int fd1;

	// G is constantly waiting for a new message from the socket
	while (1){


		fd1 = open ( "/tmp/mypipe2", O_WRONLY);
		write (fd1, &phrase, strlen ( phrase)+1);
		close (fd1);

		printf("(G): I sent>\n%s",phrase);
		fflush(stdout);



		// Waits the connection of the other socket / This call blocks indefinitely
		// Accepts request from client

		printf("(G): Waiting for a reply\n");
		fflush(stdout);

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
			perror("accept");
			return -1;
		}

		printf("(G): Reply accepted\n");
		fflush(stdout);

		// Read data from socket
		e= read(new_socket, buffer, sizeof(buffer));
		if (e<0){
			perror("read");
			return -1;
		}

		printf("(G): I read>\n%s",buffer);
		fflush(stdout);

		// Open nammed pipe
		//fd = open ("/tmp/yourpipe", O_WRONLY | O_NONBLOCK);
		//if (fd<0){
			//perror("open");
			//return -1;
		//}

		// Write data from socket to nammed pipe
		//e= write (fd, buffer, strlen (buffer)+1 );
		//if (e<0){
			//perror("write");
			//return -1;
		//}

		//printf("Wrote>\n %s\n",buffer);

		// Close nammed pipe
		//e= close (fd);
		//if (e<0){
			//perror("close");
			//return -1;
		//}

		// Splitting the char in order to save both numbers in different variables
		char *p;
		p = strtok (buffer,"\n");

		// Saving the first number from the data message (token) into the token1 attribute of the struct
		msg_to_send.token=atof(p);
		while (p!= NULL){

			// Saving the second number from the data message (token) into the token1 attribute of the struct
			msg_to_send.time1=atof(p);
			p = strtok (NULL, "\n");
		}

		sprintf(phrase,"%f\n%f\n",msg_to_send.token,msg_to_send.time1);

	}

	// Close sockets
	e=close(server_fd);
	if(e<0){
		perror("close");
	}

	e=close(new_socket);
	if(e<0){
		perror("close");
	}

	return 0;
}
