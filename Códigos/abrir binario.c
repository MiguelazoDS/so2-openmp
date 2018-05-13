/**@abrir binario.c
@author Cazajous Miguel A.
@date 13 May 2018.
@brief Socket TCP cliente.*/

#include <stdlib.h>
#include <stdio.h>

#define  GATES 500

/**Función principal*/
int main(int argc, char const *argv[]) {
  float *otro;
  int i,algo;
  FILE *file_in;
  char *binary="binario_autoc";

  file_in=fopen(binary,"rb");

  while(fread(&algo,1,sizeof(int),file_in))
  {
    printf("cantidad %d\n", algo);
    otro=malloc(2*algo*sizeof(float));
    fread(otro, 2*algo, sizeof(float), file_in);
  }
  for (i = 0; i < GATES; i++) {
    printf("cantidad %d, valores v %.10f, valores h %.10f\n", algo, *(otro+i), otro[GATES+i]);
  }
  fclose(file_in);
  return 0;
}
