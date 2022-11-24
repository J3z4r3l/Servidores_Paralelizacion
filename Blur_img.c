#include "omp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 1
void img_create(int ImgNum, int ImgB, int nc){
//Abrimos el archivo
  FILE *img, *outImg;
  omp_set_num_threads(NUM_THREADS);
  //Abrimos la variable de tiempo 
  const double tempI = omp_get_wtime();
  char org[16]; snprintf(org, 16, "img%i.bmp", ImgNum);
  char dst[16]; snprintf(dst, 16, "imgOut%i.bmp", nc);
  img = fopen(org, "rb"); outImg = fopen(dst, "wb"); 
  //Tamaño del arreglo 
  int blur = ImgB;  int half = (int)blur / 2;
  long ancho; long alto;
  //Acomodo de los pixeles en rojo verde y azul
  unsigned char r, g, b;  unsigned char *punt;
  unsigned char xx[54]; int contador = 0;

  for (int i = 0; i < 54; i++) {
    xx[i] = fgetc(img);
    fputc(xx[i], outImg);
  }
  //Copiado de códigos anteriores
  ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
  alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];
  //Imprimimos las dimensiones de la imagen  
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);
  punt = (unsigned char *)malloc(alto * ancho * 3 * sizeof(unsigned char));
  unsigned char imgIN[alto][ancho];
  unsigned char imgBlur[alto][ancho];

  /*Se probo sin el uso de imagenes grises sin embargo no 
  se logro que el programa lo hiciera correctamente*/
  while (!feof(img)) {
    b = fgetc(img);
    g = fgetc(img);
    r = fgetc(img);
    unsigned char pixel = 0.21 * r + 0.72 * g + 0.07 * b;
    punt[contador] = pixel;
    punt[contador + 1] = pixel;
    punt[contador + 2] = pixel;
    contador++;
  }
//Se realiza un conteo del proceso
  int cont = 0;
  for (int i = 0; i < alto; i++) {
    for (int j = 0; j < ancho; j++) {
      imgIN[i][j] = punt[cont];
      cont += 1;
    }
  }
  // Se realiza el efecto de blurring
  for (int i = half; i < alto - half; i++)
    for (int j = half; j < ancho - half; j++) {
      int suma = 0;
      for (int x = -half; x <= half; x++)
        for (int y = -half; y <= half; y++)
          suma += imgIN[i + x][j + y];
      imgBlur[i][j] =
          (int)suma /
          (blur * blur); // Se promedia el valor del pixel
    }
  cont = 0;
  for (int i = 0; i < alto; i++) {
    for (int j = 0; j < ancho; j++) {
      punt[cont] = imgBlur[i][j];
      punt[cont + 1] = imgBlur[i][j];
      punt[cont + 2] = imgBlur[i][j];
      cont += 1;
    }
  }

// Se paraleliza la tarea para dar una mayor rapidez
#pragma omp parallel
  {
#pragma omp for schedule(dynamic)
    //Se hace la imagen de salida 
    for (int i = 0; i < alto * ancho * 3; ++i) {
      fputc(punt[i], outImg);
      fputc(punt[i + 1], outImg);
      fputc(punt[i + 2], outImg);
    }
  }

  free(punt);
  fclose(img);
  fclose(outImg);
  const double ftime = omp_get_wtime(); // tiempo total de la ejecucion
  printf("tomo (%lf) segundos\n", (ftime - tempI));
}

int main() {
    int numImg=1; int Grado;
    int Arrg[20]={3,5,15,25,33,40,45,50,55,57,60,61,63,64,65,66,67,68,69,70};
    //3,5,15,25,33,40,45,50,55,57,60,61,63
    printf("Iniciendo Blurreo...\n");
    for(int n=15; n<=20; n++){
        int Grado=Arrg[n-1];
        img_create(numImg,Grado,n);
    }
    printf("Fin...");
    return 0;
}
