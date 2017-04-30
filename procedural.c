#include <stdlib.h>
#include <stdio.h>

#define  TAM 5000

/*typedef struct{
  int validSamples;
  float array[];
}nodo;*/

int main(int argc, char const *argv[]) {
  char *nombre="pulsos.iq";
  FILE *file_in;
  /*nodo T;*/
  int samples;
  int ciclo;
  float valores;
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  fread(&samples,2, 1, file_in);
  printf("%d\n", samples);
  ciclo=4*samples+2;
  while(ciclo){
    fread(&valores,4,1,file_in);
    printf("%.5f\n", valores);
    ciclo--;
  }
  fread(&samples,2, 1, file_in);
  printf("%d\n", samples);

  return 0;
}

/*extern const int n;
typedef struct
{
    int len;
    char p[];
} str;
void foo(void)
{
    size_t str_size = sizeof(str);  // equivalent to offsetoff(str, p)
    str *s = malloc(str_size + (sizeof(char) * n));
}*/
