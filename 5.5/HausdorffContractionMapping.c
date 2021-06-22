//gcc HausdorffContractionMapping.c -o htemp -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#define N 1000

unsigned int window_width = N;
unsigned int window_height = N;
unsigned int n = N;
float *pixels1;
float *pixels2;

//Sirpenski triangle
//double IF1[6] = {0.5, 0.0, 0.0, 0.5, 0.0, 0.0};
//double IF2[6] = {0.5, 0.0, 0.0, 0.5, 0.5, 0.0};
//double IF3[6] = {0.5, 0.0, 0.0, 0.5, 0.25, 0.5};

//*** square
double IF1[6] = {0.3333, 0.0, 0.0, 0.3333, 0.0, 0.0};
double IF2[6] = {0.3333, 0.0, 0.0, 0.3333, 0.3333, 0.0};
double IF3[6] = {0.3333, 0.0, 0.0, 0.3333, 0.6666, 0.0};
double IF4[6] = {0.3333, 0.0, 0.0, 0.3333, 0.0, 0.3333};
double IF5[6] = {0.3333, 0.0, 0.0, 0.3333, 0.6666, 0.3333};
double IF6[6] = {0.3333, 0.0, 0.0, 0.3333, 0.0, 0.6666};
double IF7[6] = {0.3333, 0.0, 0.0, 0.3333, 0.3333, 0.6666};
double IF8[6] = {0.3333, 0.0, 0.0, 0.3333, 0.6666, 0.6666};

void iteratedFunction(double x, double y, double *f)
{
	int i,j,k;
	double dx, dy, xn, yn;
	
	dx = 1.0/(double)n;
	dy = 1.0/(double)n;
	
	xn = x*f[0] + y*f[1] + f[4];
	yn = x*f[2] + y*f[3] + f[5];
	//Changing values to pixels. You need to and the small number because integers truncate they do not round. 
	//If you are really really close but just under you will be rounded down. 1.999999 goes to 1 even if you want it to go to 2.
	i = xn/dx + 0.0000001;
	j = yn/dy + 0.0000001;
	
	k = i*3.0 + j*3.0*window_width;
	pixels2[k] = 1.0;
	pixels2[k+1] = 0.0;
	pixels2[k+2] = 0.0;
}

void iteratedFunctionSystem(double x, double y)
{
	iteratedFunction(x,y,IF1);
	iteratedFunction(x,y,IF2);
	iteratedFunction(x,y,IF3);
	iteratedFunction(x,y,IF4);
	iteratedFunction(x,y,IF5);
	iteratedFunction(x,y,IF6);
	iteratedFunction(x,y,IF7);
	iteratedFunction(x,y,IF8);
}

void display(void)
{
	double x,y,dx,dy;
	int i,j,k, iter, go;

	//Creating space for the bitmaps
	pixels1 = (float *)malloc(window_width*window_height*3*sizeof(float));
	pixels2 = (float *)malloc(window_width*window_height*3*sizeof(float));
	
	//Color the initial bitmap red and putting it on the scream
	//for(j = 0; j < window_height; j++)
	for(j = 0; j < 1; j++)
	{
		//for(i = 0; i < window_width; i++)
		for(i = 0; i < 1; i++)
		{
			k = i*3.0 + j*3.0*window_width;
			pixels1[k]   = 1.0;
			pixels1[k+1] = 0.0;
			pixels1[k+2] = 0.0;
		}
	}
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels1);
	glFlush();
	printf("\nHit 1 to run another iteration.\nHit 0 to terminate the program.\n");
	scanf("%d", &go);
	if(go == 0) exit(0);
	
	dx = 1.0/(double)n;
	dy = 1.0/(double)n;
	
	for(iter =0; iter < 300; iter++)
	{
		//Coloring the working bitmap black
		for(j = 0; j < window_height; j++)
		{
			for(i = 0; i < window_width; i++)
			{
				k = i*3.0 + j*3.0*window_width;
				pixels2[k] = 0.0;
				pixels2[k+1] = 0.0;
				pixels2[k+2] = 0.0;
			}
		}
		
		//Check every pixel if it used (red) then sending it to the IFS to go through an iteration
		for(j = 0; j < window_height; j++)
		{
			for(i = 0; i < window_width; i++)
			{
				k = i*3.0 + j*3.0*window_width;
				if(pixels1[k] == 1.0)
				{
					//Creating x and y values from pixels 
					x = i*dx;
					y = j*dy;
					iteratedFunctionSystem(x,y);
				}
			}
		}
		
		//Replacing the viewing bitmap with the working bitmap and pushing it to the screen
		for(j = 0; j < window_height; j++)
		{
			for(i = 0; i < window_width; i++)
			{
				k = i*3.0 + j*3.0*window_width;
				pixels1[k]   = pixels2[k];
				pixels1[k+1] = pixels2[k+1];
				pixels1[k+2] = pixels2[k+2];
			}
		}
		glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels1);
		glFlush();
		
		printf("\nHit 1 to run another iteration.\nHit 0 to terminate the program.\n");
		scanf("%d", &go);
		if(go == 0) exit(0);
	}
	//while(1);
}

int main(int argc, char** argv)
{
   	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   	glutInitWindowSize(window_width, window_height);
   	glutCreateWindow("BitMap");
   	glutDisplayFunc(display);
   	glutMainLoop();
}
