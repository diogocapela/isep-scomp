options=-lpthread -lrt

all:
	gcc -Wall main.c -o main.bin ${options}

run:
	gcc -Wall main.c -o main.bin ${options}
	./main.bin

clean:
	rm *.bin
	rm /dev/shm/*
