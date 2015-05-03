default : servudp


sint.o: sint.c sint.h
	cc -c sint.c -lm

servudp : servudp.c sint.c sint.h
	cc -o servudp servudp.c sint.c -lm

clean :
	rm -f *.o *.gx socket 

