#include "common.h"
#include "chatWindow.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

static void recv_msg(void *s);

void chatInit(){
	//struct ChatMsg chatmessage;
	pthread_t recv_thread;
	
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder,"chatWindow.glade",NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder,"window1"));
	
	button = GTK_WIDGET(gtk_builder_get_object(builder,"button1"));
	gtk_signal_connect(GTK_OBJECT(button),"clicked", \ 
			G_CALLBACK(chat_window_close),NULL);

	textview1 = GTK_WIDGET(gtk_builder_get_object(builder,"textview1"));
	textview2 = GTK_WIDGET(gtk_builder_get_object(builder,"textview2"));

	button = GTK_WIDGET(gtk_builder_get_object(builder,"button2"));
	gtk_signal_connect(GTK_OBJECT(button),"clicked", \ 
			G_CALLBACK(sendMsg),NULL);
	
	gtk_widget_show_all(window);
	pthread_create(&recv_thread,NULL, (void *)recv_msg,(void *)socket_fd);

	/*
	if(fork() == 0){
		int flag = 10;
		while(flag--){
		recv(socket_fd,buf,128,0);
		//memcpy(&chatmessage,buf,56);
		printf("Recv Msg = %s\n",buf);
		
		
		sleep(1);
		}
		exit(0);
	}
	*/

}

void sendMsg(){

	struct ChatMsg chatmessage;
	int size;

	getMsg();
	chatmessage.Flag = 100;
	if(atoi(u) == 123){
		chatmessage.who = 456;
	}else{
		chatmessage.who = 123;
	}
	//printf("who = %d\n",atoi(u));
	strcpy(chatmessage.Msg, chatmsg);
	size = sizeof(chatmessage);
	memcpy(buf,&chatmessage,size);
	//printf("flag=%d\tmsg=%s\n",chatmessage.Flag,chatmessage.Msg);
	send(socket_fd,buf,strlen(buf), 0);
	return ;
}

void getMsg(){
	GtkTextIter start,end;
	textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview2));

	/* get start Iter and end Iter of buffer */
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textbuffer),&start,&end);
	
	const GtkTextIter s = start,e = end;
	chatmsg = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), \ 
			&s,&e,FALSE);/*get text of textbuffer */
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(textbuffer),&start,&end);
	
}

void chat_window_close(){
	struct ChatMsg chatmessage;

	chatmessage.who = 0;
	chatmessage.Flag = 200;
	strcpy(chatmessage.Msg,"");
	memcpy(buf,&chatmessage,60);
	send(socket_fd,buf,128,0);
	gtk_widget_destroy(window);
}


void recv_msg(void *s){
	int socket_fd = (int)s;
	while(1){
		recv(socket_fd,buf,128,0);
		printf("Recv Msg = %s\n",buf);
		textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview1));
		//gtk_text_buffer_insert(GTK_TEXT_BUFFER(textbuffer),);
		sleep(1);
	}
}
