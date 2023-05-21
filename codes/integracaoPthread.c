/*
 * Esse exercício resolve a integra x^2 no intervalo [0:1], ou seja,
 * o valor exato é de 1/
 * O valor de h é passado como parâmetro (h=b-a/n, sendo a e b os limites da integração e n subintervalos)
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<math.h>
#define NUM_THREADS 12

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
unsigned int steps;
double h;
double x;
double k;
double sum;
unsigned int carga;
unsigned int resto;
double func(double x){ return pow(x, 2.0); }

void* thread(void *input){
unsigned int j = 0;
  while(j<carga){
    unsigned int i = (*(unsigned int*)input)*carga + j;
    if(i>steps) break;
    double offset = h * (double) i;
	  double aux = 2.0 * k;
		  if ((i == 0) || (i == steps))
			  aux = k;
    pthread_mutex_lock(&lock);
		sum += (func(x + offset) * aux);
    pthread_mutex_unlock(&lock);
    j++;
  }
  pthread_exit(NULL);
}
void* lastthread(void *input){
unsigned int j = 0;
  while(j<=resto){
    unsigned int i = (*(unsigned int*)input)*carga + j;
    if(i>steps) break;
    double offset = h * (double) i;
	  double aux = 2.0 * k;
		  if ((i == 0) || (i == steps))
			  aux = k;
    pthread_mutex_lock(&lock);
		sum += (func(x + offset) * aux);
    pthread_mutex_unlock(&lock);
    j++;
  }
  pthread_exit(NULL);
}


int main(int ac, char**av) {
  h = atof(av[1]);
	k = h / 2;
	x = 0.0;
	sum = 0.0;

  steps = (unsigned int) 1.0 / h;


  fprintf(stdout, "\nIntegração de uma função f(x)\n");
  fprintf(stdout, "Qtde. de trapézios: %u - valor do h = %15.10lf\n", steps, h);
 // #pragma omp parallel for reduction(+:sum)
  pthread_t tid[NUM_THREADS];
  carga = steps / NUM_THREADS;
  resto = (steps % NUM_THREADS) + carga;
  for (unsigned int i = 0; i < (NUM_THREADS-1); i++){
      int *arg = malloc(sizeof(*arg));
      *arg = i;
		  pthread_create(&tid[i], NULL, thread, arg);
      pthread_join(tid[i], NULL);
  }
  int *arg = malloc(sizeof(*arg));
  *arg = NUM_THREADS-1;
  pthread_create(&tid[NUM_THREADS-1], NULL, lastthread, arg);
  pthread_join(tid[NUM_THREADS-1], NULL);
  fprintf(stdout, "resultado: I = %15.10lf\n", sum);

  return EXIT_SUCCESS;
}

