//gcc discrete2D.c -o discrete2D -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>

#define SCALE 5.0

#define X_WINDOW 1000
#define Y_WINDOW 1000

#define X_MAX SCALE
#define X_MIN -SCALE
#define X_SCALE 1.0

#define Y_MAX SCALE
#define Y_MIN -SCALE
#define Y_SCALE 1.0

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
	glPointSize(5.0);
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(x_world_to_x_screen(g_x),y_world_to_y_screen(g_y));
	glEnd();
	glFlush();
}

void f()
{
	double xOld = g_x;
	double yOld = g_y;
	
	g_x = (g_x + g_y)/2.0;
	g_y = 4.0*cos(g_x);
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
	double xTemp, yTemp;
	
	x_double = x;
	y_double = y;
	
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(x<=10 && y <= 10)
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
			xTemp = g_x;
			yTemp = g_y;
			f();
			
			glColor3f(1.0,1.0,0.0);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(xTemp), y_world_to_y_screen(yTemp));
				glVertex2f(x_world_to_x_screen(g_x), y_world_to_y_screen(g_y));
			glEnd();
			
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
