// gcc foff2D.c -o foff2D -lglut -lm -lGLU

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359 

double functionX(double x, double y)
{
	return(x - .1*(5*x*x + 3*y*y - 2*y*y*y -29));
}

double functionY(double x, double y)
{
	return(y - .1*(2*y*y + 2*y*y*y - 16));
}

int main(int argc, char** argv)
{
	int k;
	double xOld, xNew, yOld, yNew, change;
	
	xOld = 1.0;
	yOld = 1.0;
	k = 0;
	printf("\n interation = %d   x = %f y = %f ", k, xOld, yOld);

	xNew = functionX(xOld, yOld);
	yNew = functionY(xOld, yOld);
	k++;
	change = sqrt((xNew - xOld)*(xNew - xOld) + (yNew - yOld)*(yNew - yOld));
	printf("\n interation = %d   x = %f y = %f  change = %f", k, xNew, yNew, change);
	xOld = xNew;
	yOld = yNew;
	
	while(change > 0.00001 && 100 > k)
	{
		xNew = functionX(xOld, yOld);
		yNew = functionY(xOld, yOld);
		k++;
		change = sqrt((xNew - xOld)*(xNew - xOld) + (yNew - yOld)*(yNew - yOld));
		printf("\n interation = %d   x = %f y = %f  change = %f", k, xNew, yNew, change);
		xOld = xNew;
		yOld = yNew;
	}
	printf("\n\n");
}
