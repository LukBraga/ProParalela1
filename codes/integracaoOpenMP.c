/*
 * Esse exercício resolve a integra x^2 no intervalo [0:1], ou seja,
 * o valor exato é de 1/
 * O valor de h é passado como parâmetro (h=b-a/n, sendo a e b os limites da integração e n subintervalos)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double func(double x){ return pow(x, 2.0); }




int main(int ac, char**av) {
  double h = atof(av[1]),
		     k = h / 2,
		     x = 0.0,
		     sum = 0.0;

  unsigned int steps = (unsigned int) 1.0 / h;


  fprintf(stdout, "\nIntegracao de uma funcao f(x)\n");
  fprintf(stdout, "Qtde. de trapezios: %u - valor do h = %15.10lf\n", steps, h);
  #pragma omp parallel for reduction(+:sum)
  for (unsigned int i = 0; i <= steps; i++){
		  double offset = h * (double) i;
      printf("offset: %lf\n", offset);
		  double aux = 2.0 * k;
		  if ((i == 0) || (i == steps))
			  aux = k;
		  sum += (func(x + offset) * aux);
  }

  fprintf(stdout, "resultado: I = %15.10lf\n", sum);

  return EXIT_SUCCESS;
}

