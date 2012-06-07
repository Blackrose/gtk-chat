#include <stdio.h>
#include <gtk/gtk.h>

#include "common.h"
#include "chatWindow.h"
#include "callback.h"
#include "login.h"

int main(int argc,char *argv[]){
	
	gtk_init(&argc, &argv);
	
	login_window_init();

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
