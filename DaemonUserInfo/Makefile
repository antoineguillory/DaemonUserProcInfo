CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -fPIC   \
   -pthread
LDFLAGS = -lrt
OBJECTS_SERVER = server.o util.o info_proc.o info_user.o
OBJECTS_CLIENT = client.o util.o
EXEC_SERVER = server
EXEC_CLIENT = client

all: $(EXEC_CLIENT) $(EXEC_SERVER)

$(EXEC_SERVER): $(OBJECTS_SERVER)
	$(CC) $(CFLAGS) $(OBJECTS_SERVER) $(LDFLAGS) -o $(EXEC_SERVER)

$(EXEC_CLIENT): $(OBJECTS_CLIENT)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) $(LDFLAGS) -o $(EXEC_CLIENT)

clean:
	$(RM) $(OBJECTS_SERVER) $(OBJECTS_CLIENT)

fclean: clean
	$(RM) $(EXEC_SERVER) $(EXEC_CLIENT)

server.o: server.c server.h global_server.h util.h info_proc.h info_user.h
client.o: client.c client.h global_server.h util.h
util.o: util.c util.h
info_proc.o: info_proc.c info_proc.h
info_user.o: info_user.c info_user.h
