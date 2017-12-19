#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>

#define COLOR_LIMIT 255
#define SCALE_DELTA 0.005


bool escala = false, escala2 = false, rotacao = false, rotacao2 = false, translacao = false, translacao2 = false;
double mousePositionX = 0.0, mousePositionY = 0.0, lastPositionY = 0.0, mousePositionX2 = 0.0, mousePositionY2 = 0.0, lastPositionY2 = 0.0;
double scaleFactor = 1.0, scaleFactor2 = 1.0;

int window1 = 0, window2 = 0, window3 = 0, draggingPoint = 0;

std::vector<double> xAxisPoints, yAxisPoints;

PixelLab *img1 = NULL, *img2 = NULL, *img3;
pixel **pixelMatrix1, **pixelMatrix2, **pixelMatrix3, **pixelMatrixResul;


int max(int a, int b) {
	
	if(a > b)
		return a;
	else
		return b;

}

int doXOR(int x, int y){
    
    int a = x & y;
    int b = ~x & ~y;
    int z = ~a & ~b;
    return z;

} 

void displayimg1(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(mousePositionX, mousePositionY, 0.0);
	glRasterPos2f(-100, -100);
	glPixelZoom(scaleFactor, scaleFactor);
	img1->ViewImage();
	glPopMatrix();
	glutSwapBuffers();

}

void displayimg2(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(mousePositionX2, mousePositionY2, 0.0);
	glRasterPos2f(-100, -100);
	glPixelZoom(scaleFactor2, scaleFactor2);
	img2->ViewImage();
	glPopMatrix();
	glutSwapBuffers();

}

void displayimg3(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	img3->ViewImage();
	glPopMatrix();
	glutSwapBuffers();

}

void AND() {
	
	img1 -> GetDataAsMatrix(pixelMatrix1);
	img2 -> GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1->GetHeight() > img2->GetHeight())
		height = img1->GetHeight();
	else
		height = img2->GetHeight();

	if(img1->GetWidth() > img2->GetWidth())
		width = img1->GetWidth();
	else
		width = img2->GetWidth();

	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );
	img3 -> AllocatePixelMatrix(&pixelMatrix3, width, height);

	
	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){

			pixelMatrix3[x][y].R = pixelMatrix1[x][y].R & pixelMatrix2[x][y].R;
			pixelMatrix3[x][y].G = pixelMatrix1[x][y].G & pixelMatrix2[x][y].G;
			pixelMatrix3[x][y].B = pixelMatrix1[x][y].B & pixelMatrix2[x][y].B;
				
		}
	}

	img3 -> SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

void OR() {
	
	img1->GetDataAsMatrix(pixelMatrix1);
	img2->GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1->GetHeight() > img2->GetHeight())
		height = img1->GetHeight();
	else
		height = img2->GetHeight();

	if(img1->GetWidth() > img2->GetWidth())
		width = img1->GetWidth();
	else
		width = img2->GetWidth();

	img3->DeallocatePixelMatrix(&pixelMatrix3, img3->GetHeight(), img3->GetWidth() );
	img3->AllocatePixelMatrix(&pixelMatrix3, width, height);

	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){

			pixelMatrix3[x][y].R = pixelMatrix1[x][y].R | pixelMatrix2[x][y].R;
			pixelMatrix3[x][y].G = pixelMatrix1[x][y].G | pixelMatrix2[x][y].G;
			pixelMatrix3[x][y].B = pixelMatrix1[x][y].B | pixelMatrix2[x][y].B;
				
		}
	}

	img3->SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

void XOR() {
	
	img1 -> GetDataAsMatrix(pixelMatrix1);
	img2 -> GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1 -> GetHeight() > img2 -> GetHeight())
		height = img1 -> GetHeight();
	else
		height = img2 -> GetHeight();

	if(img1 -> GetWidth() > img2 -> GetWidth())
		width = img1 -> GetWidth();
	else
		width = img2 -> GetWidth();

	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );
	img3 -> AllocatePixelMatrix(&pixelMatrix3, width, height);

	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){

			pixelMatrix3[y][x].R = doXOR(pixelMatrix1[y][x].R, pixelMatrix2[y][x].R);
			pixelMatrix3[y][x].G = doXOR(pixelMatrix1[y][x].G, pixelMatrix2[y][x].G);
			
			pixelMatrix3[y][x].B = doXOR(pixelMatrix1[y][x].B, pixelMatrix2[y][x].B);
		}
	}

	img3 -> SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

