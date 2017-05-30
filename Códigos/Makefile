make:
	gcc -g -pg -Wall -Werror -pedantic -lm procedural.c -o procedural
clean:
	rm -rf procedural
	rm -rf *.txt
	rm -rf canal?
	rm -rf *.csv
	rm -rf gmon.out
	rm -rf out
	rm binario_autoc
debug:
	gdb procedural
gprof:
	gprof ./procedural > out
