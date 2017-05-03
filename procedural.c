#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define  TAM 5000

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  FILE *file_in;
  uint16_t samples;
  int ciclo;
  int count=0;
  float *valores;
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  /*while(!feof(file_in)){*/
    count=0;
    fread(&samples, sizeof(uint16_t), 1, file_in);
    printf("%d\n", samples);

    ciclo=4*samples;
    valores=malloc(ciclo*sizeof(float));
    fread(valores, sizeof(float), ciclo, file_in);
    printf("%.10f\n", *(valores+ciclo-1));
    count++;
    ciclo--;
    printf("%d\n", count);
    printf("valor 1: %f, valor 2: %f resultado: %.5f\n",*(valores+0),*(valores+1), sqrt(pow(*(valores+0),2)+pow(*(valores+1),2)));
  /*}*/
  return 0;
}
