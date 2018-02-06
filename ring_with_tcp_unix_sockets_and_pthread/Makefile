all: bin/ring

lib/connections.o: src/connections.c src/ring.h
	gcc -Wall -g -c -o $@ src/connections.c -pthread

lib/main.o: src/main.c src/ring.h
	gcc -Wall -g -c -o $@ src/main.c -pthread

lib/nodes.o: src/nodes.c src/ring.h
	gcc -Wall -g -c -o $@ src/nodes.c -pthread

lib/text.o: src/text.c src/ring.h
	gcc -Wall -g -c -o $@ src/text.c -lm

bin/ring: lib/connections.o lib/main.o lib/nodes.o lib/text.o src/ring.h
	gcc -Wall -g -o $@ lib/connections.o lib/main.o lib/nodes.o lib/text.o -lm -pthread

clean:
	rm -v lib/* bin/*
