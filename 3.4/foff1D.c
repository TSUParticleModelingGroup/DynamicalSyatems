// gcc foff1D.c -o foff1D -lglut -lm -lGLU

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359 

double function(double x)
{
	return(x - (x*x - x - 1)/(x+x - 1));
}

int main(int argc, char** argv)
{
	int k;
	double xOld, xNew, change;
	
	xOld = -.7;
	k = 0;
	printf("\n interation = %d   x = %f", k, xOld);

	xNew = function(xOld);
	k++;
	change = fabs(xNew - xOld);
	printf("\n interation = %d   x = %f   change = %f", k, xNew, change);
	xOld = xNew;
	
	while(change > 0.00001 || 100 < k)
	{
		xNew = function(xOld);
		k++;
		change = fabs(xNew - xOld);
		printf("\n interation = %d   x = %f   change = %f", k, xNew, change);
		xOld = xNew;
	}
	printf("\n\n");
}
