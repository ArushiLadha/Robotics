/************************************************************************************
  Introduction to Robotics : Project, Phase 1 : PPP Arm

  Arushi Ladha, 201030005
  Rashmi Tonge, 200902022
  ***********************************************************************************/
#include <iostream>
using namespace std;
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

typedef struct TCoach
{
	float x;
	float y;
	float z;
}Coach;

void startCoach();

float angleH=-1.0,angleV=0.0,deltaAngleH = 0.0,deltaAngleV = 0.0,ratio;
float x=98.0f,y=2.0f,z=-30.0f;
float lx=0.0f,ly=0.0f,lz=-0.1f;
float rotatewheel = 0;
GLint coach_display_list;
int deltaMove = 0,startTrain = 0;
float tlx,tly,tlz;
Coach coach;
int num_coach=1;
float shadow = 0;
float join = 0.05;
float chain = 0;
float vel = 0.1;
int camLR=0,camUD=0;
int l1=0,l2=0,l3=0;
float dx=0,dy=0,dz=0;
GLuint texture_id[1];
GLuint bg_id[1];
GLuint grass_id[1];

void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void drawCircle(float x,float y,float z,float radius,int flag)
{
	int i;
	float angle;
//	glColor3f(0.0f,0.0f,0.0f);
	if(flag == 1)
		glBegin(GL_POLYGON);
	else
	{
		glLineWidth(3);
		glBegin(GL_LINE_STRIP);
	}
	for(i=0;i<100;i++)
	{
		angle = i*2*M_PI/100;
		glVertex3f(x + radius*cos(angle),y + radius*sin(angle),z);
	}
	glEnd();
}

void drawLines(float x,float y,float z,float radius)
{
	int i;
	int flag = 0;
	float angle;
	//glBegin(GL_LINES);
	glLineWidth(2);
	for(i=0;i<100;i++)
	{
		if(i%10 == 0)
		{
			angle = i*2*M_PI/100;
			if(flag)
				glColor3f(0.0f,0.0f,0.0f);
			else
				glColor3f(1.0f,1.0f,1.0f);
			flag = 1 - flag;
			glBegin(GL_LINES);
			glVertex3f(x + radius*cos(angle),y + radius*sin(angle),z);
			glVertex3f(x,y,z);
			glEnd();
		}
	}
	glEnd();
}

void drawCuboid(float l,float h,float w)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3f(0, h, w);
		glVertex3f(l, h, w);
		glVertex3f(l, 0, w);
		glVertex3f(0, 0, w);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3f(0, h, 0);
		glVertex3f(l, h, 0);
		glVertex3f(l, 0, 0);
		glVertex3f(0, 0, 0);
	glEnd();

	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(0, h, w);
		glVertex3f(l, h, w);
		glVertex3f(l, h, 0);
		glVertex3f(0, h, 0);
	glEnd();

	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, w);
		glVertex3f(l, 0, w);
		glVertex3f(l, 0, 0);
		glVertex3f(0, 0, 0);
	glEnd();

	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, h, w);
		glVertex3f(0.0f, h, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, w);
	glEnd();

	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(l, h,w);
		glVertex3f(l, h,0.0f);
		glVertex3f(l, 0,0.0f);
		glVertex3f(l, 0,w);
	glEnd();
}

void drawArm(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	drawCuboid(0.1,0.1,3);

	glTranslatef(5,0,0);
	drawCuboid(0.1,0.1,3);
	glPopMatrix();
}

void drawLink1(float x,float y,float z)
{
	glPushMatrix();

	glTranslatef(x,y,z);

	glTranslatef(0.1,0,1.5);
	drawCuboid(5,0.1,0.1);

	glPushMatrix();
	glTranslatef(-0.05,0.05,0.05);
	glColor3f((float)139/255,0,0);
	gluCylinder(gluNewQuadric(),0.3,0.3,0.1,100,150);
	glColor3f(1,0,0);
	drawCircle(0,0,0,0.3,1);
	drawCircle(0,0,0.1,0.3,1);
	glTranslatef(-0.05,-0.05,-1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.05,0.05,0.05);
	glColor3f((float)139/255,0,0);
	gluCylinder(gluNewQuadric(),0.3,0.3,0.1,100,150);
	glColor3f(1,0,0);
	drawCircle(0,0,0,0.3,1);
	drawCircle(0,0,0.1,0.3,1);
	glTranslatef(-0.05,-0.05,-1.5);
	glPopMatrix();

	glPopMatrix();
}

void drawLink2(float x,float y,float z)
{
	int i;
	glPushMatrix();
	glTranslatef(x,y,z);

	glTranslatef(2.5,0.1,1.5);
	drawCuboid(0.1,1.5,0.1);

	//glPushMatrix();
	glTranslatef(0,0.05,0.05);
	glRotatef(90,0,1,0);
	glColor3f((float)139/255,0,0);
	gluCylinder(gluNewQuadric(),0.3,0.3,0.1,100,150);
	glColor3f(1,0,0);
	drawCircle(0,0,0,0.3,1);
	drawCircle(0,0,0.1,0.3,1);
	//glPopMatrix();

	glPopMatrix();
}

