CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -fno-common -fno-builtin
LDFLAGS=
EXEC=cdatas

all: $(EXEC)

cdatas: main.o csv.o print.o table.o list.o iter.o regexarray.o xtools.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) -c $^ $(CFLAGS)

xtools.o: xtools.c
	$(CC) -c $^ $(CFLAGS)

regexarray.o: regexarray.c
	$(CC) -c $^ $(CFLAGS)

list.o: list.c
	$(CC) -c $^ $(CFLAGS)

table.o: table.c
	$(CC) -c $^ $(CFLAGS)

print.o: print.c
	$(CC) -c $^ $(CFLAGS)

csv.o: csv.c
	$(CC) -c $^ $(CFLAGS)

iter.o: iter.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -rf *.o
	rm $(EXEC)
	rm a.out
