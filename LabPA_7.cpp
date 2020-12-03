#include <stdio.h>
#include <GL/glut.h>
#include "Header.h"
double xmin_9 = 50, ymin_9 = 50, xmax_9 = 100, ymax_9 = 100; // Window boundaries
double xvmin_9 = 200, yvmin_9 = 200, xvmax_9 = 300, yvmax_9 = 300; // Viewport boundaries
int cliptest(double p, double q, double* t1, double* t2)
{
	double t = q / p;
	if (p < 0.0) // potentially enry point, update te
	{
		if (t > * t1) *t1 = t;
		if (t > * t2) return(false); // line portion is outside
	}
	else
		if (p > 0.0) // Potentially leaving point, update tl
		{
			if (t < *t2) *t2 = t;
			if (t < *t1) return(false); // line portion is outside
		}
		else
			if (p == 0.0)
			{
				if (q < 0.0) return(false); // line parallel to edge but outside
			}
	return(true);
}
void LiangBarskyLineClipAndDraw(double x0, double y0, double x1, double y1)
{
	double dx = x1 - x0, dy = y1 - y0, te = 0.0, tl = 1.0;
	if (cliptest(-dx, x0 - xmin_9, &te, &tl)) // inside test wrt left edge
		if (cliptest(dx, xmax_9 - x0, &te, &tl)) // inside test wrt right edge
			if (cliptest(-dy, y0 - ymin_9, &te, &tl)) // inside test wrt bottom edge
				if (cliptest(dy, ymax_9 - y0, &te, &tl)) // inside test wrt top edge
				{
					if (tl < 1.0)
					{
						x1 = x0 + tl * dx;
						y1 = y0 + tl * dy;
					}
					if (te > 0.0)
					{
						x0 = x0 + te * dx;
						y0 = y0 + te * dy;
					}
					// Window to viewport mappings
					double sx = (xvmax_9 - xvmin_9) / (xmax_9 - xmin_9); // Scale parameters
					double sy = (yvmax_9 - yvmin_9) / (ymax_9 - ymin_9);
					double vx0 = xvmin_9 + (x0 - xmin_9) * sx;
					double vy0 = yvmin_9 + (y0 - ymin_9) * sy;
					double vx1 = xvmin_9 + (x1 - xmin_9) * sx;
					double vy1 = yvmin_9 + (y1 - ymin_9) * sy;
					//draw a red colored viewport
					glColor3f(1.0, 0.0, 0.0);
					glBegin(GL_LINE_LOOP);
					glVertex2f(xvmin_9, yvmin_9);
					glVertex2f(xvmax_9, yvmin_9);
					glVertex2f(xvmax_9, yvmax_9);
					glVertex2f(xvmin_9, yvmax_9);
					glEnd();
					glColor3f(0.0, 0.0, 1.0); // draw blue colored clipped line
					glBegin(GL_LINES);
					glVertex2d(vx0, vy0);
					glVertex2d(vx1, vy1);
					glEnd();
				}
}// end of line clipping
void display_LabPA9()
{
	double x0 = 60, y0 = 20, x1 = 80, y1 = 120;
	glClear(GL_COLOR_BUFFER_BIT);
	//draw the line with red color
	glColor3f(1.0, 0.0, 0.0);
	//bres(120,20,340,250);
	glBegin(GL_LINES);
	glVertex2d(x0, y0);
	glVertex2d(x1, y1);
	glEnd();
	//draw a blue colored window
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xmin_9, ymin_9);
	glVertex2f(xmax_9, ymin_9);
	glVertex2f(xmax_9, ymax_9);
	glVertex2f(xmin_9, ymax_9);
	glEnd();
	LiangBarskyLineClipAndDraw(x0, y0, x1, y1);
	glFlush();
}
void myinit_LabPA9()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 499.0, 0.0, 499.0);
}
void LabPA_9_main(int argc, char** argv)
{
	//int x1, x2, y1, y2;
	//printf("Enter End points:");
	//scanf_s("%d%d%d%d", &x1,&x2,&y1,&y2);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Liang Barsky Line Clipping Algorithm");
	glutDisplayFunc(display_LabPA9);
	myinit_LabPA9();
	glutMainLoop();
}