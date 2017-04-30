#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define  TAM 5000

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  FILE *file_in;
  uint16_t samples;
  int ciclo;
  int count=0;
  float valores;
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  while(!feof(file_in)){
    count=0;
    fread(&samples,sizeof(uint16_t), 1, file_in);
    printf("%d\n", samples);

    ciclo=4*samples;
    while(ciclo){
      fread(&valores,sizeof(float),1,file_in);
      /*printf("%.10f\n", valores);*/
      count++;
      ciclo--;
    }
    printf("%d\n", count);

  }
  /*fread(&samples,2, 1, file_in);
  printf("%d\n", samples);*/

  return 0;
}
