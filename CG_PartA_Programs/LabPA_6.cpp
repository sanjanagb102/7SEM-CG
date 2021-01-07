#include<stdio.h>
#include<stdlib.h>
#include<gl/glut.h>
#include "Header.h"

#define outcode int
#define true 1
#define false 0
double xmin_8, ymin_8, xmax_8, ymax_8;
double xvmin_8, yvmin_8, xvmax_8, yvmax_8;


const int RIGHT = 4;
const int LEFT = 8;
const int TOP = 1;
const int BOTTOM = 2;

int n_8;
struct line_segment {
	int x1;
	int y1;
	int x2;
	int y2;
};
struct line_segment ls[10];

outcode computeoutcode(double x, double y)
{
	outcode code = 0;
	if (y > ymax_8)
		code |= TOP;
	else if (y < ymin_8)
		code |= BOTTOM;
	if (x > xmax_8)
		code |= RIGHT;
	else if (x < xmin_8)
		code |= LEFT;

	return code;
}

void cohensuther(double x0, double y0, double x1, double y1)
{
	outcode outcode0, outcode1, outcodeout;
	bool accept = false, done = false;

	outcode0 = computeoutcode(x0, y0);
	outcode1 = computeoutcode(x1, y1);

	do
	{
		if (!(outcode0 | outcode1))
		{
			accept = true;
			done = true;
		}
		else if (outcode0 & outcode1)
			done = true;
		else
		{
			double x, y;
			outcodeout = outcode0 ? outcode0 : outcode1;
			if (outcodeout & TOP)
			{
				x = x0 + (x1 - x0) * (ymax_8 - y0) / (y1 - y0);
				y = ymax_8;
			}
			else if (outcodeout & BOTTOM)
			{
				x = x0 + (x1 - x0) * (ymin_8 - y0) / (y1 - y0);
				y = ymin_8;
			}
			else if (outcodeout & RIGHT)
			{
				y = y0 + (y1 - y0) * (xmax_8 - x0) / (x1 - x0);
				x = xmax_8;
			}
			else
			{
				y = y0 + (y1 - y0) * (xmin_8 - x0) / (x1 - x0);
				x = xmin_8;
			}

			if (outcodeout == outcode0)
			{
				x0 = x;
				y0 = y;
				outcode0 = computeoutcode(x0, y0);
			}
			else
			{
				x1 = x;
				y1 = y;
				outcode1 = computeoutcode(x1, y1);
			}
		}

	} while (!done);

	if (accept)
	{
		double sx = (xvmax_8 - xvmin_8) / (xmax_8 - xmin_8);
		double sy = (yvmax_8 - yvmin_8) / (ymax_8 - ymin_8);
		double vx0 = xvmin_8 + (x0 - xmin_8) * sx;
		double vy0 = yvmin_8 + (y0 - ymin_8) * sy;
		double vx1 = xvmin_8 + (x1 - xmin_8) * sx;
		double vy1 = yvmin_8 + (y1 - ymin_8) * sy;

		glColor3f(1, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xvmin_8, yvmin_8);
		glVertex2f(xvmax_8, yvmin_8);
		glVertex2f(xvmax_8, yvmax_8);
		glVertex2f(xvmin_8, yvmax_8);
		glEnd();

		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex2d(vx0, vy0);
		glVertex2d(vx1, vy1);
		glEnd();
	}
}

void display_LabPA8()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0, 0, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xmin_8, ymin_8);
	glVertex2f(xmax_8, ymin_8);
	glVertex2f(xmax_8, ymax_8);
	glVertex2f(xmin_8, ymax_8);
	glEnd();
	for (int i = 0; i < n_8; i++)
	{
		glBegin(GL_LINES);
		glVertex2d(ls[i].x1, ls[i].y1);
		glVertex2d(ls[i].x2, ls[i].y2);
		glEnd();
	}

	for (int i = 0; i < n_8; i++)
		cohensuther(ls[i].x1, ls[i].y1, ls[i].x2, ls[i].y2);

	glFlush();
}
void myinit_LabPA8()
{
	glClearColor(1, 1, 1, 1);
	glColor3f(1, 0, 0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
}

void LabPA_8_main(int argc, char** argv)
{
	printf("Enter window coordinates (xmin ymin xmax ymax): \n");
	scanf_s("%lf%lf%lf%lf", &xmin_8, &ymin_8, &xmax_8, &ymax_8);
	printf("Enter viewport coordinates (xvmin yvmin xvmax yvmax) :\n");
	scanf_s("%lf%lf%lf%lf", &xvmin_8, &yvmin_8, &xvmax_8, &yvmax_8);
	printf("Enter no. of lines:\n");
	scanf_s("%d", &n_8);
	for (int i = 0; i < n_8; i++)
	{
		printf("Enter line endpoints (x1 y1 x2 y2):\n");
		scanf_s("%d%d%d%d", &ls[i].x1, &ls[i].y1, &ls[i].x2, &ls[i].y2);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("clip");
	myinit_LabPA8();
	glutDisplayFunc(display_LabPA8);
	glutMainLoop();
}