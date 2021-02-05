//gcc Collatz3x+1Graphical.c -o temp -lglut -lGL -lm
//nvcc Collatz3x+1Graphical.c -o temp -lglut -lGL -lm
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define X_WINDOW 1000
#define Y_WINDOW 700

#define X_MAX 10.0
#define X_MIN -1.0
#define X_SCALE 1.0

#define Y_MAX 10.0
#define Y_MIN -10.0
#define Y_SCALE 1.0

#define DELTA_X 0.1

// globals
double x_world, y_world;
float XPresent; 
int YPresent;
int Count = 0;
int DisplayCount = 0;

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

void place_axis()
{
	glClear(GL_COLOR_BUFFER_BIT);
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

void place_point(float x, int y)
{
	glPointSize(5.0);
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen((float)y));
	glEnd();
}

int Collatz(int x)
{
	if(x%2 == 0) return(x/2);
	else return(3*x + 1);
}

void mymouse(int button, int state, int x, int y)
{	
	float deltaX = DELTA_X;
	
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			place_axis();
			place_hash_marks();

			XPresent = 0.0;
			YPresent = (int)y_machine_to_y_world(y);
			Count = 0;
			printf("\n\n  Collatz[%d] = %d\n", Count, YPresent);

			place_point(XPresent, YPresent);

			glFlush();
		}
		else
		{
			YPresent = Collatz(YPresent);
			XPresent += deltaX;
			Count++;
			place_point(XPresent, YPresent);
			glFlush();
			printf("  Collatz[%d] = %d\n", Count, YPresent);
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if(DisplayCount == 0)
	{
		printf("\n\n  Collatz[%d] = %d\n", Count, YPresent);
		DisplayCount++;
	}
	
	place_axis();
	place_hash_marks();
	place_point(XPresent, YPresent);
	glFlush();

	glutMouseFunc(mymouse);
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("\n  Too few arguments!");
		printf("\n  You need to enter a starting value.");
		printf("\n  ./function x0 \n\n");
		exit(0);
	}
	else if(argc == 2)
	{
		YPresent = atoi(argv[1]);
		XPresent = 0;
	}
	else
	{
		printf("\n  Too many arguments!");
		printf("\n  You need to enter a starting value.");
		printf("\n  ./function x0 \n\n");
		exit(0);
	}
	
	test_settings();
	
	glutInit(&argc,argv);
	glutInitWindowSize(X_WINDOW,Y_WINDOW);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Collatz 3x + 1");
	glutDisplayFunc(display);
	glutMainLoop();
}
