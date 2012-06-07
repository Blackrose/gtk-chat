
TARGET=server
OBJS=server.o
LDFLAGS= -lpthread -lsqlite3

$(TARGET):$(OBJS)
	gcc $^ -o $@ $(LDFLAGS)
%.c%.o:
	gcc -c $@

.PHONY:clean
clean:
	rm -rf $(OBJS) $(TARGET)
