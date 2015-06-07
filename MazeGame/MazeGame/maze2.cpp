// 
// Controls:    Left Arrow  - Rotate Left
//              Right Arrow - Rotate Right
//              Up Arrow    - Rotate Up
//              Down Arrow  - Rotate Down     

// ----------------------------------------------------------
// Includes
// ----------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "main.h"
#include "glut.h"

#define GL_GLEXT_PROTOTYPES
#define ESC_ASCII 27

#pragma comment (lib, "glut32.lib")

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
class vertex{
public:float x;
public:float y;
public:float z;
public:bool wall;
};


void display(void);
void specialKeys(int key, int x, int y);
void onKeyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);
void drawCube(void);
void drawSphere(void);
void drawBackground(void);
void light();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
UINT g_Texture[MAX_TEXTURES];

//Window state -> terminate window
int g_Win;					

//Camera translations
float g_X = 5.0f, g_Z = -40.5f, g_Y = 10.0f;
//Sphere translations
float x_T = 0.0f, y_T = 8.0f, z_T = 0.5f;
float xAngle=0.0f;
float yAngle=0.0f;
//Mouse dragging
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
float g_Emission[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 

int g_Shininess = 80;

vertex boundary[80];

//Maze matrix
int maze[] = { 1, 1, 1, 1, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 1,
1, 0, 1, 1, 0, 1, 0, 1,
1, 0, 1, 0, 0, 1, 1, 1,
0, 0, 1, 1, 0, 0, 0, 0,
1, 0, 0, 1, 0, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 1,
1, 0, 1, 1, 1, 1, 0, 1,
1, 0, 0, 0, 0, 0, 0, 1,
1, 1, 1, 1, 1, 1, 1, 1
};


// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){
	//Sound
	 PlaySound("bird_chirping2.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
	//  Initialize GLUT and process user parameters
	glutInit(&argc, argv);


	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutInitWindowSize(1000, 1000);
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

	//Perspective Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	CreateTexture(g_Texture[0], "roughwall_3d.bmp");
	CreateTexture(g_Texture[1], "Grass01.bmp");

	// Callback functions
	glutDisplayFunc(display);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(keyUp);
	
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// Light function
	light();

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;

}

void display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Camera Lookat function
	glLoadIdentity();
	gluLookAt(g_X, g_Y, g_Z, g_X, g_Y, 0, 1, 0, 0);
	glRotated(camera_angle_v, 0.0, 1.0, 0.0);
	glRotated(camera_angle_h, 0.0, 0.0, 1.0);

	//Base and background for maze
	glPushMatrix();
	drawBackground();
	glPopMatrix();

	//Maze
	glPushMatrix();
	int yc = 0;
	int xc = 0;
	for (yc = 0; yc < 10; yc++)
	{
		for (xc = 0; xc < 8; xc++)
		{
			if (maze[xc + yc * 8] == 1)
			{
				glPushMatrix();

				glTranslatef(xc*2.0f, yc*2.0f, 0.0f);

				vertex v;
				v.x = (xc*2.0f);
				v.y = (yc*2.0f);
				v.z = 0.0f;
				v.wall = true;
				boundary[xc + yc * 8] = v;

				drawCube();

				glPopMatrix();
			}
			else {
				vertex v;
				v.x = 0;
				v.y = 0;
				v.z = 0.0f;
				v.wall = false;
				boundary[xc + yc * 8] = v;
			}
		}
	}
	glPopMatrix();
	
	//Player of game -> sphere
	glPushMatrix();
	drawSphere();
	glPopMatrix();

	glutSwapBuffers();
}

//Lighting settings
void light(void)
{
	glEnable(GL_LIGHTING);	
	glEnable(GL_SPECULAR);
	glEnable(GL_AMBIENT);
	glEnable(GL_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	//Setup light 0 
	float light0Ambient[] = {0.0f, 0.0f, 0.0f};	
	float lightPosition[4] = { 25, 30, 20, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_Specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0005f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	glEnable(GL_LIGHT0);	//Enable light source 0

	//Setup global ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);
	
}
void keyUp(unsigned char key, int x, int y){
	switch(key){
	case ' ':
		z_T+=3;
	}

}
void onKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		g_X += 0.05f;
		break;

	case 's':
		g_X -= 0.05f;
		break;

	case 'a':
		g_Y -= 0.05f;
		break;

	case 'd':
		g_Y += 0.05f;
		break;

	case '=':
		g_Z += 0.05f;
		break;

	case '-':
		g_Z -= 0.05f;
		break;
	case ' ':
		z_T -= 3.0f;
		break;
	case ESC_ASCII:
		glutDestroyWindow(g_Win);
		exit(0);
	}
	glutPostRedisplay();
}

