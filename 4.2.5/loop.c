//gcc loop.c -o loop -lglut -lm -lGLU -lGL
#include <math.h>
#include <stdio.h>

void main(int argc, char** argv)
{
	int i;
	float x1, a, x1New, x2, x2New;
	
	x1 = -0.5;
	x2 = -0.5;
	
	a = -1.95;
	
	printf("\n Iteration %d:\t x1 = %3.15f \t x2 = %3.15f \t drift = %3.15f", 0, x1, x2, x1-x2);
	
	for(i = 1; i <= 50000; i++)
	{
		x1New = x1*x1 + a;
		x1 = x1New;

		//x2New = x2*x2 + a;
		//x2New = pow(x2,2) +a;
		x2New = x2*(x2*x2 + a)/x2;
		//x2New = 0.0625*(x2*x2 + a)/0.0625;
		x2 = x2New;		
		
		printf("\n Iteration %d:\t x1 = %3.15f \t x2 = %3.15f \t drift = %3.15f", i, x1, x2, x1-x2);
	}
	printf("\n");
}
