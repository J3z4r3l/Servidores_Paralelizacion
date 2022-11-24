#include "omp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 4
struct my_pixel{
  unsigned char r, g, b;
};
void img_jeza(){
    char *imagenes[]={"img1.bmp","img2.bmp","img3.bmp","img4.bmp","img5.bmp","img6.bmp","img7.bmp","img8.bmp","img9.bmp","img10.bmp","img11.bmp","img12.bmp","img13.bmp","img14.bmp","img15.bmp","img16.bmp","img17.bmp","img18.bmp","img19.bmp","img20.bmp"};
    char *nuevas[]={"n1.bmp","n2.bmp","n3.bmp","n4.bmp","n5.bmp","n6.bmp","n7.bmp","n8.bmp","n9.bmp","n10.bmp","n11.bmp","n12.bmp","n13.bmp","n14.bmp","n15.bmp","n16.bmp","n17.bmp","n18.bmp","n19.bmp","n20.bmp"};
    //char *imagenes[]={"img1.bmp","img2.bmp","img3.bmp","img4.bmp","img5.bmp","img6.bmp"};
    //char *nuevas[]={"n1.bmp","n2.bmp","n3.bmp","n4.bmp","n5.bmp","n6.bmp"};
    int i, tam= sizeof(imagenes)/sizeof(char *);
    const double tempI = omp_get_wtime();
    for(int i=0; i<tam;++i){
        FILE *image, *outputImage, *lecturas;
        image = fopen(imagenes[i],"rb");          //Imagen original a transformar
        outputImage = fopen(nuevas[i],"wb");    //Imagen transformada
        long ancho;
        long alto;
        unsigned char r, g, b;               //Pixel

        unsigned char xx[54];
        for(int i=0; i<54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
        }
        ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
        alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
        printf("largo img %li\n",alto);
        printf("ancho img %li\n",ancho);

        unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));
        unsigned char* arr_out = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));

        int padding= 0;
        int j= 0;
        while(!feof(image)){
            arr_in[j] = fgetc(image);
            arr_out [j] = arr_in [j];
            j++;
        }
        padding = ancho%4;
        printf("%d\n", j);
        int count = 0;
        //omp_set_num_threads(4);
       // #pragma omp parallel for schedule(dynamic 3) 
        for(int i = 0; i < ancho*alto*3; i+=3){

                b = arr_in [i];
                g = arr_in [i + 1];
                r = arr_in [i + 2];
                unsigned char pixel = 0.21*r+0.72*g+0.07*b;
                // arr_in  [i] = pixel;
                // arr_in  [i + 1] = pixel;
                // arr_in  [i + 2] = pixel;
                arr_in  [i] = b;
                arr_in  [i + 1] = g;
                arr_in  [i + 2] = r;
                count += 3;
                if(count == ancho*3){
                    i+=padding;
                    count = 0;
                }


        }
        for(int i = 0; i < alto; i++){
            for(int j = 0; j < (ancho*3); j+= 3){
                arr_out [(i*((ancho*3)+padding))+j-3] = arr_in  [(i*((ancho*3)+padding))+ (ancho*3) - j];
                arr_out [(i*((ancho*3)+padding))+j-4]  = arr_in  [(i*((ancho*3)+padding))+ (ancho*3 )- j - 1];
                arr_out [(i*((ancho*3)+padding))+j-5]  = arr_in  [(i*((ancho*3)+padding))+ (ancho*3) - j - 2];
            }
        }

        for(int i = 0; i < ancho*alto*3; i++){
            fputc(arr_out [i], outputImage);
        }

        fclose(image);
        fclose(outputImage);

    }
    const double ftime = omp_get_wtime(); // tiempo total de la ejecucion
  printf("tomo (%lf) segundos\n", (ftime - tempI));
    printf("Termine\n"); 
}
int main()
{
    #pragma omp parallel num_threads(4)
    {
    img_jeza();
}
    return 0;
}
