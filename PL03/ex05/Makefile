main: main.o 
	gcc -Wall -g main.o -o main -lrt

main.o: main.c 
	gcc -Wall -g -c main.c

run: main
	./main

clean:
	rm *.o main

debug:
	ddd main