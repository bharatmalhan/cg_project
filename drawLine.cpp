/*
 * Compie with the following steps
 * g++ -c Rectangle.cpp
 * g++ -c drawLine.cpp
 * g++ -o mainprog Rectangle.o drawLine.o -lglut -lGL -lGLU
 *
 */


#include <GL/glut.h>
#include <stdio.h>
#include "Rectangle.h"
#include <vector>

using std::vector;

int width = 600;
int height = 400;
int first = 0;
vector<Rectangle*> listOfRectangles;
int x1, y1, x2, y2;
int numOfRectangles = 0;
float oldColor[3] = { 0.0, 0.0, 0.0 };
float newColor[3] = { 1.0, 0.0, 0.0 };
enum tool { circle, rectangle, line, filltool }tool;

void drawLine(int x1, int y1, int x2, int y2) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);	
	glVertex2i(x2, y2);
	glEnd();
	glFlush();
}

void drawRectangleList() {
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i=0;i<listOfRectangles.size();i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2i(listOfRectangles[i]->x1, listOfRectangles[i]->y1);
		glVertex2i(listOfRectangles[i]->x2, listOfRectangles[i]->y2);
		glVertex2i(listOfRectangles[i]->x3, listOfRectangles[i]->y3);
		glVertex2i(listOfRectangles[i]->x4, listOfRectangles[i]->y4);
		glEnd();
	}
	glFlush();
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void setPixel (int x, int y, float color[3]) {
	glBegin(GL_POINTS);
		glColor3fv(color);
		glVertex2i(x, y);
	glEnd();
	glFlush();
}

void getPixel (int x, int y, float color[3]) {
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);
}

void fill (int x, int y, float oldColor[3], float newColor[3]) {
	float color[3];
	getPixel(x, y, color);
	if (color[0] == oldColor[0] && color[1] == oldColor[1] && color[2] == oldColor[2]) {
		setPixel(x, y, newColor);
		fill(x+1, y, oldColor, newColor);
		fill(x-1, y, oldColor, newColor);
		fill(x, y+1, oldColor, newColor);
		fill(x, y-1, oldColor, newColor);
	}
}

void mouse(int button, int state, int x, int y) {
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		switch(first) {
			case 0:
				if (tool == rectangle) {
					Rectangle* rect = new Rectangle();
					printf("%d\n", rect->x1);
					listOfRectangles.push_back(rect);
					listOfRectangles[numOfRectangles]->x1 = x;
					listOfRectangles[numOfRectangles]->y1 = height - y;
					listOfRectangles[numOfRectangles]->x2 = x;
					listOfRectangles[numOfRectangles]->y4 = height -y;
					first = 1;
				} else if (tool == circle) {

				} else if (tool == line) {
					x1 = x;
					y1 = height - y;
					first = 1;
				} else if (tool == filltool) {
					x1 = x;
					y1 = height - y;
					fill(x1, y1, oldColor, newColor);
					first = 0;
				}
				break;
			case 1:
				if (tool == rectangle) {
					listOfRectangles[numOfRectangles]->x3 = x;
					printf("Here I am baby\n");
					listOfRectangles[numOfRectangles]->y2 = height - y;
					listOfRectangles[numOfRectangles]->y3 = height - y;
					listOfRectangles[numOfRectangles]->x4 = x;
					printf("%d %d %d %d %d %d %d %d\n", listOfRectangles[numOfRectangles]->x1,listOfRectangles[numOfRectangles]->y1,listOfRectangles[numOfRectangles]->x2,listOfRectangles[numOfRectangles]->y2,listOfRectangles[numOfRectangles]->x3,listOfRectangles[numOfRectangles]->y3,listOfRectangles[numOfRectangles]->x4,listOfRectangles[numOfRectangles]->y4);
					numOfRectangles++;
					drawRectangleList();
				} else if (tool == circle) {

				} else if (tool == line) {
					x2 = x;
					y2 = height - y;
					drawLine(x1, y1, x2, y2);
				}
				first = 0;
				break;
		}
	}
}

void handleKeypress(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
			exit(0);
		case 'r':
			printf("pressed r\n");
			tool = rectangle;
			break;
		case 'c':
			tool = circle;
			break;
		case 'l':
			tool = line;
			break;
		case 'f':
			tool = filltool;
			break;
	}
}

void myInit() {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
	glMatrixMode(GL_MODELVIEW); 
}

int main(int argc, char ** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("demo");
	glutDisplayFunc(display);
	myInit();
	glutMouseFunc(mouse);
	glutKeyboardFunc(handleKeypress);
	glutMainLoop();
	return 0; 
}