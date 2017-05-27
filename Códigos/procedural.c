#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  char *canal_v="canalV";
  char *canal_h="canalH";
  FILE *file_in;
  FILE *file_V;
  FILE *file_H;
  uint16_t samples;
  char *lineas=malloc(100*sizeof(float));
  /*int count;*/
  int ciclo, i,j, muestras_gate;
  float *valores;
  char valor[20];
  /*float acumulador_v;
  float acumulador_h;*/
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
    /*Mientras fread devuelva el número de elementos leídos el bucle continua.
    Se lee el primer valor de 16 bits que contiene la cantidad de muestras.*/
    /*while(fread(&samples,sizeof(uint16_t), 1, file_in)){*/
    /*count=0;*/
    fread(&samples,sizeof(uint16_t), 1, file_in);
    file_V=fopen(canal_v,"w");
    file_H=fopen(canal_h,"w");
    /*La cantidad total de muestras de cada pulso es 4 veces el valor samples.*/
    ciclo=4*samples;
    /*Reservo la cantidad necesaria para guardar un pulso.*/
    valores=malloc(ciclo*sizeof(float));
    /*El índice de lectura del archivo binario quedó luego de leer el valor de 16 bits, a partir de ahí leo ciclo cantidades de valores float.*/
    fread(valores, sizeof(float), ciclo, file_in);
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
    muestras_gate=samples*2e-3;
    printf("%d %d\n", muestras_gate, (int)samples);
    file_V=fopen(canal_v,"r");
    file_H=fopen(canal_h,"r");
    while(samples){
      samples--;
      fgets(lineas, 100,file_H);
      printf("%s\n", lineas);
    }
    /*Libera la memoria para poder ser alocar memoria nuevamente.*/
    free(valores);
    /*Borro archivos temporales luego de haber calculado los datos necesarios*/
    unlink(canal_v);
    unlink(canal_h);
  /*}*/
  fclose(file_in);
  return 0;
}
