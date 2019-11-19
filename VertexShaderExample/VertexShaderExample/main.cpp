#include "FreeImage.h"
#include "glew.h"
#include "glut.h"
#include "shader.h"
#include <iostream>
#include <string>


#define PI 3.14159265358

using namespace std;

int windowSize[2] = { 600, 600 };
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);

GLuint program;
GLuint vboName;
GLuint texture = 0;

//Initialize the shaders 
void shaderInit();
void textureInit();

//Number of vertices that will be sent to shader
int verticeNumber = 3;

//Storing vertex datas that will be sent to shader
class VertexAttribute {
public:
	GLfloat position[3];
	GLfloat texcoord[2];
	void setPosition(float x, float y, float z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
	};
	void setPosition(float x, float y) {
		texcoord[0] = x;
		texcoord[1] = y;
	};
};

//Collecting vertex datas
VertexAttribute *drawEarth(int slice, int stack);

int main(int argc, char** argv) {	
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("ComputerGraphicsDemo");

	glewInit();
	shaderInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void textureInit() {
	glEnable(GL_TEXTURE_2D);
	FIBITMAP* piImage = FreeImage_Load(
		FreeImage_GetFileType("earth_texture_map.jpg", 0),
		"earth_texture_map.jpg");
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(piImage);
	int iWidth = FreeImage_GetWidth(piImage);
	int iHeight = FreeImage_GetHeight(piImage);
	//printf("%d %d\n", iWidth, iHeight);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(piImage);
}

void shaderInit() {
	GLuint vert = createShader("Shaders/example.vert", "vertex");
	GLuint frag = createShader("Shaders/example.frag", "fragment");
	program = createProgram(vert, frag);

	//Generate a new buffer object
	glGenBuffers(1, &vboName);
	glBindBuffer(GL_ARRAY_BUFFER, vboName);


	//Copy vertex data to the buffer object
	VertexAttribute *vertices;
	vertices = drawEarth(30, 30);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * verticeNumber, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)(offsetof(VertexAttribute, position)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint _vbo = 0;
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * verticeNumber, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)(offsetof(VertexAttribute, texcoord)));
	glBindBuffer(GL_ARRAY_BUFFER, 1);
}

float _rotate = 0;
void display() {   
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 3.0f,// eye
		0.0f, 0.0f, 0.0f,// center
		0.0f, 1.0f, 0.0f);// up

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)512 / (GLfloat)512, 1, 500);

	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	glMatrixMode(GL_MODELVIEW);
	glRotatef(_rotate++, 0, 1, 0);

	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat pmtx[16];
	GLfloat mmtx[16];
	glGetFloatv(GL_PROJECTION_MATRIX, pmtx);
	glGetFloatv(GL_MODELVIEW_MATRIX, mmtx);
 	GLint pmatLoc = glGetUniformLocation(program, "Projection");
	GLint mmatLoc = glGetUniformLocation(program, "ModelView");
	GLint texLoc = glGetUniformLocation(program, "Texture");
	glUseProgram(program);

	//input the modelview matrix into vertex shader
	glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, pmtx);
	//input the rotation matrix into vertex shader
	glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, mmtx);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texLoc, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, verticeNumber);
	glUseProgram(0);
	glPopMatrix();
	glutSwapBuffers();
	glBindTexture(GL_TEXTURE_2D, NULL);
}



VertexAttribute *drawEarth(int slice, int stack) {
	float sliceSpacing = 1.0f / (slice + 1.0f);
	float stackSpacing = 1.0f / (stack);
	//printf("%d %d\n", slice, stack);
	VertexAttribute *vertices;
	int num = slice * (stack+1) * 2;
	verticeNumber = num;
	vertices = new VertexAttribute[num];
	

	double x, y, z;
	double slice_step = 2 * PI / slice, stack_step = PI / stack;
	int kk = 0;
	for (int i = 0; i < slice; i++) {
		for (int j = 0; j < stack + 1; j++) {
			x = sin(j * stack_step) * cos(i*slice_step);
			y = cos(j * stack_step);
			z = sin(j * stack_step) * sin(i*slice_step);
			//printf("%f %f\n", slice_step * i, stack_step * j);
			vertices[kk].setPosition(x, y, z);
			vertices[kk++].setPosition(1-sliceSpacing * i < 0 ? 0 : 1-sliceSpacing * i, (1-stackSpacing * j) < 0 ? 0 : (1 - stackSpacing * j));
			//vertices[kk++].setPosition(0, 0);

			x = sin(j * stack_step) * cos((i + 1)*slice_step);
			y = cos(j * stack_step);
			z = sin(j * stack_step) * sin((i + 1)*slice_step);
			vertices[kk].setPosition(x, y, z);
			vertices[kk++].setPosition(1-sliceSpacing * (i + 1) < 0 ? 0 : 1-sliceSpacing * (i + 1), (1 - stackSpacing * j) < 0 ? 0 : (1 - stackSpacing * j));
			//printf("%f %f\n", sliceSpacing * (i+1), stackSpacing * j);
			//vertices[kk++].setPosition(0, 0);
		}
	}
	return vertices;
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void keyboard(unsigned char key, int x, int y) {

}

void idle() {
	glutPostRedisplay();
}