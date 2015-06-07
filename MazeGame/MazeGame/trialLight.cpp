#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "glut.h"

#define ESC_ASCII 27

#pragma comment (lib, "glut32.lib")


float g_X = 5.0f, g_Z = -40.5f, g_Y = 0.0f;
const float PI = 3.141592653;
float x, y, z;
int g_LastTime, xDragStart, yDragStart;
double camera_angle_h = 0;
double camera_angle_v = 0;
bool isDragging = false;

//Light Colors
float g_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float g_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float g_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void drawBase(){

	//Material Colors
	float g_MaterialAmbientDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };		//red material
	int g_Shininess = 80;
	float lightYellow[] = { 1, 1, 0.5, 0 };

	//for the object
	//should be specified for each object
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, g_MaterialAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, g_Specular);
	//glMaterialfv(GL_FRONT, GL_EMISSION, red);
	glMateriali(GL_FRONT, GL_SHININESS, g_Shininess);


	// glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	 //glColor3f(1.0f, 1.0f, 1.0f);     // white

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, -10.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, -10.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f, 1.0f);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightYellow);
	glutSolidSphere(4, 30, 30);

}

void mouseMove(int x, int y)
{
	if (isDragging) { // only when dragging
		// update the change in angle
		camera_angle_v += (y - yDragStart)*0.3;
		camera_angle_h += (x - xDragStart)*0.3;
		xDragStart = x;
		yDragStart = y;
	}

	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {			// left mouse button pressed
			isDragging = true;
			xDragStart = x;					// save x where button first pressed
			yDragStart = y;
		}
		else  {								//left button released
			isDragging = false;
		}
	}

	glutPostRedisplay();
}

void render(){

	//Clear display buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw something
	glLoadIdentity();
	gluLookAt(g_X, g_Y, g_Z, g_X, g_Y, 0, 1, 0, 0);
	glRotated(camera_angle_v, 0.0, 1.0, 0.0);
	glRotated(camera_angle_h, 0.0, 0.0, 1.0);

	drawBase();
	
	glutSwapBuffers();
}

void light(){

	glEnable(GL_LIGHTING);	//Enable lighting
	glEnable(GL_SPECULAR);
	glShadeModel(GL_SMOOTH);

	//for the light source
	float lightPosition[4] = { -g_X, g_Y, -30, 1 };
	float light0Ambient[] = { 0.0f, 0.0f, 0.0f };	//No light ambient component
	//We use global ambient instead
	float red[] = { 1, 0, 0, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_Specular); 
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.000005f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005f);
	//glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, red);
	//glLightfv(GL_LIGHT0, GL_LIGHT_MODEL_AMBIENT, lightYellow);
	glEnable(GL_LIGHT0);

	//Setup global ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);


}

int main(int argc, char* argv[]){

	//  Initialize GLUT and process user parameters
	glutInit(&argc, argv);


	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Maze Game");

	//display buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Set projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Enables
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	//draw if the incoming depth value is less than or equal to the stored depth value
	glEnable(GL_NORMALIZE);	//Enable auto-normalization of normal vectors

	
	// Callback functions
	glutDisplayFunc(render);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	//Light settings
	light();

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;

}