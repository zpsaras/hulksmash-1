all:
	gcc -o shell shell.c

run:
	./shell

clean:
	rm -rf *.o shell
