CC = c++
RELEASE = -O3 -mtune=native -march=native
SDLFLAGS = $(shell pkg-config --cflags sdl2)
SDLLIBS = $(shell pkg-config --libs sdl2)
SDLIMAGELIBS = $(shell pkg-config --libs SDL2_image) -lSOIL
GLLIBS = $(shell pkg-config --libs gl)
GLUTFLAGS = $(shell pkg-config --cflags glut)
GLUTLIBS = $(shell pkg-config --libs glut)
FTGLFLAGS = $(shell pkg-config --cflags ftgl)
FTGLLIBS = $(shell pkg-config --libs ftgl)
STD = -std=c++11
CFLAGS = $(STD) $(RELEASE) $(SDLFLAGS) $(SDLLIBS) $(GLLIBS) $(GLUTFLAGS) $(GLUTLIBS) $(FTGLFLAGS) $(FTGLLIBS) $(SDLIMAGELIBS)
PROG = hogedraw

$(PROG): main.o hogedraw.o object.o canvas.o lex.o option.o vec.o
	$(CC) main.o hogedraw.o object.o canvas.o lex.o option.o vec.o $(CFLAGS) -o $(PROG)

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

option.o: option.cpp
	$(CC) option.cpp -c $(STD) $(RELEASE) -o option.o

vec.o: vec.cpp
	$(CC) vec.cpp -c $(STD) $(RELEASE) -o vec.o

clean:
	rm $(PROG) *.o *~ *.out
