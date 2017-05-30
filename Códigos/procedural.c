#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#define GATES 500
/*
void autocorrelation(float *y, float *z){
	float aux;
	int i;
	for (i = 0; i < M-1; i++){
		aux += y[i];
	}

	for (i = 0; i < long_autoc; i++) {
		z[i]=y[i+1]*aux;
	}
}*/



void complejo(FILE **file_in,float ***matrix_v,float ***matrix_h){
  uint16_t samples;
  FILE *file_V;
  FILE *file_H;
  FILE *file_out_v;
  FILE *file_out_h;
  int i,j,ciclo;
  char valor[20];
  char *nombre="pulsos.iq";
  char *canal_v="canalV";
  char *canal_h="canalH";
  char *out_v="matriz_v.csv";
  char *out_h="matriz_h.csv";
  float *valores;
  char *lineas_v=malloc(100*sizeof(float));
  char *lineas_h=malloc(100*sizeof(float));
  int muestras_gate,cant=0;
  float acumulador_v;
  float acumulador_h;
  uint16_t aux;
	int filas=0;

  *file_in=fopen(nombre,"rb");
  file_out_v=fopen(out_v,"a");
  file_out_h=fopen(out_h,"a");
  while(fread(&samples,sizeof(uint16_t), 1, *file_in)){
    /*fread(&samples,sizeof(uint16_t), 1, *file_in);*/
  file_V=fopen(canal_v,"w");
  file_H=fopen(canal_h,"w");

    /*La cantidad total de muestras de cada pulso es 4 veces el valor samples.*/
    ciclo=4*samples;
    /*Reservo la cantidad necesaria para guardar un pulso.*/
    valores=malloc(ciclo*sizeof(float));

    fread(valores, sizeof(float), ciclo, *file_in);
    /*indice para incrementar de a 1 la variable donde guarda los valores complejos*/
    j=0;
    for (i = 0; i < ciclo; i+=2){
      /*Guarda los valores complejos del canal V en un archivo y los del canal H en otro archivo.*/
      if(j>=0&&j<samples){
        sprintf(valor,"%.10f\n",sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2)));
        fputs(valor,file_V);
      }
      if(j>=samples){
        sprintf(valor,"%.10f\n",sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2)));
        fputs(valor,file_H);
      }
      j++;
    }
    fclose(file_V);
    fclose(file_H);

    file_V=fopen(canal_v,"r");
    file_H=fopen(canal_h,"r");
    acumulador_v=0;
    acumulador_h=0;
    muestras_gate=0;
    aux=samples;

    cant=0;
    while(samples){
      samples--;
      fgets(lineas_v, 100, file_V);
      fgets(lineas_h, 100, file_H);
      acumulador_v+=atof(lineas_v);
      acumulador_h+=atof(lineas_h);
      muestras_gate++;
      if(muestras_gate==(int)(aux*2e-3)){
        if(cant<500){
          fprintf(file_out_v, "%.10f,", acumulador_v/(int)(aux*2e-3));
          fprintf(file_out_h, "%.10f,", acumulador_h/(int)(aux*2e-3));
					(*matrix_v)[filas][cant]=acumulador_v/(int)(aux*2e-3);
					(*matrix_h)[filas][cant]=acumulador_h/(int)(aux*2e-3);
          cant++;
          acumulador_v=0;
          acumulador_h=0;
          muestras_gate=0;
        }
      }
    }
		filas++;
    fprintf(file_out_v, "\n");
    fprintf(file_out_h, "\n");
    /*Libera la memoria para poder ser alocar memoria nuevamente.*/
    free(valores);
  }
}

int main(int argc, char const *argv[]) {
  /*manejadores de archivos.*/
  FILE *file_in;
	float **matrix_v;
	float **matrix_h;
	int i,j;
	int filas;



	matrix_v=malloc(72*sizeof(float*));
	matrix_h=malloc(72*sizeof(float*));
	for (i = 0; i < 72; i++) {
		matrix_v[i]=malloc(500*sizeof(float));
		matrix_h[i]=malloc(500*sizeof(float));
	}

  complejo(&file_in,&matrix_v,&matrix_h);


	for (i = 0; i < 72; i++) {
		for (j = 0; j < 500 ;j++) {
			printf("%.10f      ", matrix_h[i][j]);
		}
		printf("\n");
	}

  fclose(file_in);
  return 0;
}
