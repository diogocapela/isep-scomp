main: main.o
	gcc -Wall -g main.o -o main.bin -lpthread -lrt

main.o: main.c
	gcc -Wall -g -c main.c

writter: writter.o
	gcc -Wall -g writter.o -o writter -lpthread -lrt

writter.o: writter.c
	gcc -Wall -g -c writter.c

reader: reader.o
	gcc -Wall -g reader.o -o reader -lpthread -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c

run: main
	./main.bin

clean:
	rm *.o *.bin
	rm reader
	rm writter
	rm /dev/shm/*