void rotate(){

	img1 -> GetDataAsMatrix(pixelMatrix1);
	pixel **pixelMatrixAux;
	img1 -> AllocatePixelMatrix(&pixelMatrixAux, img1 -> GetHeight(), img1 -> GetWidth());

	for (int y = 0; y < img1 -> GetHeight(); y++)
		for (int x = 0; x < img1 -> GetWidth(); x++) {
			pixelMatrixAux[y][x].R = pixelMatrix1[x][img1 -> GetHeight()-1-y].R;
			pixelMatrixAux[y][x].G = pixelMatrix1[x][img1 -> GetHeight()-1-y].G;
			pixelMatrixAux[y][x].B = pixelMatrix1[x][img1 -> GetHeight()-1-y].B;
			pixelMatrixAux[y][x].value = pixelMatrix1[x][img1 -> GetHeight()-1-y].value;
		}

	img1 -> SetDataAsMatrix(pixelMatrixAux);
	glutPostWindowRedisplay(window1);

}

void rotate2(){

	img2 -> GetDataAsMatrix(pixelMatrix1);
	pixel **pixelMatrixAux;
	img2 -> AllocatePixelMatrix(&pixelMatrixAux, img2 -> GetHeight(), img2 -> GetWidth());

	for (int y = 0; y < img2 -> GetHeight(); y++)
		for (int x = 0; x < img2 -> GetWidth(); x++) {
			pixelMatrixAux[y][x].R = pixelMatrix1[x][img2 -> GetHeight()-1-y].R;
			pixelMatrixAux[y][x].G = pixelMatrix1[x][img2 -> GetHeight()-1-y].G;
			pixelMatrixAux[y][x].B = pixelMatrix1[x][img2 -> GetHeight()-1-y].B;
			pixelMatrixAux[y][x].value = pixelMatrix1[x][img2 -> GetHeight()-1-y].value;
		}

	img2 -> SetDataAsMatrix(pixelMatrixAux);
	glutPostWindowRedisplay(window1);

}

void Adicao(){

	img1 -> GetDataAsMatrix(pixelMatrix1);
	img2 -> GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1 -> GetHeight() > img2 -> GetHeight())
		height = img1 -> GetHeight();
	else
		height = img2 -> GetHeight();

	if(img1 -> GetWidth() > img2 -> GetWidth())
		width = img1 -> GetWidth();
	else
		width = img2 -> GetWidth();

	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );
	img3 -> AllocatePixelMatrix(&pixelMatrix3, width, height);

	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){

			pixelMatrix3[x][y].R = pixelMatrix1[x][y].R + pixelMatrix2[x][y].R;
			pixelMatrix3[x][y].G = pixelMatrix1[x][y].G + pixelMatrix2[x][y].G;
			pixelMatrix3[x][y].B = pixelMatrix1[x][y].B + pixelMatrix2[x][y].B;
				
		}
	}

	img3 -> SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

void Subtracao(){

	img1 -> GetDataAsMatrix(pixelMatrix1);
	img2 -> GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1 -> GetHeight() > img2 -> GetHeight())
		height = img1 -> GetHeight();
	else
		height = img2 -> GetHeight();

	if(img1 -> GetWidth() > img2 -> GetWidth())
		width = img1 -> GetWidth();
	else
		width = img2 -> GetWidth();

	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );
	img3 -> AllocatePixelMatrix(&pixelMatrix3, width, height);

	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){

			pixelMatrix3[x][y].R = pixelMatrix1[x][y].R - pixelMatrix2[x][y].R;
			pixelMatrix3[x][y].G = pixelMatrix1[x][y].G - pixelMatrix2[x][y].G;
			pixelMatrix3[x][y].B = pixelMatrix1[x][y].B - pixelMatrix2[x][y].B;
				
		}
	}

	img3 -> SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

void Multiplicacao(){

	img1 -> GetDataAsMatrix(pixelMatrix1);
	img2 -> GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1 -> GetHeight() > img2 -> GetHeight())
		height = img1 -> GetHeight();
	else
		height = img2 -> GetHeight();

	if(img1 -> GetWidth() > img2 -> GetWidth())
		width = img1 -> GetWidth();
	else
		width = img2 -> GetWidth();

	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );
	img3 -> AllocatePixelMatrix(&pixelMatrix3, width, height);

	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){

			pixelMatrix3[x][y].R = pixelMatrix1[x][y].R * pixelMatrix2[x][y].R;
			pixelMatrix3[x][y].G = pixelMatrix1[x][y].G * pixelMatrix2[x][y].G;
			pixelMatrix3[x][y].B = pixelMatrix1[x][y].B * pixelMatrix2[x][y].B;
				
		}
	}

	img3 -> SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

