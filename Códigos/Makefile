make:
	gcc -fopenmp -g -pg -Wall -Werror -pedantic -lm procedural.c -o procedural
	gcc -fopenmp -g -pg -Wall -Werror -pedantic -lm paralelo.c -o paralelo
	gcc abrir\ binario.c -o abrir\ binario
clean:
	rm -rf procedural
	rm -rf paralelo
	rm -rf gmon.out
	rm -rf binario_autoc
	rm -rf abrir\ binario
	rm -rf grafico.jpg
debug:
	gdb procedural
gprof:
	gprof procedural gmon.out
proc:
	./procedural
