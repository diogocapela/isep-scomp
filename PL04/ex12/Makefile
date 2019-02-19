main: main.o
	gcc -Wall -g main.o -o main.bin -lpthread -lrt

main.o: main.c
	gcc -Wall -g -c main.c

cliente: cliente.o
	gcc -Wall -g cliente.o -o cliente -lpthread -lrt

cliente.o: cliente.c
	gcc -Wall -g -c cliente.c

seller: seller.o
	gcc -Wall -g seller.o -o seller -lpthread -lrt

seller.o: seller.c
	gcc -Wall -g -c seller.c

run: main
	./main.bin

clean:
	rm *.o *.bin
	rm /dev/shm/*
