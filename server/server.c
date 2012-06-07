#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT 32000
#define SERVER_IP   "127.0.0.1"
#define SERVER_BACK 10
#define BUF_SIZE    128

void *sndmsg(void *arg);
void clients_thread();
int authentication_user();
int recv_name_pwd(int client_fd);

struct ChatMsg{
	int who;
	int Flag;
	char Msg[50];
};
struct ChatMsg chatmessage;
struct LoginPack{
	int flag;
	char username[20];
	char password[20];
};
struct threadflag{
	int clientdes;
	int userflag;
};


/* save received username and password */
char unamebuf[BUF_SIZE];
char pwordbuf[BUF_SIZE];
char userMsg[BUF_SIZE];
struct LoginPack *loginmsg;
int loginFlag = 0;

pthread_t thread,sndthread;
struct sockaddr_in addr;
int chatmsglen;

int sockfd,bind_fd,client_fd,recv_fd;

pthread_mutex_t mutex;

int main(int argc,char **argv){
	struct sockaddr_in client;
	unsigned int addrlen;
	
	chatmsglen = sizeof(chatmessage);
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		printf("create socket error!\n");
		exit(1);
	}
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	bind_fd = bind(sockfd,(struct sockaddr*)&addr, sizeof(struct sockaddr));
	if(bind_fd < 0){
		printf("Server Bind error!\n");
		exit(1);
	}

	listen(sockfd, SERVER_BACK);
	printf("server ready!\n");
	while(1){
		addrlen = sizeof(struct sockaddr_in);	
		client_fd = accept(sockfd, (struct sockaddr*)&addr, &addrlen);	
		
		if(client_fd == -1){
			printf("accept error!\n");
			return 0;
		}
							
		if(pthread_create(&thread, NULL, (void *)clients_thread, NULL) < 0){
			printf("pthread create error!\n");
		}
	}
	
	close(sockfd);

}

int recv_name_pwd(int client_fd){
	/* receive username */
	loginmsg = (struct LoginPack *)malloc(sizeof(struct LoginPack));
	memset(loginmsg, 0 , sizeof(struct LoginPack));
	recv_fd = recv(client_fd, loginmsg, sizeof(struct LoginPack), 0);
	if(recv <= 0){
		printf("receive error!\n");
		return -1;
	}
	
	strcpy(unamebuf, loginmsg->username);
	strcpy(unamebuf, loginmsg->password);
	return 0;
}

void clients_thread(){
	short int flag;

	/* recv name and pwd */
	if(recv_name_pwd(client_fd) < 0){
		pthread_exit("client_fd exit:recv_name_pwd error!\n");
	}
						
	if(authentication_user() == 0){
		/* login fail */
		flag = -1;
		flag = htons(flag);
		send(client_fd, &flag, 2, 0);
		pthread_exit("name or pwd error!\n");
	}else{
		/* login success */
		flag = 0;
		flag = htons(flag);
		send(client_fd, &flag,2,0);
		printf("client IP:%s\n",inet_ntoa(addr.sin_addr));
	}
	
	while(1){
#if 0
		fd_set set;
		FD_ZERO(set);
		FD_SET(client_fd, set);
		select(client_fd + 1, set, set, NULL, NULL);
#endif
		if(chatmessage.Flag ==200){
			close(client_fd);
			pthread_exit(NULL);
		}
		//pthread_mutex_lock(&mutex);
		recv(client_fd,userMsg,BUF_SIZE,0);
		memcpy(&chatmessage,userMsg,chatmsglen);
					
		printf("who =%d\tstatusflag=%d\tmsg=%s\n",chatmessage.who,chatmessage.Flag,chatmessage.Msg);
		
	}
}

void *sndmsg(void *arg){

	struct threadflag *argg;
	argg = (struct threadflag *)arg;
	printf("clientfd = %d\t userflag = %d\n",(*argg).clientdes,(*argg).userflag);
	while(1){
	
		if(chatmessage.who == (*argg).userflag){
			send((*argg).clientdes,chatmessage.Msg,50,0);
			//printf("i send to %d ,who = %d\n",atoi(unamebuf),chatmessage.who);
		}
	
		sleep(1);
	}

}

/*
 * Fun name : authentication_user
 * Description: verify user name and password,
 * 	and print result of verify.
 */ 
int authentication_user(){
	sqlite3 *db;
	char *ErrMsg=0;
	char sql[200];
	int rc;
	int i;
	int nrow=0,ncolumn=0;
	char **Result;

	rc = sqlite3_open("qq.db",&db);
	if(SQLITE_ERROR == rc){
		printf("can't open the database!\n");
		exit(1);
	}
	sprintf(sql, "select * from basicinfo where username='%s' and password='%s';", loginmsg->username, loginmsg->password);
	sqlite3_get_table(db,sql,&Result,&nrow,&ncolumn,&ErrMsg);
	printf("row=%d column=%d\n",nrow,ncolumn);
	printf("result of query is\n");

	for(i=2;i<(nrow+1)*ncolumn;i++){
		printf("Result[%d]=%s\n",i,Result[i]);
	}
	if(nrow==1){
		printf("***user=%s login successful!**\n",unamebuf);
		return 1;
	}else{
		printf("~~~user=%s login failed!~~~\n",unamebuf);
		return 0;
	}
	sqlite3_free_table(Result);
	sqlite3_close(db);
}

