#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>

#define GATES 500

/*Realiza una operación de autocorrelacion para las matriz v y la matriz h*/
void autocorrelacion(uint16_t filas, float ***matrix_v, float ***matrix_h, float **autoc_v, float **autoc_h){
	int i,j;
	float temp_v;
	float temp_h;
	for (i = 0; i < GATES; i++) {
		for (j = 0; j < filas-1; j++) {
			temp_v+=(*matrix_v)[j][i]*(*matrix_v)[j+1][i];
			temp_h+=(*matrix_h)[j][i]*(*matrix_h)[j+1][i];
		}
		(*autoc_v)[i]=temp_v/filas;
		(*autoc_h)[i]=temp_h/filas;
		temp_v=0;
		temp_h=0;
	}
}

/*Recibe la dirección del puntero para abrir el archivo y cuenta la cantidad de pulsos que hay.*/
int cantidad_pulsos(FILE **file_in){
  char *nombre="pulsos.iq";
  int cantidad=0;
  uint16_t samples;
  *file_in=fopen(nombre,"rb");
  if (*file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }

  while(fread(&samples,1,sizeof(uint16_t),*file_in)){
    fseek(*file_in,4*samples*sizeof(float),SEEK_CUR);
    cantidad++;
  }
	/*Posiciona el puntero al inicio del archivo*/
  fseek(*file_in,0,SEEK_SET);
  fclose(*file_in);
  return cantidad;
}

void matrices(FILE **file_in,float ***matrix_v,float ***matrix_h){
  FILE *file_V;
  FILE *file_H;
  int i,j,ciclo;
  char *nombre="pulsos.iq";
  char *canal_v="canalV";
  char *canal_h="canalH";
  float *valores;
  /*char *lineas_v=malloc(100*sizeof(float));
  char *lineas_h=malloc(100*sizeof(float));*/
  int muestras_gate,cant=0;
  float acumulador_v;
  float acumulador_h;
  uint16_t aux;
	uint16_t samples;
	uint16_t filas=0;
	float *cv;
	float *ch;

  *file_in=fopen(nombre,"rb");
  while(fread(&samples,1,sizeof(uint16_t), *file_in)){
  	file_V=fopen(canal_v,"w");
  	file_H=fopen(canal_h,"w");

    /*La cantidad total de muestras de cada pulso es 4 veces el valor samples.*/
    ciclo=4*samples;
    /*Reservo la cantidad necesaria para guardar un pulso.*/
    valores=malloc(ciclo*sizeof(float));
		cv=malloc(samples*sizeof(float));
		ch=malloc(samples*sizeof(float));
    fread(valores, ciclo, sizeof(float), *file_in);
    /*indice para incrementar de a 1 la variable donde guarda los valores complejos*/
    j=0;
    for (i = 0; i < ciclo; i+=2){
      /*Guarda los valores complejos del canal V en un archivo y los del canal H en otro archivo.*/
      if(j>=0&&j<samples){
				fprintf(file_V, "%.10f\n", sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2)));
				cv[j]=sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2));
      }
      if(j>=samples){
				fprintf(file_H, "%.10f\n", sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2)));
				ch[j-samples]=sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2));
      }
      j++;
    }

		/*for (i = 0; i < samples; i++) {
			printf("canal v: %.10f canal h:%.10f\n", cv[i],ch[i]);
		}*/
    fclose(file_V);
    fclose(file_H);

    file_V=fopen(canal_v,"r");
    file_H=fopen(canal_h,"r");
    acumulador_v=0;
    acumulador_h=0;
    muestras_gate=0;
    aux=samples;

    cant=0;
		for (i = 0; i < samples; i++) {
			acumulador_v+=cv[i];
			acumulador_h+=ch[i];
			muestras_gate++;
			if(muestras_gate==(int)(aux*2e-3)){
				if(cant<GATES){
					(*matrix_v)[filas][cant]=acumulador_v/(int)(aux*2e-3);
					(*matrix_h)[filas][cant]=acumulador_h/(int)(aux*2e-3);
          cant++;
          acumulador_v=0;
          acumulador_h=0;
          muestras_gate=0;
				}
			}
		}
    /*while(samples){
      samples--;
      fgets(lineas_v, 100, file_V);
      fgets(lineas_h, 100, file_H);
      acumulador_v+=atof(lineas_v);
      acumulador_h+=atof(lineas_h);
      muestras_gate++;
      if(muestras_gate==(int)(aux*2e-3)){
        if(cant<GATES){
					(*matrix_v)[filas][cant]=acumulador_v/(int)(aux*2e-3);
					(*matrix_h)[filas][cant]=acumulador_h/(int)(aux*2e-3);
          cant++;
          acumulador_v=0;
          acumulador_h=0;
          muestras_gate=0;
        }
      }
    }*/
		filas++;
    /*Libera la memoria para poder ser alocar memoria nuevamente.*/
    free(valores);
		free(cv);
		free(ch);
  }
}

int main(int argc, char const *argv[]) {
  /*manejadores de archivos.*/
  FILE *file_in;
	float **matrix_v;
	float **matrix_h;
	float *autoc_v;
	float *autoc_h;
	FILE *file_out;
	char *binary="binario_autoc";
	int i;
	uint16_t filas=cantidad_pulsos(&file_in);
	int cant_gates;

	matrix_v=malloc(filas*sizeof(float*));
	matrix_h=malloc(filas*sizeof(float*));
	for (i = 0; i < filas; i++) {
		matrix_v[i]=malloc(GATES*sizeof(float));
		matrix_h[i]=malloc(GATES*sizeof(float));
	}

  matrices(&file_in,&matrix_v,&matrix_h);

	autoc_v=malloc(GATES*sizeof(float));
	autoc_h=malloc(GATES*sizeof(float));

	autocorrelacion(filas, &matrix_v, &matrix_h, &autoc_v, &autoc_h);

	file_out=fopen(binary,"wb");
	cant_gates=GATES;
	fwrite(&cant_gates,1,sizeof(int),file_out);
	for (i = 0; i < GATES; i++) {
		fwrite(&autoc_v[i],1,sizeof(float),file_out);
	}
	for (i = 0; i < GATES; i++) {
		fwrite(&autoc_h[i],1,sizeof(float),file_out);
	}

	fclose(file_out);

  return 0;
}
