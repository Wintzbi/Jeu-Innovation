# Cibles par défaut
all: jeu

# Compilation des fichiers objets
cell.o: cell.c
	gcc -c cell.c -I/opt/homebrew/Cellar/raylib/5.0/include

inventory.o: inventory.c
	gcc -c inventory.c -I/opt/homebrew/Cellar/raylib/5.0/include

# Règle pour lier les fichiers objets en un exécutable
jeu: main.c cell.o inventory.o
	gcc main.c cell.o inventory.o -o jeu.exe -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f jeu.exe *.o

.PHONY: all clean
