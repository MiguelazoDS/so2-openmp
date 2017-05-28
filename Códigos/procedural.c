#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  /*nombre de los archivos.*/
  char *nombre="pulsos.iq";
  char *canal_v="canalV";
  char *canal_h="canalH";
  /*manejadores de archivos.*/
  FILE *file_in;
  FILE *file_V;
  FILE *file_H;
  /*Tanto en samples como en aux se guarda la cantidad de muestras de cada pulso.*/
  uint16_t samples;
  uint16_t aux;
  /*Variables donde se guardan los valores complejos de cada archivo (canal v y h).*/
  char *lineas_v=malloc(100*sizeof(float));
  char *lineas_h=malloc(100*sizeof(float));
  /*i, j son variables para los for, ciclo=samples*4. En muestras_gate se guardan la cantidad de
  muestras por gate de cada pulso.*/
  int ciclo, i,j,muestras_gate;
  /*Se guardan los valores que se leen luego de samples.*/
  float *valores;
  /*Variable intermedia para guardar los valores complejos de los dos canales
  antes de pasarlos a un archivo.*/
  char valor[20];
  /*Variables que van acumulando hasta el valor de muestras para cada gate.*/
  float acumulador_v;
  float acumulador_h;
  /*Valor necesario para armar la matriz gate-pulso.*/
  int cantidad_pulsos;
  
  /*Matrices gate-pulso para cada canal.*/
  /*float **matrizV;
  float **matrizH;*/
  file_in=fopen(nombre,"rb");
  if (file_in==NULL){
   perror("No se puede abrir el archivo binario");
  }
  fseek(file_in,0,SEEK_SET);
  /*Cuento la cantidad de pulsos y guardo en una variable para construir después la matriz gate-pulso*/
  while(fread(&samples,sizeof(uint16_t),1,file_in)){
    fseek(file_in,4*samples*sizeof(float),SEEK_CUR);
    cantidad_pulsos++;
  /*  printf("contador de pulsos%d\n", cantidad_pulsos++);*/
  }
  /*matrizV=malloc(cantidad_pulsos*sizeof(float *));
  matrizH=malloc(cantidad_pulsos*sizeof(float *));
  for (i = 0; i < cantidad_pulsos; i++) {
    matrizV[i]=malloc(500*sizeof(float));
    matrizH[i]=malloc(500*sizeof(float));
  }*/
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
    file_V=fopen(canal_v,"r");
    file_H=fopen(canal_h,"r");
    acumulador_h=0;
    muestras_gate=0;
    aux=samples;
    while(samples){
      samples--;
      fgets(lineas_v, 100,file_V);
      fgets(lineas_h, 100, file_H);
      acumulador_v+=atof(lineas_v);
      acumulador_h+=atof(lineas_h);
      muestras_gate++;
      if(muestras_gate==(int)(aux*2e-3)){
        printf("%.10f %d\n", acumulador_v/(int)(aux*2e-3));
        printf("%.10f %d\n", acumulador_h/(int)(aux*2e-3));
        acumulador_v=0;
        acumulador_h=0;
        muestras_gate=0;
      }
    }
    /*Libera la memoria para poder ser alocar memoria nuevamente.*/
    free(valores);
    /*Borro archivos temporales luego de haber calculado los datos necesarios*/
    unlink(canal_v);
    /*unlink(canal_h);*/
  /*}*/
  fclose(file_in);
  return 0;
}
