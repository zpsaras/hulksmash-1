all:
	gcc -o hulksmash hulksmash.c

run:
	./hulksmash

clean:
	rm -rf *.o hulksmash
