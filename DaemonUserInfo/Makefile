CFLAGS = -std=c11 -Wpedantic -Wall -Wextra -Wconversion -Werror -fPIC
LDFLAGS = -D_XOPEN_SOURCE=500 -pthread
EXECSRV = server
EXECCLI = client
SERVER = server.o
CLIENT = client.o
GLOBALS = globals_daemons_consts.h

server: $(EXECSRV)
client: $(EXECCLI)

$(EXECSRV): $(SERVER)
	gcc $(SERVER) $(LDFLAGS) -o $(EXECSRV)

$(CLIENT): $(CLIENT)
	gcc $(CLIENT) $(LDFLAGS) -o $(EXECCLI)

clean:
	$(RM) *.o $(OBJECT)

fclean: clean
	$(RM) $(EXEC)

server.o: server.c server.h server_consts.h $(GLOBALS)
client.o: client.c client.h client_consts.h $(GLOBALS)