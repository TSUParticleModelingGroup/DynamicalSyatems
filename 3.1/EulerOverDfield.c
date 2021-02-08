//gcc EulerOverDfield.c -o Euler -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>

#define X_WINDOW 1000
#define Y_WINDOW 800

#define GRIDY 70
#define GRIDT 70

#define SCALE 5.0

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
double g_timeStop = 100.0;
double euler_dt = 0.0001;
int CascadeIterations = 100;

double def_f(double t, double f)
{
	//return(-f/((t-2)*(t-2)) + t);
	//return(1.0*f/(t*t-4.0));
	//return(t/f + 1.0);
	//return(-0.2*f -2.0);
	//return(f*f*f -cos(f) - 2.0);
	//return(-0.1*f -sin(t) - 2.0);
	//return(f*f + f);
	//return(0.1*(f+2.0)*(f+1.0)*(f-1.0)*(f-2.0));
	//return(f/sin(t) +t);
	// return((3*f-t)/(t*t-9));
	//return((f)/(cos(t)));
	
	//return(f*f-f-1.0);
	return(sin(f));
	//return(exp(f) - 1.0);
	//return(log(f*f));
	//return(f/(1-f));
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
	return( (range/(double)X_WINDOW)*(double)x + (double)X_MIN);
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
	double y,t,y2,t2,sl,dy,dt,th,lt;
	
	dy = (Y_MAX-Y_MIN)/GRIDY;
	dt = (X_MAX-X_MIN)/GRIDT;

	lt= 0.5*sqrt(dy*dy +dt*dt);

	glColor3f(0.8,0.0,0.1);
	for(i=0; i<GRIDY; i++)
	{		
		for(j=0; j<GRIDT; j++)
		{		
			y = Y_MIN + i*dy;
			t = X_MIN + j*dt;
			
			sl = def_f(t,y);

			th = atan(sl);
			y2 = y + lt*sin(th);
			t2 = t + lt*cos(th);
			
			glColor3f(0.8,0.0,0.1);
			glBegin(GL_LINE_LOOP);
				glVertex2f(x_world_to_x_screen(t),y_world_to_y_screen(y));
				glVertex2f(x_world_to_x_screen(t2),y_world_to_y_screen(y2));
			glEnd();
		}
	}

	glPointSize(1.0);
	glColor3f(0.0,1.0,0.0);
	for(i=0; i<GRIDY; i++)
	{		
		for(j=0; j<GRIDT; j++)
		{		
			y = Y_MIN + i*dy;
			t = X_MIN + j*dt;

			glBegin(GL_POINTS);
				glVertex2f(x_world_to_x_screen(t),y_world_to_y_screen(y));
			glEnd();
		}
	}
	glFlush();
}

void euler(double t0, double y0, double tstop)
{
	double sl,dt,yn,tn,tstart;

	tstart = 0.0;
	
	//dt = (MAXT-MINT)/(GRIDT);
	
	dt = euler_dt;

	while(tstart <= tstop)
	{	
		sl = def_f(t0,y0);

		yn = y0 + sl*dt;
		tn = t0 + dt;

		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(t0),y_world_to_y_screen(y0));
			glVertex2f(x_world_to_x_screen(tn),y_world_to_y_screen(yn));
		glEnd();

		y0 = yn;
		t0 = tn;
		tstart = tstart + dt;
	}
	glFlush();
}

void euler_cascade(double t_start, double t_stop)
{
	double t0,jump,y0;
	int i;
	int CascadeIterations = 150;

	jump = (Y_MAX -Y_MIN)/CascadeIterations;

	for(i=0; i<CascadeIterations; i++)
	{
		y0 = Y_MIN + i*jump;
		t0 = t_start;
		euler(t0, y0, t_stop);
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
		else
		{
			g_x = x_machine_to_x_world(x);
			glColor3f(1.0,0.0,1.0);
			euler_cascade(g_x, g_timeStop);
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
