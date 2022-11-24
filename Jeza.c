#include <stdio.h>
#include <omp.h>
#include <math.h>
#define NUM_THREADS 6 //Vemos el número de threads a ocupar
/*Esto esta relacionado al número de threads? cada uno para
para cada tarea?*/
   FILE *fptr;
   FILE *fptr1;
   FILE *fptr2;
   FILE *fptr3;
   FILE *fptr4;
   FILE *fptr5;
//Creamos una función 
   void iteracion(int N,FILE *fptr);
   int N;


int main()
{  omp_set_num_threads(NUM_THREADS);//establecemos el número de trheads 
  //double t[N][NUM_THREADS],w[N][NUM_THREADS];
  double t1;
  double t2;
  double tiempo;
//Abrimos los ficheros 
   fptr=fopen("Euler_n_0.txt","w");
   fptr1=fopen("Euler_n_1.txt","w");
   fptr2=fopen("Euler_n_2.txt","w");
   fptr3=fopen("Euler_n_3.txt","w");
   fptr4=fopen("Euler_n_4.txt","w");
   fptr5=fopen("Euler_n_5.txt","w");
//Establecemos el tiempo necesario 
t1 = omp_get_wtime();
//Generamos el numero de subprocesos 
   #pragma omp parallel
{
     #pragma omp sections
    {
        /*Lo separamos por secciones
        y escribimos cuantos cuantas iteraciones 
        tendrá cara tarea*/ 
       #pragma omp section
         iteracion(100,fptr);//20//2000900/ %%%100/3300/7500/11400/15400
       #pragma omp section
         iteracion(200,fptr1);//200//2000400 %%%200/3400/7600/11500/15500
       #pragma omp section
         iteracion(400,fptr2);//2000//2000700 %%%400/3800/7800/11700/15700
       #pragma omp section
         iteracion(800,fptr3);//20000//2000800 %%%800/4600/8200/12100/16100
       #pragma omp section
         iteracion(1600, fptr4);//200000//2000500 %%%1600/6200/9800/17700
       #pragma omp section
         iteracion(3200, fptr5);//2000000//2000100 %%%3200/7400/11400/19300
 
   }
}

t2=omp_get_wtime();
tiempo=t2-t1;
printf("tomo (%lf) segundos\n", tiempo);
//Cerramos el archivo
   fclose(fptr);
   fclose(fptr1);
   fclose(fptr2);
   fclose(fptr3);
   fclose(fptr4);
   fclose(fptr5);
   return (0);
   }

void iteracion(int N, FILE *fptr){
      double h;
      double t[N][NUM_THREADS],w[N][NUM_THREADS];
      int i;
      //Establecemos los parametros inicio de y(x)=w0 a limite inferior de t y b es limite superior de t 
      double w0=2,a=1,b=2;
      h=(b-a)/N;
      //Condiciones inicales de t y "Y"
      w[0][omp_get_thread_num()] = w0;
      t[0][omp_get_thread_num()] = a;
      for(i = 1;i<N;i++){
          t[i][omp_get_thread_num()]=a+(h*i); //Cada cuanto avanza la iteración
          //Aqui modificamos dependiendo la operación que vayamos a realizar lo que esta dentro de h(aqui tiene h(t-y)^2) 
          w[i][omp_get_thread_num()]=w[i-1][omp_get_thread_num()]+h*(1+(w[i-1][omp_get_thread_num()]/t[i-1][omp_get_thread_num()]));
              fprintf(fptr, "%f\t %f\n", t[i][omp_get_thread_num()], w[i][omp_get_thread_num()]);
          }
         }