void Divisao(){

	img1 -> GetDataAsMatrix(pixelMatrix1);
	img2 -> GetDataAsMatrix(pixelMatrix2);

	double width = 0.0, height = 0.0;

	if(img1 -> GetHeight() > img2 -> GetHeight())
		height = img1 -> GetHeight();
	else
		height = img2 -> GetHeight();

	if(img1 -> GetWidth() > img2 -> GetWidth())
		width = img1 -> GetWidth();
	else
		width = img2 -> GetWidth();

	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );
	img3 -> AllocatePixelMatrix(&pixelMatrix3, width, height);

	for (int x = 0; x < height; x++){
		for (int y = 0; y < width; y++){
			
			if((pixelMatrix1[x][y].R == 0) || (pixelMatrix2[x][y].R == 0))
				pixelMatrix3[x][y].R = 0;
			
			else
				if((pixelMatrix1[x][y].G == 0) || (pixelMatrix2[x][y].G == 0))
					pixelMatrix3[x][y].G = 0;
				
				else
					if((pixelMatrix1[x][y].B == 0) || (pixelMatrix2[x][y].B == 0))
						pixelMatrix3[x][y].B = 0;
					
					else{

						pixelMatrix3[x][y].R = pixelMatrix1[x][y].R / pixelMatrix2[x][y].R;
						pixelMatrix3[x][y].G = pixelMatrix1[x][y].G / pixelMatrix2[x][y].G;
						pixelMatrix3[x][y].B = pixelMatrix1[x][y].B / pixelMatrix2[x][y].B;
					}
				
		}
	}

	img3 -> SetDataAsMatrix(pixelMatrix3);
	glutPostWindowRedisplay(window3);

}

static void teclado(unsigned char tecla, int x, int y) {
	
	switch (tecla) {
	
		case 'e':
			AND();
			break;

		case 'E':
			AND();		
			break;

		case 'o':	
			OR();		
			break;

		case 'O':	
			OR();
			break;

		case 'x':
			XOR();
			break;

		case 'X':
			XOR();
			break;

		case 'r':
			rotate();
			break;

		case 'R':
			rotate();
			break;

		case 't':
			rotate2();
			break;

		case 'T':
			rotate2();
			break;

		case '+':
			Adicao();
			break;

		case '-':
			Subtracao();
			break;

		case '*':
			Multiplicacao();
			break;

		case '/':
			Divisao();
			break;
		
		case 'q':
			exit(0);

		case 'Q':
			exit(0);	

		
		
	}

	glutPostRedisplay();

}

void mouseClickEvents(int button, int state, int x, int y) {
	
	if(button == GLUT_LEFT_BUTTON) {
		switch(state) {
			case GLUT_DOWN:
				translacao = true;
				break;

			case GLUT_UP:
				translacao = false;
				break;
		}

	}
	

	if(button == GLUT_RIGHT_BUTTON) {
		switch(state) {
			case GLUT_DOWN:

				if(!escala) {
					escala = true;
					lastPositionY = y;

				}
				break;

			case GLUT_UP:
				escala = false;
				lastPositionY = 0.0;
				break;
		}

	}

}

void mouseMotionEvents(int x, int y ) {

	if(translacao) {
		y = 255 - y;
		mousePositionX = x;
		mousePositionY = y;
		glutPostWindowRedisplay(window1);
	}

	if(escala) {
		if(y < lastPositionY)
			scaleFactor -= SCALE_DELTA;
		else
			scaleFactor += SCALE_DELTA;

		if(scaleFactor < SCALE_DELTA)
			scaleFactor = SCALE_DELTA;

		glutPostWindowRedisplay(window1);
		glutReshapeWindow(img1 -> GetWidth()*scaleFactor, img1 -> GetHeight()*scaleFactor);
	}

}

