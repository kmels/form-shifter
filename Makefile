CC     = gcc
CFLAGS = `pkg-config --cflags gtk+-2.0`
LIBS   = `pkg-config --libs gtk+-2.0`

form-shifter:  clean main.o
	gcc $(CFLAGS) $(LIBS) -o form-shifter src/main.o src/widgets.o src/canvas.o src/polygons.o

main.o: widgets.o canvas.o polygons.o
	gcc $(CFLAGS) $(LIBS) -c -Wall src/main.c -o src/main.o

polygons.o: 
	gcc $(CFLAGS) $(LIBS) -c -Wall src/polygons.c -o src/polygons.o

widgets.o: src/widgets.c src/polygons.h
	gcc $(CFLAGS) $(LIBS) -c -Wall src/widgets.c -o src/widgets.o

canvas.o: src/canvas.c
	gcc $(CFLAGS) $(LIBS) -c -Wall src/canvas.c -o src/canvas.o

clean:
	/bin/rm -f *.o; /bin/rm -f form-shifter
