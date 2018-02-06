all: bin/ring

lib/connection.o: src/connection.c src/ring.h
	gcc -Wall -g -c -o $@ src/connection.c

lib/main.o: src/main.c src/ring.h
	gcc -Wall -g -c -o $@ src/main.c -pthread

lib/nodes.o: src/nodes.c src/ring.h
	gcc -Wall -g -c -o $@ src/nodes.c -pthread

lib/text.o: src/text.c ring.h
	gcc -Wall -g -c -o $@ src/text.c -lm

bin/ring: lib/connection.o lib/main.o lib/nodes.o lib/text.o ring.h
	gcc -Wall -g -o $@ lib/connection.o lib/main.o lib/nodes.o lib/text.o -lm -pthread

clean:
	rm -rv lib/* bin/*
