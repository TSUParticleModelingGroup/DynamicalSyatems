// gcc EulerOverDirectionFields1D.c -o Eulers -lglut -lm -lGL
/* Set the first order D.E. that you would like to test along with its peramiters then left
   click to run an Euler's method starting at the mouse point or right click to graph a cascade 
   of Euler's methods above and below the mouse click, Click in the upper left corner to clear
   the screen.
*/
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359 

#define POINTS 10000 //number of points plotted

#define X_WINDOW 1000
#define Y_WINDOW 700

#define X_MAX 10.0
#define X_MIN -1.0
#define X_SCALE 1.0

#define Y_MAX 10.0
#define Y_MIN -1.0
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

/* Grid settings for the direction fields
*/
#define f_GRID 100
#define t_GRID 100

/* Set the time duration and step size here.
*/
#define TIME_DURATION  20.0
#define STEP_SIZE      0.001

// globals
double x_world, y_world;

/*	The function F in f'=F(f(t),t)
*/
double derivative_function(double f, double t)
{
	return(M*f + B);
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

/* Euler's method
*/
void euler(double f_old, double t_old)
{
	double t_new,f_new;
	double slope,delta_time,stop_time;

	stop_time = t_old + TIME_DURATION;
	
	delta_time = STEP_SIZE;

	while(t_old <= stop_time)
	{	
		slope = derivative_function(f_old,t_old);

		f_new = f_old + slope*delta_time;
		t_new = t_old + delta_time;

		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(t_old), y_world_to_y_screen(f_old));
			glVertex2f(x_world_to_x_screen(t_new), y_world_to_y_screen(f_new));
		glEnd();

		f_old = f_new;
		t_old = t_new;
	}
}

/* does an aray of Euler's method up and down from the mouse click
*/
void euler_cascade(double t)
{
	double f,jump;
	int i;

	jump = (Y_MAX - Y_MIN)/f_GRID;

	for(i=0; i < f_GRID; i++)
	{
		f = Y_MIN + i*jump;
		euler(f,t);
	}
}

/*	Draws the direction fields produced by df(f,t)
*/
void direction_fields()
{
	int i,j;
	double f1,f2,t1,t2,f_spacing,t_spacing;
	double length,slope,unit;

	f_spacing = (Y_MAX - Y_MIN)/f_GRID;
	t_spacing = (X_MAX - X_MIN)/t_GRID;

	/* setting the length of the direction vector to be the length of the smaller grid size
	*/
	if(f_spacing < t_spacing) length = f_spacing;
	else                      length = t_spacing;
	length = length*0.8;

	glColor3f(0.8,0.0,0.1);

	/* Drawing the vectors
	*/
	for(i=0; i < f_GRID; i++)
	{		
		for(j=0; j < t_GRID; j++)
		{		
			f1 = Y_MIN + i*f_spacing;
			t1 = X_MIN + j*t_spacing;
			
			slope = derivative_function(f1,t1);
			unit = sqrt(1.0 + slope*slope);
			
			f2 = f1 + length*slope/unit;
			t2 = t1 + length/unit;
			

			glColor3f(0.8,0.0,0.1);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(t1), y_world_to_y_screen(f1));
				glVertex2f(x_world_to_x_screen(t2), y_world_to_y_screen(f2));
			glEnd();
		}
	}

	/* Placing a point at the start of the vector
	*/
	glPointSize(1.0);
	glColor3f(0.0,1.0,0.0);
	for(i=0; i < f_GRID; i++)
	{		
		for(j=0; j < t_GRID; j++)
		{		
			f1 = Y_MIN + i*f_spacing;
			t1 = X_MIN + j*t_spacing;

			glBegin(GL_POINTS);
				glVertex2f(x_world_to_x_screen(t1), y_world_to_y_screen(f1));
			glEnd();
		}
	}
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
	if(f_GRID <= 0.0 || t_GRID <= 0.0) 
	{
		printf("f_GRID and t_GRID must both be positive\n");
		test = 0;
	}
	if(TIME_DURATION*STEP_SIZE <= 0.0) 
	{
		printf("TIME_DURATION and STEP_SIZE must both be of the same sign and nonzero\n");
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

void mymouse(int button, int state, int t, int f)
{	
//	double t,f;

	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(t<=5 && f <= 5)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				direction_fields();
				place_axis();
				place_hash_marks();
				glFlush();
			}
			else
			{
				glColor3f(1.0,1.0,1.0);
				euler(y_machine_to_y_world(f),x_machine_to_x_world(t));
				glFlush();
			}
		}
		else
		{
			glColor3f(1.0,1.0,0.0);
			euler_cascade(x_machine_to_x_world(t));
			glFlush();
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	direction_fields();

	place_axis();

	place_hash_marks();

	glFlush();

	glutMouseFunc(mymouse);
}

int main(int argc, char** argv)
{
	if(test_settings() == 1)
	{
	glutInit(&argc,argv);
	glutInitWindowSize(X_WINDOW,Y_WINDOW);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Euler's Over Direction Fields 1D");
	glutDisplayFunc(display);
	glutMainLoop();
	}
}
