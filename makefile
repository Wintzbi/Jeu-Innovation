# Cibles par défaut
all: jeu

# Compilation des fichiers objets
cell.o: cell.c
	gcc -c cell.c -I ~/raylib/src -L ~/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall

inventory.o: inventory.c
	gcc -c inventory.c -I ~/raylib/src -L ~/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall

main.o : main.c
	gcc -c main.c  -I ~/raylib/src -L ~/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall
# Règle pour lier les fichiers objets en un exécutable
jeu: cell.o inventory.o main.o
	gcc cell.o inventory.o main.o -o jeu -I ~/raylib/src -L ~/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f jeu *.o

.PHONY: all clean