CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -fPIC -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE
LDFLAGS = -lrt -pthread
OBJECTS_SERVER = server.o util.o
OBJECTS_CLIENT = client.o util.o
EXEC_PROC = info_proc
EXEC_USER = info_user
EXEC_SERVER = server
EXEC_CLIENT = client

all: $(EXEC_PROC) $(EXEC_USER) $(EXEC_CLIENT) $(EXEC_SERVER)

$(EXEC_PROC):
	$(CC) $(CFLAGS) $(EXEC_PROC).c $(LDFLAGS) -o $(EXEC_PROC)

$(EXEC_USER):
	$(CC) $(CFLAGS) $(EXEC_USER).c $(LDFLAGS) -o $(EXEC_USER)

$(EXEC_SERVER): $(EXEC_PROC).o $(EXEC_USER).o $(OBJECTS_SERVER)
	$(CC) $(CFLAGS) $(OBJECTS_SERVER) $(LDFLAGS) -o $(EXEC_SERVER)

$(EXEC_CLIENT): $(OBJECTS_CLIENT)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) $(LDFLAGS) -o $(EXEC_CLIENT)

clean:
	$(RM) $(EXEC_PROC).o $(EXEC_USER).o $(OBJECTS_SERVER) $(OBJECTS_CLIENT)

fclean: clean
	$(RM) $(EXEC_PROC) $(EXEC_USER) $(EXEC_SERVER) $(EXEC_CLIENT)

server.o: server.c server.h global_server.h util.h
client.o: client.c client.h global_server.h util.h
util.o: util.c util.h
info_proc.o: info_proc.c
info_user.o: info_user.c
