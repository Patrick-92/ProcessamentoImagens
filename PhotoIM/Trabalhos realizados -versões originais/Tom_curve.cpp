#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>

using namespace std;

int window1 = 0;
int window2 = 0;
int window3 = 0;

PixelLab *img1 = NULL, *img2 = NULL;

int windth, height, UI_width = 0;

int controlX[5] = {16,32,64,128,255};
int controlY[5] = {16,32,64,128,255};

bool passiveMotion = false;
int controlFlag = 0;

void displayImage1(void);
void displayImage3(void);
void displayImageTomCurve(void);
void mouse(int , int , int , int );
void motion(int , int );
int angular_coefficient (int , int , int , int );
int fundamental_equation (int , int , int , int );
void swap_tone (PixelLab *, int , int );
void interpolation (int [], int [], PixelLab *);

void init (void) {
	img1 = new PixelLab();
	img1->Read("../figs/lenaGray.png");

	img2 = new PixelLab();
	img2->CreateImage(img1->GetWidth(), img1->GetHeight());
}

void reshape( int x, int y ) {
  glutPostRedisplay();
}

void computeUIWidth() {
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = vw;
}

// Main Function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();

	glutMouseFunc(mouse);

	// Create two windows
	window1 = glutCreateWindow("Original Image");
  	window2 = glutCreateWindow("Control Image");
  	window3 = glutCreateWindow("Edited Image");

	//Janela da imagem
	glutSetWindow (window1);
	glutDisplayFunc(displayImage1);
	glutReshapeWindow(img1->GetWidth(), img1->GetHeight());
	glutPositionWindow(100, 100);

	//Curva de tom
	glutSetWindow (window2);
	glOrtho(0.0, 255.0, 0, 255.0, 0.0, 255.0);
	glutDisplayFunc(displayImageTomCurve);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(NULL);
	glutPositionWindow(550, 356);
	glutReshapeWindow(255, 255);

  	//Janela da Editada
	glutSetWindow (window3); // Change current window to 2
	glutDisplayFunc(displayImage3);
	glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
	glutPositionWindow(800, 100);

	glutMainLoop();

	return 0;
}

void displayImage1(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	glutSwapBuffers();
}

void displayImageTomCurve(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	glClearColor (1.0, 1.0, 1.0, 0.0);

	// Desenhando a curva inicial
	glColor3f (0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	for (int i = 0; i <= 4; i++) {
        glVertex2f(controlX[i], controlY[i]);
        glVertex2f(controlX[i], controlY[i]);
	}
	glEnd();

	//Desenhando os pontos de controle de tom 
	for (int i = 0; i < 4; i++) {
        glPushMatrix();
	        glTranslatef(controlX[i],controlY[i],0);
        	glColor3ub(0,0,0);
        	glutSolidCube(4);
	    glPopMatrix(); 
	}

	interpolation(controlX, controlY, img1);

	glutSwapBuffers();
  	glutPostRedisplay();
}

void displayImage3(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	glutSwapBuffers();
}

// Motion callback
void motion(int x, int y)
{
	// Inverte mouse para que origem fique no canto inferior esquerdo da janela
	// (por default, a origem � no canto superior esquerdo)
	y = 255 - y;

	if ((x >= controlX[0]-10 && x <= controlX[0]+10) && (y >= controlY[0]-10 && y <= controlY[0]+10)) {
		controlFlag = 1;
		controlX[0] = x;
		controlY[0] = y;
	}
	if ((x >= controlX[1]-10 && x <= controlX[1]+10) && (y >= controlY[1]-10 && y <= controlY[1]+10)) {
		controlFlag = 2;
		controlX[1] = x;
		controlY[1] = y;
	}
	if ((x >= controlX[2]-10 && x <= controlX[2]+10) && (y >= controlY[2]-10 && y <= controlY[2]+10)) {
		controlFlag = 3;
		controlX[2] = x;
		controlY[2] = y;
	}
	if ((x >= controlX[3]-10 && x <= controlX[3]+10) && (y >= controlY[3]-10 && y <= controlY[3]+10)) {
		controlFlag = 4;
		controlX[3] = x;
		controlY[3] = y;
	}
}

// Motion callback
void noMotion(int x, int y)
{
	// Cancel passive motion
}


void mouse(int button, int state, int x, int y)
{
	// Inverte mouse para que origem fique no canto inferior esquerdo da janela
	// (por default, a origem � no canto superior esquerdo)
	y = 255 - y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("\nBotao esquerdo pressionado na posicao [%d, %d].", x, y);
			switch (controlFlag) {
				case 1:
					controlX[0] = x;
					controlY[0] = y;
				case 2:
					controlX[1] = x;
					controlY[1] = y;
				case 3:
					controlX[2] = x;
					controlY[2] = y;
				case 4:
					controlX[3] = x;
					controlY[3] = y;
				default:
					cout << "\nNenhum botao pressionado";
			}
		}
		else
			printf("\nBotao esquerdo solto na posicao [%d, %d].", x, y);  // GLUT_UP
	}
	else
		printf("\nOutro botao do mouse pressionado.");
}

void interpolation (int control_x[], int control_y[], PixelLab *image) {
	int m, yf;

	for (int i = 0; i <= 3; i++) {
		m = angular_coefficient(control_x[i], control_y[i], control_x[i+1], control_y[i+1]);
		for (int j = 0; j < abs(control_x[i] - control_x[i+1]); j++) {
			yf = fundamental_equation(control_x[i], control_y[i], m, control_x[i]+j);
			swap_tone(image, control_x[i]+j, yf);
		}
	}
}

int angular_coefficient (int xi, int yi, int xf, int yf) {
	return (yf - yi) / (xf - xi);
}

int fundamental_equation (int xi, int yi, int m, int xf) {
	return m * (xf - xi) + yi;
}

void swap_tone (PixelLab *image, int originalTone, int newTone) {
	for (int x = 0; x < image->GetHeight(); x++) {
		for (int y = 0; y < image->GetWidth(); y++) {
			if (image->GetGrayValue(x,y) == originalTone) {
				img2->SetRGB(x, y, (uByte)newTone, (uByte)newTone, (uByte)newTone);
			}
		}
	}
	glutPostWindowRedisplay(window3);
}