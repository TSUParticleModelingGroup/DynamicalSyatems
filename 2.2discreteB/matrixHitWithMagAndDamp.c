//gcc matrixHitWithMagAndDamp.c -o matrixHit -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <GL/glu.h>
#include <GL/gl.h>
//#include <device.h>

#define SCALE 50.0

#define X_WINDOW 1000
#define Y_WINDOW 1000

#define X_MAX SCALE
#define X_MIN -SCALE
#define X_SCALE 1.0

#define Y_MAX SCALE
#define Y_MIN -SCALE
#define Y_SCALE 1.0

// ************* Enter your magnitude and damping here by removing the coment lines ***************

// Eigenvalue 5 eigenvector <2, 1> 
// Eigenvalue 2 eigenvector <1,-1>

/*
#define A11 (4.0)
#define A12 (2.0)
#define A21 (1.0)
#define A22 (3.0)
/* */

// Eigenvalue  3 eigenvector <1, 1> 
// Eigenvalue -1 eigenvector <1,-1>

/*
#define A11 (1.0)
#define A12 (2.0)
#define A21 (2.0)
#define A22 (1.0)
/* */

// Eigenvalue 4 eigenvector <2,1> 
// Eigenvalue -4 eigenvector <-2/3,1>

/*
#define A11 (2.0)
#define A12 (4.0)
#define A21 (3.0)
#define A22 (-2.0)
/* */

// Eigenvalue  i eigenvector <-2-i,5> mag 1 angle 90
// Eigenvalue -i eigenvector <-2+i,5> mag 1 angle 270

/*
#define A11 (2.0)
#define A12 (1.0)
#define A21 (-5.0)
#define A22 (-2.0)
/* */

// Eigenvalue 1 + i*sqrt(2) eigenvector < i*sqrt(2),1> mag sqrt(3) angle  39.23
// Eigenvalue 1 - i*sqrt(2) eigenvector <-i*sqrt(2),1> mag sqrt(3) angle 320.77

///*
#define A11 (1.0)
#define A12 (-2.0)
#define A21 (1.0)
#define A22 (1.0) 
/* */


// function prototypes
void KeyPressed(unsigned char key, int x, int y);
void Display(void);

//globals
double g_x;
double g_y;
static int g_win;

double g_damp;
double g_mag; 
double g_normal;

// ************* Sellect your magnitude removing the comment (//) and enter damping here ***************
void setGlobals()
{
	g_damp = -1.1;
	//g_mag = 5;
	//g_mag = 3;
	//g_mag = 4;
	//g_mag = 1;
	g_mag = sqrt(3); 
	g_normal = 1.0/g_mag;
}

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

void placePoint()
{
	glBegin(GL_POINTS);
		glVertex2f(x_world_to_x_screen(g_x),y_world_to_y_screen(g_y));
	glEnd();
	glFlush();
}

void hitMatrix()
{
	double xOld = g_x;
	double yOld = g_y;
	
	g_x = A11*g_damp*g_normal*xOld + A12*g_damp*g_normal*yOld;
	g_y = A21*g_damp*g_normal*xOld + A22*g_damp*g_normal*yOld;
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
	setGlobals();
	
	glPointSize(2.0);
	
	glClear(GL_COLOR_BUFFER_BIT);

	place_axis();

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
