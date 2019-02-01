#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2223
#define BUFFER 200
#define MAXCLI 10
#define MAXNAME 10


typedef struct {
	char name[MAXNAME];
	int ID ;
	struct sockaddr_in a_cli_addr;
	int a_cli_fd;
	int status; //0=offline, 1=online
	//char buffer[BUFFER];
}a_client;

a_client* all_cli [MAXCLI];
a_client* online_cli [MAXCLI];

static int totalCli = 0;
static int onlineCli = 0;

/**
 * print error message
 */
void error(const char *msg);

/**
 * reformat inputs
 */
void format (char* b){
	while(*b != '\0'){
		if(*b == '\r' || *b == '\n') {*b ='\0';}
		b++;
	}
}

/**
 * send message to one client
 */
void oneToOne(char* msg, int rec_id, a_client* sen){
	char buffer[BUFFER];
	int i, flag;
	flag = 1;
	memset (buffer, '\0', BUFFER); //clean buffer

	for(i = 0; i < totalCli; i++){
		if(all_cli[i]-> ID == rec_id){
			//target is offline
			if (all_cli[i]->status == 0){
				flag = 0;
				printf("OFFLINE");
				sprintf(buffer, "~User %s is offline\r\n", all_cli[i]->name);
				write (sen->a_cli_fd, buffer, BUFFER);
			}
			//target is good
			else{
				flag = 0;
				sprintf(buffer, "\r\n<Message>%s: %s\r", sen->name, msg);
				write (all_cli[i]->a_cli_fd, buffer, BUFFER);
			}
			break;
		}
	}
	//client does not exist
	if(flag){
		sprintf(buffer, "~Client %d does not exist, send FAILED\r\n", rec_id);
		write (sen->a_cli_fd, buffer, BUFFER);
	}
	return;
}


/**
 * send message to all online clients but self
 */

void oneToAllButSelf(char* msg, a_client* sen){

	char buffer[BUFFER];
	int i;

	memset (buffer, '\0', BUFFER); //clean buffer

	for(i = 0; i < totalCli; i++){
		if ((all_cli[i]->ID != sen->ID)&&(all_cli[i]->status != 0)){
			sprintf(buffer, "\r\n<NOTE>%s: %s\r", sen->name, msg);
			write (all_cli[i]->a_cli_fd, buffer, BUFFER);
		}
	}
	return;
}


