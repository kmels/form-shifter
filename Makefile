CC     = gcc
CFLAGS = `pkg-config --cflags gtk+-2.0`
LIBS   = `pkg-config --libs gtk+-2.0`

form-shifter: 
	gcc $(CFLAGS) $(LIBS) -o form-shifter main.o

main.o: main.c
	gcc $(CFLAGS) $(LIBS) -c main.c

clean:
	/bin/rm -f main.o
