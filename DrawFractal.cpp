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

void init();
void display();
void drawSphere(double r, int u, int v);
void breakTriangle(double r, double x, double y, double z, int iter, int now, int type);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);

	glutCreateWindow("OpenGL 3D View");

	//init();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5, 5, -5, 5, 5, 15);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

void display()
{
	glBegin(GL_POINTS);
	glPointSize(10.0f);  // wat
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glFlush();
	breakTriangle(1, 0, 0, 1, 3, 0, 1);
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

void drawSphere(double r, int u, int v) {

}