make:
	gcc -g -pg -Wall -Werror -pedantic -lm procedural.c -o procedural
	gcc abrir\ binario.c -o abrir\ binario
clean:
	rm -rf procedural
	rm -rf canal?
	rm -rf gmon.out
	rm -rf out
	rm -rf binario_autoc
	rm -rf abrir\ binario
debug:
	gdb procedural
gprof:
	gprof ./procedural > out
proc:
	./procedural
