make:
	gcc -std=c99 -Wall -Werror -pedantic procedural.c -o procedural
clean:
	rm -rf procedural
