#include <GL/gl.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "main.h"
#include "glut.h"
GLfloat d;

GLfloat p1x;
GLfloat p1y;
GLfloat p1z;

const int p1radius = 1;
const int  p2radius = 0;

GLfloat p2x;
GLfloat p2y;
GLfloat p2z;

void collision (void) {
d = sqrt(((p1x - p2x) * (p1x - p2x)) + ((p1y - p2y) * (p1y - p2y)) 
+ ((p1z - p2z) * (p1z - p2z)));
}

void pointz (void) {
glPushMatrix();
if (d <= p2radius + p1radius)
{
glColor3f(1, 0, 0);
}
else
{
glColor3f(0, 0, 1);
}
glBegin(GL_POINTS);
glVertex3f(p1x, p1y, p1z);
glEnd();
glPopMatrix();
glPushMatrix();
glColor3f(0, 1, 0);
glBegin(GL_POINTS);
glVertex3f(p2x, p2y, p2z);
glEnd();
glPopMatrix();
}

void display (void) {
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity();  
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPointSize(5);
    collision();
    pointz();
    glutSwapBuffers();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y) {
if (key=='q') {
p1z = p1z - 0.1;
}

if (key=='z') {
p1z = p1z + 0.1;
}

if (key=='w') {
p1y = p1y + 0.1;
}

if (key=='s') {
p1y = p1y - 0.1;
}

if (key=='a') {
p1x = p1x - 0.1;
}

if (key=='d') {
p1x = p1x + 0.1;
}

if (key=='i') {
p2y = p2y + 0.1;
}

if (key=='k') {
p2y = p2y - 0.1;
}

if (key=='j') {
p2x = p2x - 0.1;
}

if (key=='l') {
p2x = p2x + 0.1;
}

if (key==27) { //27 is the ascii code for the ESC key
    exit (0); //end the program
}}
/* int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE); //set up the double 

    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("A basic OpenGL Window");
    glutDisplayFunc (display);
    glutIdleFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);//the call for the keyboard 

    glutMainLoop ();
    return 0;
}*/