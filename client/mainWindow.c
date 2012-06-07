#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "chatWindow.h"


void setUsername();
gint label_event(GtkWidget *widget, GdkEventButton *event, \
		gpointer func_data);

GdkColor color;

void mainWindow(){
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "mainWindow.glade",NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "listwindow"));
	
	gtk_signal_connect(GTK_OBJECT(window),"destroy", \
			G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_default_size(GTK_WINDOW(window),250,550);

	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	image = GTK_WIDGET(gtk_builder_get_object(builder, "image1"));
	
	//setUsername();

	button = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
	eventBox = GTK_WIDGET(gtk_builder_get_object(builder, "eventbox1"));
	//gtk_widget_add_events(eventBox,DK_POINTER_MOTION_MASK);
	gtk_signal_connect(GTK_OBJECT(eventBox),"event", \
			G_CALLBACK(label_event),NULL);
	//gtk_signal_connect(GTK_OBJECT(eventBox),"move-");

	printf("mainWindow run!\n");

	gtk_widget_show_all(window);
}

void setUsername(){
	gtk_label_set_text(GTK_LABEL(label),u);
}

gint label_event(GtkWidget *widget, GdkEventButton *event, \ 
		gpointer func_data){
	label = GTK_WIDGET(gtk_builder_get_object(builder,"label3"));
	//if (GTK_IS_LABEL(widget) &&(event->type==GDK_2BUTTON_PRESS ||event->type==GDK_3BUTTON_PRESS) ) {
  	if(event->type == GDK_2BUTTON_PRESS){
		printf("%s clicked with button %d\n",
				event->type==GDK_2BUTTON_PRESS ? "double" : "triple",
				event->button);
		chatInit();
		//send(socket_fd,"S",1,0);
			
	}else if(event->type == GDK_ENTER_NOTIFY){
		gdk_color_parse("#7093DB",&color);
		gtk_widget_modify_bg(eventBox,GTK_STATE_NORMAL,&color);

	}else if(event->type == GDK_LEAVE_NOTIFY){
		//gtk_label_set_text(GTK_LABEL(label),"label leave");
		gdk_color_parse("white",&color);
		gtk_widget_modify_bg(eventBox,GTK_STATE_NORMAL,&color);
		}

  	return FALSE;
}
