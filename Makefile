ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglut -lGLEW -lglfw -lm
endif

CC = g++
CPLUS = -x c++ -std=c++11
CFLAGS=-g -I/opt/ports/include
DEPS = common.h file.h hw5.h

INIT_SHADER = initshader.o

all: hw5 hw5_multiple cubeDS cube_gouraud cube_phong cube_flat

cubeDS: cubeDS.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

cube_gouraud: cube_gouraud.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

cube_phong: cube_phong.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

cube_flat: cube_flat.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

hw5: hw5.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

hw5_multiple: hw5_multiple.o $(INIT_SHADER) $(DEPS)
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CPLUS) $(CFLAGS) -c $< -o $@

clean:
	-rm -r hw5 hw5_multiple cubeDS cube_gouraud cube_phong cube_flat *.o core *.dSYM
