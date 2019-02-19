writer: writer.o
	gcc -Wall -g writer.o -o writer.bin -lrt

writer.o: writer.c
	gcc -Wall -g -c writer.c

reader: reader.o
	gcc -Wall -g reader.o -o reader.bin -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c

run: writer reader
	./writer.bin
	./reader.bin

clean:
	rm *.o *.bin
