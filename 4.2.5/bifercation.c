//gcc bifercation.c -o bifercation -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>

#define SCALE 0.25

#define X_WINDOW 1500
#define Y_WINDOW 1000

#define X_MAX 5.0
#define X_MIN -5.0
#define X_SCALE SCALE

#define Y_MAX 5.0
#define Y_MIN -5.0
#define Y_SCALE SCALE

// function prototypes
void KeyPressed(unsigned char key, int x, int y);
void Display(void);

//globals
double g_x;
double g_y;
static int g_win;
int g_funCompNum = 1;
double g_parm;

double f(double x)
{
	return(x*x + g_parm);
	//return(g_parm*x*(1.0-x));
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
	return(-((range/Y_WINDOW)*y - Y_MAX));
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
	glColor3f(1.0,1.0,1.0);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(X_MIN),y_world_to_y_screen(0.0));
		glVertex2f(x_world_to_x_screen(X_MAX),y_world_to_y_screen(0.0));
	glEnd();
	
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(0.0),y_world_to_y_screen(Y_MIN));
		glVertex2f(x_world_to_x_screen(0.0),y_world_to_y_screen(Y_MAX));
	glEnd();

	glFlush();
}

void place_hash_marks()
{
	double x,y,dx,dy;

	glColor3f(1.0,1.0,0.0);

	dx = X_SCALE;
	dy = Y_SCALE;

	x = X_MIN;
	while(x <= X_MAX)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(x), 0.01+y_world_to_y_screen(0));
			glVertex2f(x_world_to_x_screen(x),-0.01+y_world_to_y_screen(0));
		glEnd();

		x = x + dx;
	}

	y = Y_MIN;
	while(y <= Y_MAX)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2f( 0.005+x_world_to_x_screen(0),y_world_to_y_screen(y));
			glVertex2f(-0.005+x_world_to_x_screen(0),y_world_to_y_screen(y));
		glEnd();

		y = y + dy;
	}
	glFlush();
}

void drawPoints()
{
	double x;
	int i;
	
	x = 0.01;
	for(i = 0; i < 100; i++)
	{
		x = f(x);
	}
	
	for(i = 0; i < 1000; i++)
	{
		x = f(x);
		glBegin(GL_POINTS);
			glVertex2f(x_world_to_x_screen(g_parm),y_world_to_y_screen(x));
		glEnd();
	}
	glFlush();
}

void moveThroughParm()
{
	double y, x, dx;
	
	glPointSize(1.0);
	glColor3f(1.0,0.0,0.0);
	
	dx = (X_MAX - X_MIN)/10000.0;
	g_parm = X_MAX;
	while(g_parm > X_MIN)
	{
		drawPoints();
		g_parm -= dx;
	}
	glFlush();
}

void mymouse(int button, int state, int x, int y)
{	
	if(state == GLUT_DOWN)
	{
		printf("\nParmeter = %f y value = %f\n",x_machine_to_x_world(x), y_machine_to_y_world(y));
	}
}

void KeyPressed(unsigned char key, int x, int y)
{
	float tempx;
	int n;

	if(key == 'q')
	{
		glutDestroyWindow(g_win);
		exit(0);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	moveThroughParm();
	glutMouseFunc(mymouse);
	place_axis();
	place_hash_marks();
	while(1);
}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(X_WINDOW,Y_WINDOW);
	glutInitWindowPosition(0,0);
	g_win = glutCreateWindow("COBWEB");
	glutKeyboardFunc(KeyPressed);
	glutDisplayFunc(display);
	glutMainLoop();
}
