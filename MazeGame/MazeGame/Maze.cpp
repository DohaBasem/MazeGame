//
// File:        mycube.c
// Author:      Matt Daisley
// Created:     4/25/2012
// Project:     Source code for Make a Cube in OpenGL
// Description: Creates an OpenGL window and draws a 3D cube
//              That the user can rotate using the arrow keys
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
#define GL_GLEXT_PROTOTYPES

#include "glut.h"
#pragma comment (lib, "glut32.lib")
// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void specialKeys();
 
// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
GLfloat rotationXaxis; 
GLfloat rotationYaxis; 
GLfloat rotationZaxis; 

double rotate_y=0; 
double rotate_x=0;
float g_X = 0.0f, g_Theta = 0.0f, g_Z = -1.0f, g_Y = 0.0f;
 //Maze matrix
int maze[] = { 1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,1,
	       1,0,1,1,0,1,0,1,
	       1,0,1,0,0,1,1,1,
	       0,0,1,1,0,0,0,0,
	       1,0,0,1,0,1,1,1,
	       1,0,0,0,0,0,0,1,
	       1,0,1,1,1,1,0,1,
	       1,0,0,0,0,0,0,1,
               1,1,1,1,1,1,1,1
             };


//A function to Draw a single Cube
void DrawCube(){
  //Multi-colored side - FRONT
  glBegin(GL_QUADS);
      /* Front Face */
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 1.0f );

      /* Back Face */
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );

      /* Top Face */
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.0f,  1.0f,  1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );

      /* Bottom Face */
      /* Top Right Of The Texture and Quad */
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f,  1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );

      /* Right face */
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 1.0f,  1.0f, -1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f,  1.0f,  1.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f,  1.0f );

      /* Left Face */
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
    glEnd( );

}

void display(){
 
 glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 gluLookAt(0.0, 0.0, -20, g_X, 0.0, 0.0, 1, 0, 0);
    /* Move Into The Screen 5 Units */
 //Doha:Viewing Transformatin --> aiming and positioning the camera
    glLoadIdentity( );
	 
   // glTranslatef( -10.0f, -8.0f, -34.0f );
    glRotatef( rotationXaxis, 0.0f, 1.0f, 0.0f);
 
  // Other Transformations
  // glTranslatef( 0.1, 0.0, 0.0 );      // Not included
  // glRotatef( 180, 0.0, 1.0, 0.0 );    // Not included
 
  // Rotate when user changes rotate_x and rotate_y
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );
 
  // Other Transformations
  // glScalef( 2.0, 2.0, 0.0 );          // Not included
 
   int yc = 0;
    int xc = 0;
    for (yc = 0; yc < 10; yc++)
    {
    for (xc = 0; xc < 8; xc++)
    {
      if (maze[ xc + yc *8] == 1)
      {
	    //glPushMatrix();
	   
		
		   glTranslatef( xc*0.25f, yc*0.25f, -0.0f );
	   //DrawCube();
		   glutWireCube(0.25);
	    //glPopMatrix();
      }
    }
    }
 /*int i=0;
	while(i<3)
  //glFlush();
  glPushMatrix();
  glutWireCube(.25);
  glPopMatrix();
  i++;*/
//	glTranslatef( 1.05, 1.0f, -0.0f );
  glutSwapBuffers();
   /* Rotate Cube */
    rotationXaxis += 0.3f; 
    rotationYaxis += 0.2f;
}
 
// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 5;
 
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 5;
 
  else if (key == GLUT_KEY_UP)
    rotate_x += 5;
 
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;
 
  //  Request display update
  glutPostRedisplay();
 
}
 
// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){
 
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);
 
  //  Request double buffered true color window with Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
  //  Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);
  //The way of testing z-buffer
   glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

 
  // Create window
  glutCreateWindow("Awesome Cube");
 
  //Set projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(90.0f, 1.0f, 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);

	//gluPerspective(60.0f, 1.0f, 0.7f, 100.0f);
	//glMatrixMode(GL_MODELVIEW);
  // Callback functions
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
 
  //  Pass control to GLUT for events
  glutMainLoop();
 
  //  Return to OS
  return 0;
 
}