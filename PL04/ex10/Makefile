options=-lpthread -lrt

all:
	gcc -Wall main.c -o main.bin ${options}
	gcc -Wall insert.c -o insert.bin ${options}
	gcc -Wall consult.c -o consult.bin ${options}
	gcc -Wall consult_all.c -o consult_all.bin ${options}

run:
	gcc -Wall main.c -o main.bin ${options}
	gcc -Wall insert.c -o insert.bin ${options}
	gcc -Wall consult.c -o consult.bin ${options}
	gcc -Wall consult_all.c -o consult_all.bin ${options}
	./main.bin

clean:
	rm *.bin
	rm /dev/shm/*
