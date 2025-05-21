CC=gcc
 
# Les différents répertoires contenant respectivement les fichiers : Sources *.c, Headers *.h, Objets *.o, l'exécutable
SRCDIR=src
HEADDIR=include
LIBDIR=obj
BINDIR=bin
 
 
# Les différentes options de compilations, soyons strictes !
CFLAGS= -g -Wall -pedantic -Os -Wextra -std=c11 -fno-common -fno-builtin

# Les différents FrameWorks et bibliothèques pour le linkage
LDFLAGS=
 
# L'exécutable
BIN=cdatas
 
# Où trouver les différents sources *.c qu'il faudra compiler pour créer les objets correspondants
SRC= $(wildcard $(SRCDIR)/*.c)
OBJ= $(SRC:$(SRCDIR)/%.c=$(LIBDIR)/%.o)
 
all: $(BIN)
 
#Création de l'exécutable
cdatas: $(OBJ) 
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
 
$(LIBDIR)/main.o: $(SRCDIR)/main.c
	$(CC) -o $@ -c $< $(CFLAGS)

# Création des différents *.o à partir des *.c
$(LIBDIR)/%.o: $(SRCDIR)/%.c $(HEADDIR)/%.h 
	$(CC) -o $@ -c $< $(CFLAGS)
 
# Nettoyage des objets => Tout sera recompiler !
clean:
	rm $(LIBDIR)/*.o
 
# Nettoyage complet => clean + effacement du l'exécutable
Clean: clean
	rm $(BINDIR)/*
