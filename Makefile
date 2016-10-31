ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglut -lGLEW -lglfw -lm
endif

CC = g++
CPLUS = -x c++ -std=c++11
CFLAGS=-g -I/opt/ports/include
DEPS = common.h 

INIT_SHADER = initshader.o

all: hw5

hw5: hw5.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CPLUS) $(CFLAGS) -c $< -o $@ 

clean:
	-rm -r hw5 *.o core *.dSYM
