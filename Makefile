lab: lab3.o functions.o
	cc lab3.o functions.o -std=c99 -lreadline -g -o lab3

lab3.o: 
	cc -c lab3.c -std=c99 -lreadline -g

functions.o:
	cc -c functions.c -std=c99 -lreadline -g

clean:
	rm -rf *.o lab3 *.bin