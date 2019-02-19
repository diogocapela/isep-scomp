pipes: pipes.o
	gcc -Wall -g pipes.o -o pipes -lrt

pipes.o: pipes.c
	gcc -Wall -g -c pipes.c

writer: writer.o
	gcc -Wall -g writer.o -o writer -lrt

writer.o: writer.c
	gcc -Wall -g -c writer.c

reader: reader.o
	gcc -Wall -g reader.o -o reader -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c




run-pipes: pipes
	./pipes



clean:
	rm *.o pipes writer reader
