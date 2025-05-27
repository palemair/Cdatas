CC=gcc
 
# Les différents répertoires contenant respectivement les fichiers : Sources *.c, Headers *.h, Objets *.o, l'exécutable
SRCDIR=src
HEADDIR=include
OBJDIR=obj
LIBDIR=lib
BINDIR=bin
 
 
# Les différentes options de compilations, soyons strictes !
CFLAGS= -g -Wall -pedantic -Os -Wextra -std=c11 -fno-common -fno-builtin

# Les différents FrameWorks et bibliothèques pour le linkage
LDFLAGS=
 
# L'exécutable
BIN=cdatas
 
# Où trouver les différents sources *.c qu'il faudra compiler pour créer les objets correspondants
SRC= $(wildcard $(SRCDIR)/*.c)
OBJ= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
 
all: $(BIN)
 
#Création de l'exécutable
cdatas: $(OBJ) 
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)
 
$(OBJDIR)/main.o: $(SRCDIR)/main.c
	$(CC) -I$(HEADDIR)/ -o $@ -c $< $(CFLAGS)

# Création des différents *.o à partir des *.c
$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -I$(HEADDIR)/ -o $@ -c $< $(CFLAGS)
 
# Nettoyage des objets => Tout sera recompiler !
clean:
	rm $(OBJDIR)/*.o
 
# Nettoyage complet => clean + effacement du l'exécutable
Clean: clean
	rm $(BINDIR)/*
