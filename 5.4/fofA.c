//gcc fofA.c -o fofA -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>

#define SCALE 5.0

#define X_WINDOW 600
#define Y_WINDOW 600

#define X_MAX SCALE
#define X_MIN -SCALE
#define X_SCALE 1.0

#define Y_MAX SCALE
#define Y_MIN -SCALE
#define Y_SCALE 1.0

#define T_START 0.0
#define T_STOP 6.0

// function prototypes
void KeyPressed(unsigned char key, int x, int y);
void Display(void);

//globals
double g_x;
double g_y;
static int g_win;
int g_funCompNum = 0;

double rootObjectX(double x)
{
	return(sin(x));
}

double rootObjectY(double y)
{
	return(cos(y));
}

void modifyObject(double x, double y, int n)
{
	g_x = x;
	g_y = y;
	for(int i = 0; i < n; i++)
	{
		g_x = 2.0*g_x;
		g_y = 0.5*g_y;
	}
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

void drawModifiedObject(int n)
{
	double t, dt;
	double x1,y1,x2,y2;
	
	dt = (T_STOP - T_START)/1000.0;
	t = T_START;
	while(t < T_STOP)
	{
		x1 = rootObjectX(t);
		y1 = rootObjectY(t);
		modifyObject(x1,y1,n);
		x1 = g_x;
		y1 = g_y;
		
		x2 = rootObjectX(t+dt);
		y2 = rootObjectY(t+dt);
		modifyObject(x2,y2,n);
		x2 = g_x;
		y2 = g_y;
		
		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(x1),y_world_to_y_screen(y1));
			glVertex2f(x_world_to_x_screen(x2),y_world_to_y_screen(y2));
		glEnd();
		t += dt;
	}
	glFlush();
}

void mymouse(int button, int state, int x, int y)
{	
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(x<=10 && y <= 10)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				place_axis();
				glColor3f(1.0,0.0,0.0);
				drawModifiedObject(0);
			}
			else
			{
				g_funCompNum++;
				glColor3f(0.0,1.0,0.0);
				drawModifiedObject(g_funCompNum);
			}
		}
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
	place_axis();
	glColor3f(1.0,0.0,0.0);
	drawModifiedObject(0);
	glutMouseFunc(mymouse);
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
