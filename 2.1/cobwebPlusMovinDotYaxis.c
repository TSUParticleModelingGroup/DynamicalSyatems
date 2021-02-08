//gcc cobwebPlusMovinDotYaxis.c -o cobweb -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359 

#define POINTS 10000 //number of points plotted

#define X_WINDOW 1000
#define Y_WINDOW 700

#define X_MAX 10.0
#define X_MIN -10.0
#define X_SCALE 1.0

#define Y_MAX 10.0
#define Y_MIN -10.0
#define Y_SCALE 1.0

// M is the slope
//#define M (-1.0/2.0) 
//#define M (1.0/2.0) 
//#define M (1.0)  
//#define M (-1.0) 
//#define M (1.1)         
//#define M (-1.02) 
#define M (-0.99) 
//#define M (0.0) 

//B is the y intercept 
#define B (2.0) 
//#define B (0.0) 

// globals
double x_world, y_world;
double present_x, present_y;

/*	Function to be drawn
*/
double f(double x)
{
	return(M*x+B);
}

/* Testing to see if all the settings make sence
*/
void test_settings()
{
	if(X_MAX <= X_MIN) 
	{
		printf("\n\n  X_MAX and X_MIN are out of wack.\n");
		printf("\n  Bye \n\n");
		exit(0);
	}
	if(Y_MAX <= Y_MIN) 
	{
		printf("\n\n  Y_MAX and Y_MIN are out of wack.\n");
		printf("\n  Bye \n\n");
		exit(0);
	}
	if(X_SCALE <= 0.0 || Y_SCALE <= 0.0) 
	{
		printf("\n\n  X_SCALE and Y_SCALE must both be positive.");
		printf("\n  Bye \n\n");
		exit(0);
	}
}

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

/*	Places POINTS number of input/output pairs from X_MIN to X_MAX
*/
void draw_function()
{
	int i;
	double range,dx;
	range = X_MAX - X_MIN;

	dx = range/POINTS;

	glPointSize(1.0);
	glColor3f(1.0,1.0,0.0);

	for(i=0; i<POINTS; i++)
	{
		glBegin(GL_POINTS);
			glVertex2f(x_world_to_x_screen(X_MIN + dx*i),y_world_to_y_screen(f(X_MIN + dx*i)));
		glEnd();
	}
	glFlush();
}

void cobweb(double x, double y)
{
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(y));
		glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(f(x)));
	glEnd();

	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(f(x)));
		glVertex2f(x_world_to_x_screen(f(x)),y_world_to_y_screen(f(x)));
	glEnd();
	glFlush();

	present_x = f(x);
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

	glFlush();
}

void place_y_equals_x_line()
{
	glColor3f(1.0,1.0,1.0);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(X_MIN),y_world_to_y_screen(Y_MIN));
		glVertex2f(x_world_to_x_screen(X_MAX),y_world_to_y_screen(Y_MAX));
	glEnd();

	glFlush();
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

void mymouse(int button, int state, int x, int y)
{	
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			place_axis();
			place_y_equals_x_line();
			place_hash_marks();
			draw_function();
			present_x = x_machine_to_x_world(x);
			
			glPointSize(5.0);
			glColor3f(1.0,1.0,0.0);
			glBegin(GL_POINTS);
				glVertex2f(x_world_to_x_screen(0.0),y_world_to_y_screen(present_x));
			glEnd();

			cobweb(present_x, 0.0);

			glFlush();
			
			printf("\n**************************************************\n");
			printf("\n  New run starting at %f", present_x);
		}
		else
		{
			glPointSize(5.0);
			glColor3f(1.0,1.0,0.0);
			glBegin(GL_POINTS);
				glVertex2f(x_world_to_x_screen(0.0),y_world_to_y_screen(present_x));
			glEnd();

			cobweb(present_x,present_x);

			glFlush();
			
			printf("\n  Next value = %f", present_x);
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	place_axis();

	place_y_equals_x_line();

	place_hash_marks();

	draw_function();

	glutMouseFunc(mymouse);
}

int main(int argc, char** argv)
{
	test_settings();
	glutInit(&argc,argv);
	glutInitWindowSize(X_WINDOW,Y_WINDOW);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Cobweb");
	glutDisplayFunc(display);
	glutMainLoop();
}

