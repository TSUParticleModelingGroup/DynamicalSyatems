//gcc matrixHitPlus.c -o matrixHitPlus -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>

#define SCALE 3.0

#define PI 3.14159265359

#define X_WINDOW 1000
#define Y_WINDOW 1000

#define X_MAX SCALE
#define X_MIN -SCALE
#define X_SCALE 1.0

#define Y_MAX SCALE
#define Y_MIN -SCALE
#define Y_SCALE 1.0

/* Eigenvalue 5/4 eigenvector <2,1> Eigenvalue 1/2 eigenvector <1,-1> 
#define damp (1.0/4.0)
#define A11 (4.0*damp)
#define A12 (2.0*damp)
#define A21 (1.0*damp)
#define A22 (3.0*damp)
*/

/* Eigenvalue 3/2 eigenvector <1,1> Eigenvalue -1/2 eigenvector <1,-1>
#define A11 (1.0/2.0)
#define A12 (1.0)
#define A21 (1.0)
#define A22 (1.0/2.0)
*/

/* Eigenvalue damp*3/2 eigenvector <1,1> Eigenvalue damp*(-1/2) eigenvector <1,-1>
#define damp (1.0/2.0)
#define A11 (1.0/2.0)*damp
#define A12 (1.0)*damp
#define A21 (1.0)*damp
#define A22 (1.0/2.0)*damp
*/

/* Eigenvalue -sqrt(2)+1 eigenvector <-sqrt(2),1> Eigenvalue sqrt(2)+1 eigenvector <sqrt(2),1>
#define A11 (1.0)
#define A12 (2.0)
#define A21 (1.0)
#define A22 (1.0)
*/

/* Eigenvalue sqrt(3)/2 eigenvector <sqrt(3)+1,1> Eigenvalue -sqrt(3)/2 eigenvector <-sqrt(3)+1,1>
#define A11 (1.0/2.0)
#define A12 (1.0)
#define A21 (1.0/2.0)
#define A22 (-1.0/2.0)
*/

/* Eigenvalue 1/2 + i*sqrt(2)/2 eigenvector <i*sqrt(2),1> Eigenvalue 1/2 - i*sqrt(2)/2 eigenvector <-i*sqrt(2),1>
#define A11 (1.0/2.0)
#define A12 (-1.0)
#define A21 (1.0/2.0)
#define A22 (1.0/2.0)
*/

/* Eigenvalue damp*(1+i*sqrt(2)) eigenvector <i*sqrt(2),1> Eigenvalue damp*(1-i*sqrt(2)) eigenvector <-i*sqrt(2),1>
#define damp 3.0/5.0
#define A11 (1.0*damp)
#define A12 (-2.0*damp)
#define A21 (1.0*damp)
#define A22 (1.0*damp)
*/

/* Eigenvalue 1 eigenvector <2,1> Eigenvalue -1 eigenvector <-2/3,1> 
#define A11 (1.0/2.0)
#define A12 (1.0)
#define A21 (3.0/4.0)
#define A22 (-1.0/2.0)
*/

/* Eigenvalue i eigenvector <-2-i,5> Eigenvalue -1 eigenvector <-2+i,5>*/
#define A11 (2.0)
#define A12 (1.0)
#define A21 (-5.0)
#define A22 (-2.0)


/* Eigenvalue 1 eigenvector <1,0> Eigenvalue 1 eigenvector <1,0> 
#define A11 (1.0)
#define A12 (1.0)
#define A21 (0.0)
#define A22 (1.0)
*/

/* Eigenvalue 1/2 eigenvector <3/2,1> Eigenvalue 1/2 eigenvector <3/2,1>
#define A11 (2.0)
#define A12 (-9.0/4.0)
#define A21 (1.0)
#define A22 (-1.0)
*/

/* Eigenvalue (-3*sqrt(2) + 15)/10 eigenvector <sqrt(2)+1,1> Eigenvalue (3*sqrt(2) + 15)/10 eigenvector <-sqrt(2)+1,1>
#define damp 8.0/29.0
#define A11 (2.0)*damp
#define A12 (-1.0/2.0)*damp
#define A21 (-1.0/2.0)*damp
#define A22 (3.0)*damp
*/

/* Eigenvalue 0.8 Eigenvalue 0.9 
#define A11 (0.8)
#define A12 (-0.9)
#define A21 (0.0)
#define A22 (0.9)*/

// function prototypes
void KeyPressed(unsigned char key, int x, int y);
void Display(void);

