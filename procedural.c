#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define  TAM 5000

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  FILE *file_in;
  FILE *file_out;
  uint16_t samples;
  int ciclo;
  int count;
  int i;
  float *valores;
  char auxiliar[10];
  char valor[20];
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  count=0;
  while(!feof(file_in)){
    fread(&samples, sizeof(uint16_t), 1, file_in);
    ciclo=4*samples;
    valores=malloc(ciclo*sizeof(float));
    fread(valores, sizeof(float), ciclo, file_in);
    count++;
    ciclo--;
    sprintf(auxiliar,"%d.txt",count);
    file_out=fopen(auxiliar,"a");
    sprintf(valor,"%d\n",samples);
    fputs(valor,file_out);
    for (i = 0; i < ciclo; i+=2) {
      sprintf(valor,"%.10f\n",sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2)));
      /*printf("valor 1: %.10f, valor 2: %.10f resultado: %.10f\n",*(valores+i),*(valores+i+1), sqrt(pow(*(valores+i),2)+pow(*(valores+i+1),2)));*/
      fputs(valor,file_out);
    }
    printf("%d\n", count);
    fclose(file_out);
  }
  return 0;
}
