// SphereTest.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#ifdef __APPLE__ // 若作業系統為macOS系統，則使用下列header檔案
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <math.h>
#endif
#define M_PI 3.141592653589793

void init();
void display();
void breakTriangle(double r, double x, double y, double z, int iter, int now, int type);
void _drawSphere(double r, int lats, int longs, double xx, double yy, double zz, double rr, double gg, double bb);
void reshape(int _width, int _height);
void keyboard(unsigned char key, int xx, int yy);
void drawCylinder();

int width = 400, height = 400;
double cylinder_height = 3;
bool toStop = false;
int slice = 10, stack = 10;


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutReshapeFunc(reshape);
	
	glutCreateWindow("OpenGL 3D View");

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(&keyboard);
	glutMainLoop();
	return 0;
}

void reshape(int _width, int _height) {
	width = _width;
	height = _height;
}

void keyboard(unsigned char key, int xx, int yy) {
	// TO DO: Set the keyboard control
	if (key == 'p') {
		toStop = !toStop;
	}
	else if (key == 'o') {
		int a, b;
		printf("Please enter the slice number: ");
		scanf_s("%d", &a);
		printf("Please enter the stack number: ");
		scanf_s("%d", &b);
		slice = a;
		stack = b;
	}

}


void init()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 10, 10, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	//Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, width / (GLfloat)height, 0.1, 1000);
	//Viewport Matrix
	glViewport(0, 0, width, height);


	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_CULL_FACE);		//remove back face
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);		//normalized normal 
	glEnable(GL_COLOR_MATERIAL);
	
}

double d = 0;
void display()
{
	if (!toStop) {
		d += 0.1;
		d = d > 360 ? 0 : d;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	_drawSphere(3, slice, stack, 0, 0, 0, 1, 0, 0);
	//glRotatef(d / 30.0, 0, 0, -1);
	glPushMatrix();

	glRotatef(d / 30.0*360.0, 0, 0, -1);
	glTranslatef(5, 0, 0);

	glRotatef(d / 30.0*360.0, 0, 0, -1);
	_drawSphere(1, slice, stack, 0, 0, 0, 0, 0, 1);
	glTranslatef(0, 0, -1.5);
	drawCylinder();

	glTranslatef(2, 0, 1.5);
	_drawSphere(0.5, slice, stack, 0, 0, 0, 0, 1, 0);

	glPopMatrix();
	if(!toStop)
		glRotatef(0.1, 0, 0, -1);
	//glPopMatrix();
	
	glutPostRedisplay();
	glutSwapBuffers();
}

void breakTriangle(double r, double x, double y, double z, int iter, int now, int type) {
	if (now == iter) return;
	now++;

	double newR = r / 2;
	double centerX[4] = {0};
	double centerY[4] = {0};

	//init
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);

	//up triangle
	
	glVertex3f(x - newR / 2, y + type * newR / (2 * sqrt(3)), z);
	glVertex3f(x + newR / 2, y + type * newR / (2 * sqrt(3)), z);
	glVertex3f(x, y + type * newR / (2 * sqrt(3)) + newR / 2 * sqrt(3), z);
	glVertex3f(x - newR / 2, y + type * newR / (2 * sqrt(3)), z);
	
	centerX[0] = x;
	centerY[0] = y + type * newR / sqrt(3);

	glEnd();
	glFlush();

	//left 
	glBegin(GL_LINE_STRIP);
	glVertex3f(x - newR, y - type * newR / sqrt(3), z);
	glVertex3f(x, y - type * newR / sqrt(3), z);
	glVertex3f(x - newR / 2, y + type * newR / (sqrt(3) * 2), z);
	glVertex3f(x - newR, y - type * newR / sqrt(3), z);

	centerX[1] = x - newR / 2;
	centerY[1] = y - type * newR / (sqrt(3)*2);

	glEnd();
	glFlush();

	//middle
	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y - type * newR / sqrt(3) , z);
	glVertex3f(x + newR / 2, y + type * newR / (sqrt(3) * 2), z);
	glVertex3f(x - newR / 2, y + type * newR / (sqrt(3) * 2), z);
	glVertex3f(x, y - type * newR / sqrt(3) , z);

	centerX[2] = x;
	centerY[2] = y;

	glEnd();
	glFlush();

	//right
	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y - type * newR / sqrt(3), z);
	glVertex3f(x + newR, y - type * newR / sqrt(3), z);
	glVertex3f(x + newR / 2, y + type * newR / (sqrt(3) * 2), z);
	glVertex3f(x, y - type * newR / sqrt(3), z);

	centerX[3] = x + newR / 2;
	centerY[3] = y - type * newR / (sqrt(3) * 2);

	glEnd();
	glFlush();

	//iter
	for (int i = 0; i < 4; i++) {
		if(i != 2)
			breakTriangle(newR, centerX[i], centerY[i], z, iter, now, type * 1);
		else 
			breakTriangle(newR, centerX[2], centerY[2], z, iter, now, type * (-1));
	}
	

}


void _drawSphere(double r, int lats, int longs, double xx, double yy, double zz, double rr, double gg, double bb) {
	for (double i = 0; i <= M_PI; i += M_PI / lats) {
		for (double j = 0; j <= 2 * M_PI; j += M_PI / longs) {
			double x = r * sin(i) * cos(j);
			double y = r * sin(i) * sin(j);
			double z = r * cos(i);
			double _x = r * sin(i + M_PI / lats) * cos(j);
			double _y = r * sin(i + M_PI / lats) * sin(j);
			double _z = r * cos(i + M_PI / lats);
			glBegin(GL_LINE_STRIP);
			glColor3f(rr, gg, bb);
			glVertex3f(x + xx, y + yy, z + zz);
			glVertex3f(_x + xx, _y + yy, _z + zz);
		}
		glEnd();
		glFlush();
	}
	
}

void drawCylinder() {
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 0.0f);
	GLUquadricObj *obj = gluNewQuadric();
	//glTranslatef(10, 0, 0);
	gluCylinder(obj, 0.1f, 0.1f, cylinder_height, 50, 50);
	glEnd();
}