//globals
double g_x;
double g_y;
static int g_win;

double x_machine_to_x_screen(int x)
{
	return( (2.0*x)/X_WINDOW-1.0 );
}

double y_machine_to_y_screen(int y)
{
	return( -(2.0*y)/Y_WINDOW+1.0 );
}

double x_machine_to_x_world(int x)
{
	double range;
	range = X_MAX - X_MIN;
	return( (range/X_WINDOW)*x + X_MIN);
}

double y_machine_to_y_world(int y)
{
	double range;
	range = Y_MAX - Y_MIN;
	return(-((range/Y_WINDOW)*y - X_MAX));
}

double x_world_to_x_screen(double x)
{
	double range;
	range = X_MAX - X_MIN;
	return( -1.0 + 2.0*(x - X_MIN)/range );
}

double y_world_to_y_screen(double y)
{
	double range;
	range = Y_MAX - Y_MIN;
	return( -1.0 + 2.0*(y - Y_MIN)/range );
}

void hitMatrix()
{
	double xOld = g_x;
	double yOld = g_y;
	
	g_x = A11*xOld + A12*yOld;
	g_y = A21*xOld + A22*yOld;
}

void place_axis()
{
	glColor3f(0.0,0.0,1.0);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x_machine_to_x_screen(0),0);
		glVertex2f(x_machine_to_x_screen(X_WINDOW),0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex2f(0,y_machine_to_y_screen(0));
		glVertex2f(0,y_machine_to_y_screen(Y_WINDOW));
	glEnd();

	glFlush();
}

void drawUnitCircle()
{	
	int i;
	int lineAmount = 100;
	GLfloat twicePi = 2.0f * PI;
	
	glLineWidth(1.0);
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) 
		{ 
			glVertex2f(x_world_to_x_screen((1.0*cos(i*twicePi/lineAmount))), 
			           y_world_to_y_screen((1.0*sin(i*twicePi/lineAmount))));
		}
	glEnd();
}

void drawSpectralNorm()
{	
	int i;
	int lineAmount = 100;
	GLfloat twicePi = 2.0f * PI;
	
	glLineWidth(1.0);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) 
		{ 
			g_x = 1.0*cos(i*twicePi/lineAmount);
			g_y = 1.0*sin(i*twicePi/lineAmount);
			hitMatrix();
			glVertex2f(x_world_to_x_screen(g_x), 
			           y_world_to_y_screen(g_y));
		}
	glEnd();
}

void placePoint()
{
	glBegin(GL_POINTS);
		glVertex2f(x_world_to_x_screen(g_x),y_world_to_y_screen(g_y));
	glEnd();
	glFlush();
}

void printPoint()
{
	printf("\n  x = %f\n",g_x);
	printf("  y = %f\n",g_y);
}

void mymouse(int button, int state, int x, int y)
{	
	double x_double;
	double y_double;
	
	x_double = x;
	y_double = y;
	
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(x<=5 && y <= 5)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				place_axis();
				drawUnitCircle();
				drawSpectralNorm();
				glFlush();
			}
			else
			{
				glColor3f(1.0,1.0,0.0);
				g_x = x_machine_to_x_world(x);
				g_y = y_machine_to_y_world(y);
				placePoint(g_x,g_y);
				printPoint();
			}
		}
		else
		{
			glColor3f(0.0,1.0,0.0);
			hitMatrix(g_x,g_y);
			placePoint(g_x,g_y);
			printPoint();
		}
	}
}

void KeyPressed(unsigned char key, int x, int y)
{
	float tempx;
	float tempy;
	if(key == 'q')
	{
		glutDestroyWindow(g_win);
		exit(0);
	}
	if(key == 't')
	{
		printf("\n  Inter start x value\n");
		scanf("%f", &tempx);
		g_x = tempx;
		printf("\n  Inter start y value\n");
		scanf("%f", &tempy);
		g_y = tempy;
		
		glColor3f(1.0,0.0,0.0);
		placePoint(g_x,g_y);
		printPoint();
	}
}

void display()
{
	glPointSize(2.0);
	glClear(GL_COLOR_BUFFER_BIT);
	place_axis();
	drawUnitCircle();
	drawSpectralNorm();
	glFlush();
	glutMouseFunc(mymouse);
}


void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(X_WINDOW,Y_WINDOW);
	glutInitWindowPosition(0,0);
	g_win = glutCreateWindow("MATRIX HIT");
	glutKeyboardFunc(KeyPressed);
	glutDisplayFunc(display);
	glutMainLoop();
}
