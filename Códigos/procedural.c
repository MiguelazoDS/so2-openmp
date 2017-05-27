#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  char *salida_canal_v="canalV";
  char *salida_canal_h="canalH";
  FILE *file_in;
  FILE *file_V;
  FILE *file_H;
  uint16_t samples;
  /*int lineas;*/
  int count;
  int ciclo, i,j, muestras_gate;
  float *valores;
  char valor[20];
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  count=0;
  /*while(!feof(file_in)){*/
    while(fread(&samples,sizeof(uint16_t), 1, file_in)){
    count++;
    file_V=fopen(salida_canal_v,"w");
    file_H=fopen(salida_canal_h,"w");
    /*Guardo en samples el primer valor de 16 bits que tiene el archivo.*/
    /*fread(&samples, sizeof(uint16_t), 1, file_in);*/
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
    printf("%d %d %d\n", muestras_gate, (int)samples, count);
    if(count==73){
      printf("%f\n", *(valores+samples));
    }
    /*while(samples){
      printf("%d\n", (int)samples--);
      samples--;
    }*/
    /*Libera la memoria para poder ser alocar memoria nuevamente.*/
    free(valores);
    /*Borro archivos temporales luego de haber calculado los datos necesarios*/
    unlink(salida_canal_v);
    unlink(salida_canal_h);
  }
  return 0;
}
