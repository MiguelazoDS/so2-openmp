#include <stdlib.h>
#include <stdio.h>

#define  TAM 5000

typedef struct{
  int validSamples;
  float V_I[TAM];
  float V_Q[TAM];
  float H_I[TAM];
  float H_Q[TAM];
  float array[];
}nodo;

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  FILE *file_in;
  nodo T;
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }

  while(!feof(file_in)){
    fread(&T, 1, sizeof(nodo), file_in);
    if(T.validSamples==649858869) printf("%d\n", T.validSamples);
  }
  fclose(file_in);

  return 0;
}
