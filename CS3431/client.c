#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER 200
#define PORT 2223


void error(const char *msg);

int main(int argc, char *argv[])
{
	int sock_fd, sock_cn, newmsg;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer_read[BUFFER];
	char buffer_print[BUFFER];
	char buffer_write[BUFFER];


	//initialize and get socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);//file descriptor for client
	if (sock_fd < 0){ error("ERROR: opening socket");}
	else{printf("SUCCESS: connected to server\n");}

	//initialize server address with 0
	bzero(&serv_addr, sizeof(serv_addr));

	//assign server address struct
	serv_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.20", &serv_addr.sin_addr.s_addr );
	serv_addr.sin_port = htons(PORT);

	//connect client with server
	sock_cn = connect(sock_fd,
			(struct sockaddr *) &serv_addr, //server get client addr here
			sizeof(serv_addr));
	//check if connected or not
	if (sock_cn < 0) {error("ERROR: not yet connected");}

	//------start send and receive------
	do{
		memset (buffer_read, '\0', BUFFER);
		memset (buffer_write, '\0', BUFFER);

		/*prompt*/
		printf( "=================================================\n"
				" ~ClientNum~message   private message to a user;\n"
				" anybotton            check message;\n"
				" @n newname           change nickname;\n"
				" -a                   check all online users;\n"
				" .q                   disconnect.\n"
				"=================================================\n");
		printf(">>Enter your action:");

		/*write*/
		fgets(buffer_write,BUFFER-1,stdin);

		newmsg = write(sock_fd,buffer_write,BUFFER);
		if (newmsg < 0){error("ERROR: writing failed");}

		if(!memcmp(buffer_write,".q",2)){break;}

		/*read*/
		do{
			memset (buffer_read, '\0', BUFFER);

			read(sock_fd,buffer_read,BUFFER);
			if (newmsg < 0){error("ERROR: reading fail");}

			printf("%s\n",buffer_read);


		}while(buffer_read[0]!=buffer_write[0]);

	}while(1);

	close(sock_fd);//disconnect from server
	return 0;
}

void error(const char *msg)
{
	perror(msg);
	exit(0);
}
