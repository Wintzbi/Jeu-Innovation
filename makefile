OUTPUT = jeu

all: $(OUTPUT)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    CC = gcc
    CFLAGS = -I/opt/homebrew/Cellar/raylib/5.0/include -Wall
    LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    CC = gcc
    CFLAGS = -I$(HOME)/raylib/src -Wall
    LDFLAGS = -L$(HOME)/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

OBJS = cell.o inventory.o texture.o input.o menu.o musique.o camera.o craft.o stat.o main.o machine.o texture_registry.o ui.o energy.o conveyor.o production.o saveload.o

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUTPUT) $(LDFLAGS)
	rm -f *.o

clean:
	rm -f $(OUTPUT) *.o

.PHONY: all clean