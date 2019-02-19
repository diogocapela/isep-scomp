main: main.o 
	gcc -Wall -g main.o -o main

validate: validate.o
	gcc -Wall -g validate.o validate

validate.o: validate.c
	gcc -Wall -g -c validate.c

main.o: main.c 
	gcc -Wall -g -c main.c

run: main
	./main

clean:
	rm *.o main

debug:
	ddd main