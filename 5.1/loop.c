//gcc loop.c -o loop -lglut -lm -lGLU -lGL
#include <math.h>
#include <stdio.h>

void main(int argc, char** argv)
{
	int i;
	double x1, x1New;
	
	x1 = -0.5;
	
	printf("\n Iteration %d:\t x1 = %3.12f \t", 0, x1);
	
	for(i = 1; i <= 100; i++)
	{
		x1New = -3.0*fabs(x1 - 0.5) + 3.0/2.0;
		x1 = x1New;
		printf("\n Iteration %d:\t x1 = %3.12f \t", i, x1);
	}
	printf("\n");
}
