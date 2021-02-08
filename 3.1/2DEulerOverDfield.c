//gcc 2DEulerOverDfield.c -o 2DEuler -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>
  
#define X_WINDOW 1000
#define Y_WINDOW 600

#define GRIDY 50
#define GRIDX 50

#define SCALE 6.0

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
static int g_win;
double g_x;
double g_y;
double g_dx;
double g_dy;
double g_timeStop = 400.0;
double euler_dt = 0.0001;
int CascadeIterations = 100;

double def_xy(double x, double y)
{
	//g_dx = (y*y - x*x -x -3.0)/2.0;
	//g_dy = (x + y + 1.0)/2.0;
	
	//g_dx = x*x*x - y;
	//g_dy = x + y;
	
	//g_dx = y*y - x;
	//g_dy = x*x - y;
	
	//g_dx = x - y*y;
	//g_dy = x + y - 2.0;
	
	//g_dx = sin(y);
	//g_dy = x + y;
	
	g_dx = sin(y);
	g_dy = cos(x);
	
	//g_dx = 3.0*y - exp(x);
	//g_dy = 2.0*x - y;
}

double x_machine_to_x_screen(int x)
{
	return( (2.0*(double)x)/(double)X_WINDOW - 1.0 );
}

double y_machine_to_y_screen(int y)
{
	return( -(2.0*(double)y)/(double)Y_WINDOW + 1.0 );
}

double x_machine_to_x_world(int x)
{
	double range;
	range = X_MAX - X_MIN;
	return( (range/(double)X_WINDOW)*(double)x + X_MIN);
}

double y_machine_to_y_world(int y)
{
	double range;
	range = Y_MAX - Y_MIN;
	return(-((range/(double)Y_WINDOW)*(double)y - (double)Y_MAX));
}

double x_world_to_x_screen(double x)
{
	double range;
	range = X_MAX - X_MIN;
	return( -1.0 + 2.0*(x - (double)X_MIN)/range );
}

double y_world_to_y_screen(double y)
{
	double range;
	range = Y_MAX - Y_MIN;
	return( -1.0 + 2.0*(y - (double)Y_MIN)/range );
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

void placePoint(double x, double y)
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(y));
	glEnd();
	glFlush();
}

void d_field()
{
	int i,j;
	double y,x,y2,x2,dx,dy,stepx,stepy,mag, lt;
	
	stepy = (Y_MAX-Y_MIN)/GRIDY;
	stepx = (X_MAX-X_MIN)/GRIDX;

	lt= 0.4*sqrt(stepx*stepx + stepy*stepy);

	glPointSize(2.0);
	for(i=0; i<GRIDY; i++)
	{		
		for(j=0; j<GRIDX; j++)
		{		
			y = Y_MIN + i*stepy;
			x = X_MIN + j*stepx;
			
			glColor3f(0.0,1.0,0.0);
			glBegin(GL_POINTS);
				glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(y));
			glEnd();
			
			def_xy(x,y);
			dx = g_dx;
			dy = g_dy;
			
			mag = sqrt(dx*dx + dy*dy);
			dx = lt*dx/mag;
			dy = lt*dy/mag;
			
			x2 = x + dx;
			y2 = y + dy;
			
			glColor3f(0.8,0.0,0.1);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(y));
				glVertex2f(x_world_to_x_screen(x2),y_world_to_y_screen(y2));
			glEnd();
		}
	}

	glFlush();
}

void euler(double x, double y, double tstop)
{
	double sl,dt,yn,xn,time;

	time = 0.0;
	dt = euler_dt;

	while(time <= tstop)
	{	
		def_xy(x, y);

		xn = x + g_dx*dt;
		yn = y + g_dy*dt;

		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(y));
			glVertex2f(x_world_to_x_screen(xn),y_world_to_y_screen(yn));
		glEnd();

		x = xn;
		y = yn;
		time += dt;
	}
	glFlush();
}

void mymouse(int button, int state, int x, int y)
{	
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(x<=5 && y <= 5)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				place_axis();
				d_field();
			}
			else
			{
				g_x = x_machine_to_x_world(x);
				g_y = y_machine_to_y_world(y);
				glColor3f(1.0,1.0,0.0);
				placePoint(g_x,g_y);
				glColor3f(0.0,1.0,0.0);
				euler(g_x,g_y,g_timeStop);
			}
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
		printf("\nInter start x value\n");
		scanf("%f", &tempx);
		g_x = tempx;
		printf("\nInter start y value\n");
		scanf("%f", &tempy);
		g_y = tempy;
		
		glColor3f(1.0,1.0,0.0);
		placePoint(g_x,g_y);
		glColor3f(0.0,1.0,0.0);
		euler(g_x,g_y,g_timeStop);
	}
}

void display()
{
	glPointSize(2.0);
	glClear(GL_COLOR_BUFFER_BIT);
	place_axis();
	d_field();
	glutMouseFunc(mymouse);
}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(X_WINDOW,Y_WINDOW);
	glutInitWindowPosition(0,0);
	g_win = glutCreateWindow("Euler's Over DField");
	glutKeyboardFunc(KeyPressed);
	glutDisplayFunc(display);
	glutMainLoop();
}
