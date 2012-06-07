#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "login.h"
#include "common.h"
#include "mainWindow.h"

void get_login_info(GtkWidget *widget,gpointer data);
void login_connect(const gchar *uname,const gchar *pword);

void login_window_init(){

	/* get graphics from login.glade */
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder,"login.glade",NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder,"window1"));
	gtk_signal_connect(GTK_OBJECT(window),"destroy", \ 
			G_CALLBACK(gtk_main_quit),NULL);

	button = GTK_WIDGET(gtk_builder_get_object(builder,"quitbutton"));
	gtk_signal_connect(GTK_OBJECT(button),"clicked", \
			G_CALLBACK(gtk_main_quit),NULL);

	usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder,"unentry"));
	passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder,"pwentry"));

	label = GTK_WIDGET(gtk_builder_get_object(builder,"inform"));

	button = GTK_WIDGET(gtk_builder_get_object(builder,"loginbutton"));
	gtk_signal_connect(GTK_OBJECT(button),"clicked", \
			G_CALLBACK(get_login_info),NULL);

}

void login_connect(const char *uname,const char *pword){

	struct sockaddr_in addr;
	struct LoginPack *loginmsg;
	short int flag = 0;


	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd < 0){
		printf("create socket error!\n");
		exit(0);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	connect_fd = connect(socket_fd, (struct sockaddr *)&addr, \ 
			sizeof(struct sockaddr));
	
	if(connect_fd < 0){
		printf("connect error!\n");
		exit(0);
	}
	/* send username and password */
	loginmsg = (struct LoginPack *)malloc(sizeof(struct LoginPack));
	memset(loginmsg, 0, sizeof(struct LoginPack));
	strcpy(loginmsg->username, uname);
	strcpy(loginmsg->password, pword);
	printf("send u=%s, p=%s\n", loginmsg->username, loginmsg->password);
	send(socket_fd, loginmsg, sizeof(struct LoginPack), 0);

	/* received flag */
	recv(socket_fd, &flag, 2,0);
	flag = ntohs(flag);
	printf("received flag=%d\n", flag);
	if( 0 == flag){             /* 0 is login success, -1 is failed */
		
		gtk_label_set_text(GTK_LABEL(label),"loading...");
		
		gtk_widget_hide(window);
		mainWindow();

	}else{
		gtk_label_set_text(GTK_LABEL(label), \ 
				"loading failed,username or password error!");
		gtk_entry_set_text(GTK_ENTRY(passwordEntry),"");
	}

	//close(socket_fd);

}

/*
 * Name: get_login_info
 * Description: get username and password from ui from, 
 * 		call login_connect() function.
 *
 */ 
void get_login_info(GtkWidget *widget,gpointer data){
	const gchar *uname,*pword;

	/* get username text  */
	uname = gtk_entry_get_text(GTK_ENTRY(usernameEntry));
	pword = gtk_entry_get_text(GTK_ENTRY(passwordEntry));
	printf("username=%s\npassword=%s\n",uname,pword);
	login_connect(uname,pword);
		
}
