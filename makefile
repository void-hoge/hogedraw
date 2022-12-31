CC = c++
RELEASE = -O3
SDLFLAGS = $(shell pkg-config --cflags sdl2)
SDLLIBS = $(shell pkg-config --libs sdl2)
GLLIBS = $(shell pkg-config --libs gl)
GLUTFLAGS = $(shell pkg-config --cflags glut)
GLUTLIBS = $(shell pkg-config --libs glut)
FTGLFLAGS = $(shell pkg-config --cflags ftgl)
FTGLLIBS = $(shell pkg-config --libs ftgl)
STD = -std=c++11
CFLAGS = $(STD) $(RELEASE) $(SDLFLAGS) $(SDLLIBS) $(GLLIBS) $(GLUTFLAGS) $(GLUTLIBS) $(FTGLFLAGS) $(FTGLLIBS) 
PROG = hogedraw

$(PROG): main.o hogedraw.o object.o canvas.o lex.o
	$(CC) main.o hogedraw.o object.o canvas.o lex.o $(CFLAGS) -o $(PROG)

main.o: main.cpp
	$(CC) main.cpp -c $(CFLAGS) -o main.o

hogedraw.o: hogedraw.cpp
	$(CC) hogedraw.cpp -c $(CFLAGS) -o hogedraw.o

object.o: object.cpp
	$(CC) object.cpp -c $(CFLAGS) -o object.o

canvas.o: canvas.cpp
	$(CC) canvas.cpp -c $(CFLAGS) -o canvas.o

lex.o: lex.cpp
	$(CC) lex.cpp -c $(STD) $(RELEASE) -o lex.o

clean:
	rm $(PROG) *.o *~ *.out
