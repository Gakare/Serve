all: server client

client:
	gcc client.c -o client.out

server:
	gcc server.c -o server.out

clean:
	rm *.out
