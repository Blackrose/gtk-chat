TARGET=chat
objs=gtk-chat.o callback.o login.o chatWindow.o mainWindow.o
CFLAGS= -g -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS= `pkg-config --libs gtk+-2.0` -lpthread

$(TARGET):$(objs)
	gcc $^ -o $@ $(LDFLAGS)
	rm -rf $(objs)
%.o:%.c
	gcc -c $< $(CFLAGS)

.PHONY:clean
clean:
	rm -rf $(TARGET) $(objs)
