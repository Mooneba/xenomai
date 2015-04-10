# Pour sint

all: socket

sint.o: sint.c sint.h
	cc -c sint.c -lm

socket : socket.c sint.c sint.h
	cc -o socket socket.c sint.c -lm

clean :
	rm -f *.o *.gx socket 
