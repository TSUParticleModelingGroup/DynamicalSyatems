//gcc randomHausdorffContractionMapping.c -o temp -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Screen size
#define N 1000

#define NUMBER_OF_ITERATIONS 10000

#define X0 0.0
#define Y0 0.0

unsigned int window_width = N;
unsigned int window_height = N;
unsigned int n = N;
float *pixels;

double globalX;
double globalY;

double IF1[6] = {0.5, 0.0, 0.0, 0.5, 0.0 , 0.0};
double IF2[6] = {0.5, 0.0, 0.0, 0.5, 0.5 , 0.0};
double IF3[6] = {0.5, 0.0, 0.0, 0.5, 0.25, 0.5};

void colorPoint(double x, double y)
{
	int i,j,k;
	double dx, dy;
	
	dx = 1.0/(double)n;
	dy = 1.0/(double)n;
	
	//Changing values to pixels. You need the small number because integers truncate they do not round. 
	//If you are really really close but just under you will be rounded down. 1.999999 goes to 1 even if you want it to go to 2.
	i = x/dx + 0.0000001;
	j = y/dy + 0.0000001;
	
	k = i*3.0 + j*3.0*window_width;
	pixels[k  ] = 1.0;
	pixels[k+1] = 0.0;
	pixels[k+2] = 0.0;
}

void iteratedFunction(double x, double y, double *f)
{
	int i,j,k;
	double dx, dy;
	
	dx = 1.0/(double)n;
	dy = 1.0/(double)n;
	
	globalX = x*f[0] + y*f[1] + f[4];
	globalY = x*f[2] + y*f[3] + f[5];
	
	colorPoint(globalX, globalY);
}

void randomIteratedFunctionSystem(double x, double y)
{
	int i, numberOfFunctions;

	numberOfFunctions = 3;
	
	//Select a random number 0,1, or 2
	i = rand() % numberOfFunctions;
	
	//Run (x,y) through the selected function
	if     (i == 0) iteratedFunction(x,y,IF1);
	else if(i == 1) iteratedFunction(x,y,IF2);
	else if(i == 2) iteratedFunction(x,y,IF3);
}

void initailize()
{
	int i,j,k;
	time_t t;
	
	//Intializes random number generator
	srand((unsigned) time(&t));
	
	//Creating space for the bitmap
	pixels = (float *)malloc(window_width*window_height*3*sizeof(float));
	
	//Color the initial bitmap all black
	for(j = 0; j < window_height; j++)
	{
		for(i = 0; i < window_width; i++)
		{
			k = i*3.0 + j*3.0*window_width;
			pixels[k]   = 0.0;
			pixels[k+1] = 0.0;
			pixels[k+2] = 0.0;
		}
	}
	
	//Set and coloring the initial point
	globalX = X0;
	globalY = Y0;
	colorPoint(globalX, globalY);
	
	//Pushing the initail bitmap to the screen
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);
	glFlush();
}

void controlCenter(void)
{
	int iter, go;
	
	initailize();
	
	for(iter =0; iter < NUMBER_OF_ITERATIONS; iter++)
	{
		randomIteratedFunctionSystem(globalX, globalY);
		
		//Pushing bitmap to the screen
		glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);
		glFlush();
	}
	printf("\nHit any key to terminate the program.\n");
	scanf("%d", &go);
	exit(0);
}

int main(int argc, char** argv)
{
   	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   	glutInitWindowSize(window_width, window_height);
   	glutCreateWindow("Hausdorff Contraction");
   	glutDisplayFunc(controlCenter);
   	glutMainLoop();
}
