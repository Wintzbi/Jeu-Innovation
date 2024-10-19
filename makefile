# Cibles par défaut
all: jeu

# Détecter le système d'exploitation
UNAME_S := $(shell uname -s)

# Paramètres spécifiques à macOS
ifeq ($(UNAME_S), Darwin)
    CC = gcc
    CFLAGS = -I/opt/homebrew/Cellar/raylib/5.0/include -Wall
    LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    OUTPUT = jeu
else
    # Paramètres spécifiques à Linux
    CC = gcc
    CFLAGS = -I$(HOME)/raylib/src -Wall
    LDFLAGS = -L$(HOME)/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    OUTPUT = jeu
endif

# Liste des fichiers objets
OBJS = cell.o inventory.o texture.o input.o menu.o musique.o camera.o main.o

# Compilation des fichiers objets
%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

# Lier les fichiers objets en un exécutable
$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUTPUT) $(LDFLAGS)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OUTPUT) *.o

.PHONY: all clean
