all: 
	rm -f ex
	gcc -Wall -o ex main.c

run:
	./ex