void drawLink3(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);

	glTranslatef(2.5,0.1,1.5);
	glTranslatef(-0.25,0.75,-0.25);
	drawCuboid(0.5,0.1,0.5);

	glPopMatrix();
}
GLuint createDL() {
	GLuint coachDL;

	// Create the id for the list
	coachDL = glGenLists(1);

	// start list
	glNewList(coachDL,GL_COMPILE);

	// call the function that contains the rendering commands
	startCoach();

	// endList
	glEndList();

	return(coachDL);
}

void initCoach()
{
	coach.x = 100.0;
	coach.y = 1.0;
	coach.z = -25.0;
	dx = 0;dy=0;dz=0;
}


void initScene() {

	coach_display_list = createDL();

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90,1.0,1,15);
        glMatrixMode(GL_MODELVIEW);

	initCoach();

}

void orientMe() {


	lx = sin(angleH)*cos(angleV);
	lz = -cos(angleH)*cos(angleV);
	ly =sin(angleV);
	glLoadIdentity();
	gluLookAt(x, y, z,
			x + lx,y + ly,z + lz,
			0.0f,1.0f,0.0f);
}


void moveMeFlat(float i) {
	x = x + i*(lx)*2;
	z = z + i*(lz)*2;
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void moveCamUD(float i)
{
	z = z + i*(-lz*ly)*1.5;
	x = x + i*(-lx*ly)*1.5;
	y = y + i*(lx*lx + lz*lz)*1.5;
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void moveCamLR(float i)
{
	x = x + i*(lz)*3;
	z = y + i*(-lx)*3;
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void startCoach()
{
	drawArm(coach.x-10,coach.y,coach.z-10);
	drawLink1(coach.x-10,coach.y,coach.z-10);
	drawLink2(coach.x-10,coach.y,coach.z-10);
	drawLink3(coach.x-10,coach.y,coach.z-10);

}

void renderScene(void) {


	//glClearColor(0.28f ,0.45f ,0.9823f ,0.0f);
	//glClearColor(0.13f, 0.5545f, 0.13f,0.0f);
	glClearColor((float)0 ,(float)0 ,(float)0 ,0.0f);

	if (deltaAngleH) {
		angleH += deltaAngleH*0.01;
	}
	if (deltaAngleV ) {
		if(angleV < M_PI/6.0 && angleV > -M_PI/6.0)
			angleV += deltaAngleV*0.01;
		else if (angleV > M_PI/6.0 && deltaAngleV < 0)
			angleV += deltaAngleV*0.01;
		else if (angleV < -M_PI/6.0 && deltaAngleV > 0)
			angleV += deltaAngleV*0.01;
	}
	orientMe();

	if(deltaMove)
		moveMeFlat(deltaMove*0.1);
	if(camUD)
		moveCamUD(camUD*0.1);
	if(camLR)
		moveCamLR(camLR*0.1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Draw ground
	//glColor3f(0.4196f ,0.5568f ,0.1372f);
	glColor3f(1.0f ,1.0f ,1.0f);

	drawArm(90.0,1.0,-35.0);

	if(l1 != 0)
	{
		coach.z += l1*0.005;
		if(coach.z >= -25.0 + 0.75)
			l1 = -1;
		if(coach.z <= -25.0 - 0.75)
			l1 = 1;
	}
	drawLink1(coach.x-10,coach.y,coach.z-10);
	if(l2 != 0)
	{
		dx += l2*0.005;
		if(dx >= 1.25)
		{
			l2 = -1;
		}
		if(dx <= -1.25)
		{
			l2 = 1;
		}
	}
	drawLink2(coach.x-10+dx,coach.y,coach.z-10);
	if(l3 != 0)
	{
		dy += l3*0.001;
		if(dy >= 0.375)
		{
			l3 = -1;
		}
		if(dy <= -0.375)
		{
			l3 = 1;
		}
	}
	drawLink3(coach.x-10+dx,coach.y+dy,coach.z-10);

	glutSwapBuffers();
}

void pressKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngleH = -1.0f;break;
		case GLUT_KEY_RIGHT : deltaAngleH = 1.0f;break;
		case GLUT_KEY_UP : deltaAngleV = 1.0f;break;
		case GLUT_KEY_DOWN : deltaAngleV = -1.0f;break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngleH = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaAngleV = 0;break;
	}
}

void processNormalKeys(unsigned char key, int x, int y) {

	switch(key)
	{
		case 119: deltaMove = 1;break;
		case 115: deltaMove = -1;break;
		case 'l': camLR = 1;break;
		case 'r': camLR = -1;break;
		case 'u': camUD = 1;break;
		case 'd': camUD = -1;break;
		case 'a': l1=1;l2=1;l3=1;break;
		case '1': l1=1;break;
		case '2': l2=1;break;
		case '3': l3=1;break;
		case 27 : exit(0);break;
	}
}

void releaseNormalKeys(unsigned char key, int x, int y) {
	switch(key)
	{
		case 119: deltaMove = 0;break;
		case 115: deltaMove = -0;break;
		case 'l': camLR = 0;break;
		case 'r': camLR = -0;break;
		case 'u': camUD = 0;break;
		case 'd': camUD = -0;break;
		case '1': l1=0;initCoach();break;
		case '2': l2=0;initCoach();break;
		case '3': l3=0;initCoach();break;
		case 'a': l1=0;l2=0;l3=0;initCoach();break;

	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1200,720);
	glutCreateWindow("SnowMen from Lighthouse 3D");

	initScene();

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(releaseNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}
