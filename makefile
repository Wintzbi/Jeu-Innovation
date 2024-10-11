# Cibles par défaut
all: jeu

# Compilation des fichiers objets
cell.o: cell.c
	gcc -c cell.c -I/opt/homebrew/Cellar/raylib/5.0/include 

inventory.o: inventory.c
	gcc -c inventory.c -I/opt/homebrew/Cellar/raylib/5.0/include 

texture.o: texture.c
	gcc -c texture.c -I/opt/homebrew/Cellar/raylib/5.0/include 

input.o: input.c
	gcc -c input.c -I/opt/homebrew/Cellar/raylib/5.0/include 

main.o : main.c
	gcc -c main.c  -I/opt/homebrew/Cellar/raylib/5.0/include 
# Règle pour lier les fichiers objets en un exécutable
jeu: cell.o inventory.o texture.o input.o main.o
	gcc cell.o inventory.o texture.o input.o main.o -o jeu -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f jeu *.o

.PHONY: all clean