bool collide(float x, float y, float z){
	int yc, xc;
	float p1x = x;
	float p1y = y;
	float p1z = z;
	for (yc = 0; yc < 10; yc++)
	{
		for (xc = 0; xc < 8; xc++)
		{

			float p2x = boundary[xc + yc * 8].x;
			float p2y = boundary[xc + yc * 8].y;
			float p2z = boundary[xc + yc * 8].z;
			if (boundary[xc + yc * 8].wall){
				// calculate distance
					{
						float d = sqrt(((p1x - p2x) * (p1x - p2x)) + ((p1y - p2y) * (p1y - p2y))
							);
						if (d <= 0.5 + 1)
						{
							return true;
							break;
						}
					}
			}
		}
	}

	return false;
}

void specialKeys(int key, int x, int y) {
	int yc;
	int xc; 
	float p1x; 
	float p1y;
	float p1z;
	float p2x;
	float p2y;
	float p2z;
	float temp;

	//GLfloat d = 999;
	//Calculate the distance from the current position of the ball to all the vertices in boundary
	for (yc = 0; yc < 10; yc++)
	{
		for (xc = 0; xc < 8; xc++)
		{
			p1x = x_T;
			p1y = y_T;
			p1z = z_T;
			p2x = boundary[xc + yc * 8].x;
			p2y = boundary[xc + yc * 8].y;
			p2z = boundary[xc + yc * 8].z;
			/*if (p2x == p1x || p1y==p2y ){
			// calculate distance
			{
			float d = sqrt(((p1x - p2x) * (p1x - p2x)) + ((p1y - p2y) * (p1y - p2y))
			+ ((p1z - p2z) * (p1z - p2z)));
			d = sqrt(d);
			break;}
			}*/
		}
	}

	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT){
		//if(d>0.5+1)
		//y_T += 0.1;
		temp = y_T + 0.1;
		if (!collide(x_T, temp, 0.0f))
		{yAngle+=20;
			y_T += 0.1f;
		}
	}


	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT)
	{
		temp = y_T - 0.1;
		if (!collide(x_T, temp, 0.0f))
		{	y_T -= 0.1;
		yAngle-=20;}
	}
	else if (key == GLUT_KEY_UP){
		temp = x_T + 0.1;
		if (!collide(temp, y_T, 0.0f))
		{		x_T += 0.1;
		xAngle+=20;}
	}
	else if (key == GLUT_KEY_DOWN)
	{
		temp = x_T - 0.1;
		if (!collide(temp, y_T, 0.0f))
		{x_T -= 0.1;
		xAngle+=20;}
	}

	//  Request display update
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

//Drawing a single cube -> base unit of Maze
void drawCube(void){
	//White Maze
	float cubeMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };		
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cubeMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_Specular);


	glEnable(GL_TEXTURE_2D);
	//Texture Mapping
	glBindTexture(GL_TEXTURE_2D, g_Texture[0]);

	glBegin(GL_QUADS);

	/* Bottom Face */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	/* Upper Face */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	/* Right Face */
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	/* Left Face */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	/* Back face */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	/* Front Face */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glutPostRedisplay();

}

void drawSphere(void){
	//Red Sphere
	//float sphereMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float blue[] = { 0, 0, 1, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_Specular);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, g_Shininess); 

	glTranslatef(x_T, y_T, z_T);
	glRotatef(xAngle,1,0,0);
	glRotatef(yAngle,0,1,0);
	glutSolidSphere(0.5, 30, 30);
	glutPostRedisplay();
}

void drawBackground(void){
	//White Background
	float backgroundMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };		

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, backgroundMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_Specular);
	//glMaterialfv(GL_FRONT, GL_EMISSION, red);
	//glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, g_Shininess);

	//glMaterialfv(GL_FRONT, GL_SPECULAR, g_Specular);
	//glMaterialfv(GL_FRONT, GL_EMISSION, red);
	//glMateriali(GL_FRONT, GL_SHININESS, g_Shininess);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, g_Texture[1]);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, -50.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, -50.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 50.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 50.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

//Creating Textures
bool CreateTexture(GLuint &textureID, LPTSTR szFileName)                          // Creates Texture From A Bitmap File
{
	HBITMAP hBMP;                                                                 // Handle Of The Bitmap
	BITMAP  bitmap;																  // Bitmap Structure

	glGenTextures(1, &textureID);                                                 // Create The Texture
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBMP)                                                                    // Does The Bitmap Exist?
		return FALSE;                                                           // If Not Return False

	GetObject(hBMP, sizeof(bitmap), &bitmap);                                     // Get The Object
	// hBMP:        Handle To Graphics Object
	// sizeof(bitmap): Size Of Buffer For Object Information
	// &bitmap:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                                        // Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, textureID);                                      // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			  // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			  // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);
	// MUST NOT BE INDEX BMP, BUT RGB
	DeleteObject(hBMP);                                                           // Delete The Object

	return TRUE;                                                                  // Loading Was Successful
}