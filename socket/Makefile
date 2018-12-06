# Compiler
CC = gcc

# Macros specifying path for compile
SRC = ./src/
SERVER = ./bin/server/
CLIENT = ./bin/client/

# Compile server.c
server: server.o
	$(CC) -o $(SERVER)server $(SRC)server.o
server.o: server.c
	$(CC) -c $(SRC)server.o $(SRC)server.c

# Compile client.c
client: client.o
	$(CC) -o $(CLIENT)client $(SRC)client.o
client.o: client.c
	$(CC) -c $(SRC)client.c $(SRC)client.c

# Clean
OBJS := $(wildcard src/*.o)
clean:
	rm $(OBJS)
	rm $(SERVER)server
	rm $(CLIENT)client