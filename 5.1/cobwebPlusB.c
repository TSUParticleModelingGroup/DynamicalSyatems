//gcc cobwebPlusB.c -o cobwebPlusB -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//#include <device.h>

#define X_WINDOW 1500
#define Y_WINDOW 1000

#define X_MAX 5.0
#define X_MIN -5.0
#define X_SCALE 0.25

#define Y_MAX 5.0
#define Y_MIN -5.0
#define Y_SCALE 0.25

// function prototypes
void KeyPressed(unsigned char key, int x, int y);
void Display(void);

//globals
double g_x;
double g_y;
static int g_win;
int g_funCompNum = 1;

double rootfunction(double x)
{
	return(-3.0*fabs(x - 1.0/2.0) + 3.0/2.0);
}

double f(double x, int n)
{
	double xnew;
	int i;
	for(i = 0; i < n; i++)
	{
		xnew = rootfunction(x);
		x = xnew;		
	}
	return(x);
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

void draw_bound_lines()
{
	glColor3f(0.0,0.0,1.0);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(X_MIN),y_world_to_y_screen(0.0));
		glVertex2f(x_world_to_x_screen(X_MAX),y_world_to_y_screen(0.0));
	glEnd();
	
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(X_MIN),y_world_to_y_screen(1.0));
		glVertex2f(x_world_to_x_screen(X_MAX),y_world_to_y_screen(1.0));
	glEnd();

	glFlush();
}

void drawFunction(int n)
{
	double x, dx;
	
	dx = (X_MAX - X_MIN)/1000.0;
	x = X_MIN;
	while(x < X_MAX)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(f(x,n)));
			glVertex2f(x_world_to_x_screen(x+dx),y_world_to_y_screen(f(x+dx,n)));
		glEnd();
		x += dx;
	}
	glFlush();
}

void draw45DegreeLine()
{
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(X_MIN),y_world_to_y_screen(1.0*X_MIN + 0.0));
		glVertex2f(x_world_to_x_screen(X_MAX),y_world_to_y_screen(1.0*X_MAX + 0.0));
	glEnd();

	glFlush();
}

void drawLineVertical(int n)
{
	double yOld;
	yOld = g_y;
	g_y = f(g_x,n);
	
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(g_x),y_world_to_y_screen(yOld));
		glVertex2f(x_world_to_x_screen(g_x),y_world_to_y_screen(g_y));
	glEnd();

	glFlush();
}

void drawLineHorizontal()
{
	double xOld;
	xOld = g_x;
	g_x = g_y;
	
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x_world_to_x_screen(xOld),y_world_to_y_screen(g_y));
		glVertex2f(x_world_to_x_screen(g_x),y_world_to_y_screen(g_y));
	glEnd();
	glFlush();
}

void placePoint(double x)
{
	glPointSize(10.0);
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_POINTS);
		glVertex2f(x_world_to_x_screen(x),y_world_to_y_screen(0.0));
	glEnd();
	glFlush();
}

void printPoint()
{
	printf("\nx = %f\n",g_x);
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
				place_hash_marks();
				draw_bound_lines();
				glColor3f(0.0,1.0,0.0);
				drawFunction(g_funCompNum);
				draw45DegreeLine();
			}
			else
			{
				glClear(GL_COLOR_BUFFER_BIT);
				place_axis();
				place_hash_marks();
				draw_bound_lines();
				glColor3f(0.0,1.0,0.0);
				drawFunction(g_funCompNum);
				draw45DegreeLine();
				
				glColor3f(1.0,1.0,0.0);
				g_x = x_machine_to_x_world(x);
				g_y = 0.0;
				placePoint(g_x);
				printPoint();
			}
		}
		else
		{
			drawLineVertical(g_funCompNum);
			drawLineHorizontal();
			printPoint();
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
	if(key == 'p')
	{
		printf("\nInter start x value\n");
		scanf("%f", &tempx);
		g_x = tempx;
		
		glColor3f(1.0,0.0,0.0);
		placePoint(g_x);
		printPoint();
	}
	if(key == 'f')
	{
		printf("\nInter function composition number\n");
		scanf("%d", &g_funCompNum);
		
		glClear(GL_COLOR_BUFFER_BIT);
		place_axis();
		place_hash_marks();
		draw_bound_lines();
		glColor3f(0.0,1.0,0.0);
		drawFunction(g_funCompNum);
		draw45DegreeLine();
	}
	if(key == 's')
	{
		printf("\nInter function composition number to show along side the function\n");
		scanf("%d", &n);
		if(n == 1)      glColor3f(1.0,1.0,1.0);
		else if(n == 2) glColor3f(0.0,1.0,1.0);
		else if(n == 3) glColor3f(1.0,0.0,0.0);
		else if(n == 4) glColor3f(1.0,1.0,0.0);
		else if(n == 5) glColor3f(1.0,0.0,1.0);
		else if(n == 6) glColor3f(0.0,0.0,1.0);
		drawFunction(n);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	place_axis();
	place_hash_marks();
	draw_bound_lines();
	glColor3f(0.0,1.0,0.0);
	drawFunction(g_funCompNum);
	draw45DegreeLine();
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
