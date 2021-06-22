//gcc 3DPlotLorenzSensitivity.c -o Lorenz -lglut -lm -lGLU -lGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define SCALE 50.0
#define MAXT SCALE
#define MINT -SCALE
#define MAXY SCALE
#define MINY -SCALE
#define MAXZ SCALE
#define MINZ -SCALE

#define DRAW 10

#define X0 1.0
#define X1 1.0
#define X2 10.0

#define XX0 1.0
#define XX1 1.0
#define XX2 10.001

#define TSTART 0.0

#define DT 0.000001
#define TSTOP 50.0

#define ALF 10.0
#define B (8.0/3.0)
#define R 28.0

#define SHIFT 0.3

double x[3];
double dx[3];
double xx[3];
double dxx[3];

void model()
{
	dx[0] = ALF*(x[1]-x[0]);
	dx[1] = R*x[0]-x[1]-x[0]*x[2];
	dx[2] = x[0]*x[1]-B*x[2];
	
	dxx[0] = ALF*(xx[1]-xx[0]);
	dxx[1] = R*xx[0]-xx[1]-xx[0]*xx[2];
	dxx[2] = xx[0]*xx[1]-B*xx[2];
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_FLAT);
}

void euler()
{
	double t;
	int tdraw;

	x[0] = X0;
	x[1] = X1;
	x[2] = X2;
	
	xx[0] = XX0;
	xx[1] = XX1;
	xx[2] = XX2;
	
	t = TSTART;
	tdraw = 0;
	glPointSize(1.0);

	while(t <= TSTOP)
	{
		model();

		x[0] = dx[0]*DT + x[0];
		x[1] = dx[1]*DT + x[1];
		x[2] = dx[2]*DT + x[2];
		
		xx[0] = dxx[0]*DT + xx[0];
		xx[1] = dxx[1]*DT + xx[1];
		xx[2] = dxx[2]*DT + xx[2];
		
		t = t + DT;

		tdraw++;

		if(tdraw == DRAW)
		{
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_POINTS);
				glVertex3f(x[0]/SCALE,x[1]/SCALE,x[2]/SCALE);
			glEnd();
			
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_POINTS);
				glVertex3f(xx[0]/SCALE + SHIFT ,xx[1]/SCALE ,xx[2]/SCALE);
			glEnd();

			glFlush();
			tdraw = 0;
		}
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	glLoadIdentity();

	gluLookAt(1.0,1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	euler();
	
	while(1);

}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glFrustum(-0.2, 0.2, -0.2, 0.2, 0.2, 5.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(950,950);
	glutInitWindowPosition(0,0);
	glutCreateWindow("3 D Graph");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
