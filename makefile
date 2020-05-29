parallel: 
	gcc -fopenmp ./src/parallel.c -o parallel
serial:
	gcc ./src/serial.c -o serial
clean:
	rm serial parallel