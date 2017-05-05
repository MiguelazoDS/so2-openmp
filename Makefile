make:
	gcc -Wall -Werror -pedantic -lm procedural.c -o procedural
clean:
	rm -rf procedural
	rm -rf *.txt