void* handle_client (void* argv){
	int newmsg;
	char buffer[BUFFER];
	char buffer_temp[BUFFER];
	char buffer_name [BUFFER];
	char buffer_back [BUFFER];

	a_client* c  = (a_client*)argv;

	memset (buffer, '\0', BUFFER); //clean buffer
	memset (buffer_temp, '\0', BUFFER);//clean buffer_cpy
	memset (buffer_name, '\0', BUFFER); //clean buffer_name

	//online broadcast
	sprintf(buffer, "Client %d: %s is online now", c->ID, c->name);
	oneToAllButSelf(buffer,c);

	memset (buffer, '\0', BUFFER); //clean buffer

	while (read(c->a_cli_fd,buffer,BUFFER-1)>0){
		memset (buffer_temp, '\0', BUFFER);//clean buffer_cpy
		format(buffer);

		/*change nickname*/
		if(!memcmp(buffer,"@n ",3)){
			int j, name_flage;
			name_flage = 1;
			sprintf(buffer_temp,"%s",buffer+3);

			//test duplication of the name
			for(j = 0; j < totalCli; j++){
				if(strcmp(all_cli[j]->name, buffer_temp) == 0){
					memset (buffer_temp, '\0', BUFFER);//clean buffer_cpy
					sprintf(buffer_temp,"@Error: duplicate name, edit failed");
					name_flage = 0;
				}
			}
			//past test: good to change
			if(name_flage){
				memset (c->name, '\0', MAXNAME); //clean original name
				sprintf(c->name,"%s", buffer+3);
				sprintf(buffer_temp,"@Name updated to: %s",c->name);
				oneToAllButSelf(buffer_temp, c);
				write(c->a_cli_fd,buffer_temp,BUFFER);
			}else{
				write(c->a_cli_fd,buffer_temp,BUFFER);
			}

		}

		/*check online users*/
		else if(!memcmp(buffer,"-a",2)){
			int i, a;
			a = 0;
			for(i = 0; i < totalCli; i++){
				if(all_cli[i]->status == 1){
					online_cli[a] = all_cli[i];
					sprintf(buffer_name,"Client %d", online_cli[a]->ID);
					strcat(buffer_temp, "  ~");
					strcat(buffer_temp, buffer_name);
					strcat(buffer_temp, " : ");
					strcat(buffer_temp, online_cli[a++]->name);
					strcat(buffer_temp, "\r\n");
				}
			}

			memset (buffer, '\0', BUFFER); //clean original name
			sprintf(buffer,     //c->buffer+strlen(c->buffer),
					"-All online users:\n%s", buffer_temp);
			write(c->a_cli_fd,buffer,BUFFER);
		}

		/*disconnect client*/
		else if(!memcmp(buffer,".q",2)){
			c->status = 0; //go offline
			printf("Disconnected Client %d:%s\r\n", c->ID, c->name);
			sprintf(buffer_temp,".Disconnected: user %d:%s\r\n", c->ID, c->name);
			oneToAllButSelf(buffer_temp, c); //global notification
			write(c->a_cli_fd,buffer_temp,BUFFER);
			break;
		}

		/* Whisper message*/
		else if((buffer[2] == '~')&&(buffer[0] == '~')){

			int rec_id;
			rec_id = (int)(buffer[1] - 48);

			//read receiver and message
			strcpy(buffer_temp,buffer+3);
			printf("\nNew message received: %s\n",buffer); //server report

			//buffer_temp stores the message
			//buffer_name stores receiver name
			printf("ID: %d\n", rec_id);
			printf("message: %s\r", buffer_temp);

			memset (buffer, '\0', BUFFER); //clean buffer
			sprintf(buffer,"~Message(to Client %d): %s", rec_id,buffer_temp);

			oneToOne(buffer, rec_id, c);
			write(c->a_cli_fd,buffer,BUFFER);
		}
		/*server report*/
		else if (buffer[0]!='\0'){
			sprintf(buffer_temp,"%s: Refreshed", buffer);
			newmsg = write(c->a_cli_fd,buffer_temp,BUFFER);
			if (newmsg < 0) error("ERROR: not writings");
		}
		memset (buffer, '\0', BUFFER); //clean buffer
		memset (buffer_temp, '\0', BUFFER);//clean buffer_cpy
		memset (buffer_name, '\0', BUFFER); //clean buffer_name
		memset (buffer_back, '\0', BUFFER); //clean buffer_name

	}

	onlineCli--;
	close(c->a_cli_fd); //close current fd
	pthread_detach(pthread_self());//end this thread

	return NULL;
}

int main(int argc, char *argv[])
{
	int sock_fd, sock_bd, newsock_fd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	pthread_t new_thread;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0){error("ERROR opening socket");}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.20");//htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	sock_bd = bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (sock_bd < 0){error("ERROR: not binding");}

	//wait for arrival of a message
	if (listen(sock_fd,10) < 0){error("ERROR: not listening");}

	int id = 1;//ID starts from 1
	int index = 0;//index of all clients

	//------add new client --------
	while(1){
		//a new client arrives
		newsock_fd = accept(sock_fd,
				(struct sockaddr *) &cli_addr,
				&clilen);
		if (newsock_fd < 0){error("ERROR: not accept");}
		clilen = sizeof(cli_addr);

		totalCli++;
		onlineCli++;

		a_client* acli = (a_client* )malloc(sizeof(a_client));

		char new_char [10] = {'a','b','c','d','e','f','g','h','i','j'};
		char* new_name = (char* )malloc(sizeof(char)*MAXNAME);
		memset(new_name, '\0', MAXNAME);
		sprintf(new_name,"user%c", new_char[id-1]);
		strcpy(acli->name, new_name);
		acli->a_cli_fd = newsock_fd;
		acli->ID = id++;
		acli->a_cli_addr = cli_addr;
		acli->status = 1;

		printf("name: %s\n",acli->name );
		printf("status: %d\n", acli->status);


		//add the new client to the array
		if(index < MAXCLI){all_cli[index++] = acli;}
		else{error("ERROR: max client number reached");}

		printf("client %d connected\n", acli->ID);

		pthread_create(&new_thread, NULL, &handle_client, acli);

		free(new_name);
		sleep(1);
	}
}

void error(const char *msg){
	perror(msg);
	exit(1);
}
