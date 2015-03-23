all:
	gcc -o shell shell.c

debug:
	gcc shell.c -g shell

run:
	./shell

clean:
	rm -rf *.o shell headertest
