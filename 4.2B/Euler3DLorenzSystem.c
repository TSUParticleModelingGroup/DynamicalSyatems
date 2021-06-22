//gcc Euler3DLorenzSystem.c -o temp -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359 

#define POINTS 10000 //number of points plotted

#define X_WINDOW 1000
#define Y_WINDOW 700

#define X_MAX 50.0
#define X_MIN 0.0
#define X_SCALE 1.0

#define Y_MAX 50.0
#define Y_MIN -50.0
#define Y_SCALE 10.0

#define ON  1
#define OFF 0 

#define X0 1.0
#define X1 1.0
#define X2 10.0

#define TSTART 0.0

#define DT 0.0001
#define TSTOP 50.0

#define ALF 10.0
#define B (8.0/3.0)
#define R 28.0

double x[3];
double dx[3];

/*	Takes machine x and y which start in the upper left corner and go from zero to X_WINDOW
	left to right and form zero to Y_WINDOW top to bottom and transslates this into screen 
	points which are a -1 to 1, -1 to 1 window.
*/
double x_machine_to_x_screen(int x)
{
	return( (2.0*x)/X_WINDOW-1.0 );
}

double y_machine_to_y_screen(int y)
{
	return( -(2.0*y)/Y_WINDOW+1.0 );
}

/*	Takes machine x and y which start in the upper left corner and go from zero to X_WINDOW
	left to right and form zero to Y_WINDOW top to bottom and transslates this into world 
	points which are a X_MIN to X_MAX, Y_MIN to Y_MAX window.
*/
double x_machine_to_x_world(int x)
{
	double range;
	range = X_MAX - X_MIN;
	return( (range/X_WINDOW)*x + X_MIN );
}

double y_machine_to_y_world(int y)
{
	double range;
	range = Y_MAX - Y_MIN;
	return(-((range/Y_WINDOW)*y - Y_MAX));
}

/*	Take world  points to screen points 
*/
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

void model()
{
	dx[0] = ALF*(x[1]-x[0]);
	dx[1] = R*x[0]-x[1]-x[0]*x[2];
	dx[2] = x[0]*x[1]-B*x[2];
}

void euler(double x1start,double x2start, double x3start)
{
	double t,tn,x0n,x1n,x2n;

	x[0] = x1start;
	x[1] = x2start;
	x[2] = x3start;
	
	t = TSTART;

	while(t <= TSTOP)
	{
		model();

		x0n = dx[0]*DT + x[0];
		x1n = dx[1]*DT + x[1];
		x2n = dx[2]*DT + x[2];
		
		tn = t + DT;

		if(ON)
		{
			glColor3f(1.0,1.0,0.0);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(t), y_world_to_y_screen(x[0]));
				glVertex2f(x_world_to_x_screen(tn),y_world_to_y_screen(x0n));
			glEnd();
		}

		if(OFF)
		{
			glColor3f(0.0,1.0,1.0);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(t), y_world_to_y_screen(x[1]));
				glVertex2f(x_world_to_x_screen(tn),y_world_to_y_screen(x1n));
			glEnd();
		}

		if(OFF)
		{
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(t), y_world_to_y_screen(x[2]));
				glVertex2f(x_world_to_x_screen(tn),y_world_to_y_screen(x2n));
			glEnd();
		}

		x[0] = x0n;
		x[1] = x1n;
		x[2] = x2n; 

		t = tn;
	}
	glFlush();
}

/* Testing to see if all the settings make sence
*/
int test_settings()
{
	int test;

	test = 1;

	if(X_MAX <= X_MIN) 
	{
		printf("X_MAX and X_MIN are out of wack\n");
		test = 0;
	}
	if(Y_MAX <= Y_MIN) 
	{
		printf("Y_MAX and Y_MIN are out of wack\n");
		test = 0;
	}
	if(X_SCALE <= 0.0 || Y_SCALE <= 0.0) 
	{
		printf("X_SCALE and Y_SCALE must both be positive\n");
		test = 0;
	}
	return(test);
}

void place_axis()
{
	glColor3f(0.0,0.0,1.0);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(X_MIN),y_world_to_y_screen(0.0));
		glVertex2f(x_world_to_x_screen(X_MAX),y_world_to_y_screen(0.0));
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(0.0),y_world_to_y_screen(Y_MIN));
		glVertex2f(x_world_to_x_screen(0.0),y_world_to_y_screen(Y_MAX));
	glEnd();
}

void place_hash_marks()
{
	double x,y,dx,dy;

	glColor3f(1.0,1.0,1.0);

	dx = X_SCALE;
	dy = Y_SCALE;

	x = X_MIN;
	while(x <= X_MAX)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(x), 0.005+y_world_to_y_screen(0));
			glVertex2f(x_world_to_x_screen(x),-0.005+y_world_to_y_screen(0));
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
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	test_settings();

	place_axis();

	place_hash_marks();

	glFlush();

	euler(X0,X1,X2);
}


int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(1000,700);
	glutInitWindowPosition(0,0);
	glutCreateWindow("BOX");
	glutDisplayFunc(display);
	glutMainLoop();
}