void mouseClickEvents2(int button, int state, int x, int y) {
	
	if(button == GLUT_LEFT_BUTTON) {
		switch(state) {
			case GLUT_DOWN:
				translacao2 = true;
				break;

			case GLUT_UP:
				translacao2 = false;
				break;
		}

	}

	if(button == GLUT_MIDDLE_BUTTON) {
		switch(state) {
			case GLUT_DOWN:
				rotate2();
				break;

			case GLUT_UP:
				break;
		}

	}
	

	if(button == GLUT_RIGHT_BUTTON) {
		switch(state) {
			case GLUT_DOWN:

				if(!escala2) {
					escala2 = true;
					lastPositionY2 = y;

				}
				break;

			case GLUT_UP:
				escala2 = false;
				lastPositionY2 = 0.0;
				break;
		}

	}

}

void mouseMotionEvents2(int x, int y ) {

	if(translacao2){
		y = 255 - y;
		mousePositionX2 = x;
		mousePositionY2 = y;
		glutPostWindowRedisplay(window2);
	}

	if(escala2){

		if(y < lastPositionY2)
			scaleFactor2 -= SCALE_DELTA;
		else
			scaleFactor2 += SCALE_DELTA;

		if(scaleFactor2 < SCALE_DELTA)
			scaleFactor2 = SCALE_DELTA;

		glutPostWindowRedisplay(window2);
		glutReshapeWindow(img2 -> GetWidth()*scaleFactor2, img2 -> GetHeight()*scaleFactor2);
	}

/*	if(rotacao2){
		printf("cheguei\n");
		
		pixel **pixelMatrixAux;
		img2 -> AllocatePixelMatrix(&pixelMatrixAux, img2 -> GetHeight(), img2 -> GetWidth());

		for (int y = 0; y < img2 -> GetHeight(); y++){
			for (int x = 0; x < img2 -> GetWidth(); x++) {
				pixelMatrixAux[y][x].R = pixelMatrix1[x][img2 -> GetHeight()-1-y].R;
				pixelMatrixAux[y][x].G = pixelMatrix1[x][img2 -> GetHeight()-1-y].G;
				pixelMatrixAux[y][x].B = pixelMatrix1[x][img2 -> GetHeight()-1-y].B;
				pixelMatrixAux[y][x].value = pixelMatrix1[x][img2 -> GetHeight()-1-y].value;
			}
		}

		img2 -> SetDataAsMatrix(pixelMatrixAux);
		glutPostWindowRedisplay(window2);
	}*/

}


void loadImages() {
	img1 = new PixelLab();
	img1->Read("../figs/split.example1.png");

	img2 = new PixelLab();
	img2->Read("../figs/split.example2.png");

	img3 = new PixelLab();
	img3->Copy(img1);

	img1->AllocatePixelMatrix(&pixelMatrix1, img1->GetHeight(), img1->GetWidth());
	img2->AllocatePixelMatrix(&pixelMatrix2, img2->GetHeight(), img2->GetWidth());
	img3->AllocatePixelMatrix(&pixelMatrix3, img3->GetHeight(), img3->GetWidth());

}

void initData() {

	window1 = glutCreateWindow("Image 1 Window");
	window2 = glutCreateWindow("Image 2 Window");
	window3 = glutCreateWindow("Image 3 Window");
	loadImages();

}

void displayWindows() {
	glutSetWindow(window1);
	glutDisplayFunc(displayimg1);
	glutReshapeWindow(img1->GetWidth(), img1->GetHeight());
	glutPositionWindow(50, 50);
	glutMouseFunc(mouseClickEvents);
	glutMotionFunc(mouseMotionEvents);
	glutKeyboardFunc(teclado);

	glutSetWindow(window2);
	glutDisplayFunc(displayimg2);
	glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
	glutPositionWindow(img1->GetWidth() + 100, 100);
	glutMouseFunc(mouseClickEvents2);
	glutMotionFunc(mouseMotionEvents2);
	glutKeyboardFunc(teclado);

	glutSetWindow(window3);
	glutDisplayFunc(displayimg3);
	glutReshapeWindow(img3->GetWidth(), img3->GetHeight());
	glutPositionWindow(img2->GetWidth() + 150, 150);
	glutKeyboardFunc(teclado);

}

int main(int argc, char *argv[]) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	initData();

	displayWindows();
	glutMainLoop();

	img1 -> DeallocatePixelMatrix(&pixelMatrix1, img1 -> GetHeight(), img1 -> GetWidth() );
	img2 -> DeallocatePixelMatrix(&pixelMatrix2, img2 -> GetHeight(), img2 -> GetWidth() );
	img3 -> DeallocatePixelMatrix(&pixelMatrix3, img3 -> GetHeight(), img3 -> GetWidth() );

	return 0;

}