all: hw13.o
	gcc -o hw13 hw13.o

hw13.o: hw13.c
	gcc -c hw13.c

run:
	./hw13

clean:
	rm *.o
