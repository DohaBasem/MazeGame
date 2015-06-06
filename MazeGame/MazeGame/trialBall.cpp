//Include necessary header files
#include "glut.h"
#include <cstdlib>
#include <cmath>

//Link with necessary libraries
#pragma comment (lib, "glut32.lib")

//Callback function prototypes
void render(void);
void onKeyboard(unsigned char key, int x, int y);
void onSpecialKey(int key, int x, int y);
void onFrame(void);
//Since there are no rings to make visualization better I tried to get top view of the project to see easier
//Then I made the rings but still kept them
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);

#define ESC_ASCII 27

//state of window, handle to terminate window
int g_Win;


const float PI = 3.141592653;
float x, y, z;
float g_Theta = 0.0f;
int g_LastTime, xDragStart, yDragStart;
float g_X = 0.0f, g_Y = 0.0f, g_Z = -3.5f;
float x_T = 0.0f, y_T = 0.0f, z_T = 0.0f;
double camera_angle_h = 0;
double camera_angle_v = 0;
float spd[4] = { 400, 50, 200, 500 };
float dis[5] = { 0.59, 0.79, 0.99, 1.2, 0.1 };
float rad[6] = { 0.2, 0.1, 0.09, 0.04, 0.07, 0.02 };
bool isDragging = false;


int main(int argc, char **argv)
{
	//initializing glut
	glutInit(&argc, argv);

	//double buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);				//double buffer with depth

	//creating window
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("trial Ball");

	//display buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//projection transformation -> mapping 3D to 2D  -> setting of angle of lenses of camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.0f, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Enabling
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_LIGHTING);	//Enable lighting
	glEnable(GL_SPECULAR);*/
	glEnable(GL_NORMALIZE);	//Enable auto-normalization of normal vectors
	glShadeModel(GL_SMOOTH);

	//registering callbacks function
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialKey);
	glutDisplayFunc(render);
	//glutIdleFunc(onFrame);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	//Go full-screen
	//glutFullScreen();

	//main loop
	glutMainLoop();

	return 0;
}

void render(void)
{
	//Clear display buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//clearing previous transformation to have only one image showing
	glLoadIdentity();

	//view transformation -> where the camera/eye looks
	//must be first after load identity function
	gluLookAt(g_X, g_Y, g_Z, g_X, g_Y, 0, 0, 1, 0);				
	glRotated(camera_angle_v, 1.0, 0.0, 0.0);
	glRotated(camera_angle_h, 0.0, 1.0, 0.0);



	//*********************************************************
	//Sun
	//*********************************************************
	glColor3f(1.0f, 1.0f, 0.0f);				//yellow sun
	glTranslatef(x_T, y_T, z_T);					
	//glutWireSphere(0.3, 30, 30);		
	glutSolidSphere(rad[0], 30, 30);

	//Output color buffer to screen
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{
	//x_T = 0.0; y_T = 0.0; z_T = 0.0;

	switch (key)
	{
	case 'w':
		y_T += 0.05f;				//moving the camera so the model will move in the opposite direction
		break;

	case 's':
		y_T -= 0.05f;
		break;

	case 'a':
		x_T += 0.05f;
		break;

	case 'd':
		x_T -= 0.05f;
		break;

	case ESC_ASCII:
		glutDestroyWindow(g_Win);
		exit(0);
	}
	glutPostRedisplay();
}

void onSpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		z_T += 0.05f;			//increasing the distance between camera and object
		break;

	case GLUT_KEY_DOWN:
		z_T -= 0.05f;
		break;
	}
	glutPostRedisplay();
}

//changing only the angle which will be rotated by the rotatef function in the next round of the render fundtion
void onFrame()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	g_Theta += 0.0005f * (time - g_LastTime);
	//g_Theta += 0.0005f;
	g_LastTime = time;
	glutPostRedisplay();
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
}