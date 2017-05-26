#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

void *memset(void *str, int c, size_t n);

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  FILE *file_in;
  uint16_t samples;
  int ciclo;
  int count;
  int i,j;
  float *valores;
  float *canal_V;
  float *canal_H;
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  count=0;
  /*while(!feof(file_in)){*/
    /*Guardo en samples el primer valor de 16 bits que tiene el archivo.*/
    fread(&samples, sizeof(uint16_t), 1, file_in);
    /*La cantidad total de muestras de cada pulso es 4 veces el valor samples.*/
    ciclo=4*samples;
    /*Reservo la cantidad necesaria para guardar un pulso.*/
    valores=malloc(ciclo*sizeof(float));
    canal_V=malloc(samples*sizeof(float));
    canal_H=malloc(samples*sizeof(float));
    memset(valores,'\0',ciclo);
    memset(canal_V,'\0',samples);
    memset(canal_H,'\0',samples);
    /*El índice de lectura del archivo binario quedó luego de leer el valor de 16 bits, a partir de ahí leo ciclo cantidades de valores float.*/
    fread(valores, sizeof(float), ciclo, file_in);
    /*Cuento la cantidad de pulsos.*/
    count++;
    /*indice para incrementar de a 1 la variable donde guarda los valores complejos*/
    j=0;
    for (i = 0; i < ciclo; i+=2) {
      /*Guarda los valores complejo del canal V seguido de los valores complejos del canal H.*/
      if(j>=0&&j<samples){
        *(canal_V+j)=sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2));
        printf("Canal V: %.10f\n", *(canal_V+j));
      }
      if(j>=samples){
        *(canal_H+j)=sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2));
        printf("Canal H: %.10f\n", *(canal_H+j));
      }
      j++;
    }
    printf("%d\n", count);
  /*}*/
  return 0;
}
