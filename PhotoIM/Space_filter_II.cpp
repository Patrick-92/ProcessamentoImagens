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

int main_window;

struct ponto {
  uByte r;
  uByte g;
  uByte b;
};

string textFile, textRadius;

GLUI_TextBox     *moo;
GLUI_EditText    *edittext;
GLUI_Panel       *obj_panel;
GLUI_Button	*bImportar, *bSair;

#define CMD_BTN_1 100
#define CMD_BTN_2 101
#define CMD_BTN_3 102
#define CMD_BTN_4 103
#define CMD_BTN_5 104
#define CMD_BTN_6 105
#define CMD_BTN_7 106
#define CMD_BTN_8 107
#define CMD_BTN_9 108
#define CMD_BTN_10 109
#define CMD_BTN_11 110
#define CMD_BTN_12 111

void displayImage1(void);
void displayImage3(void);
void displayImage4(void);
void displayImage5(void);
void displayImage6(void);
void displayImage7(void);
void displayImage8(void);
void displayImage9(void);
void displayImage10(void);
void displayImage11(void);
void displayImage12(void);
void displayImage13(void);
void displayImageTomCurve(void);
void pointer_cb (GLUI_Control* );
void myGlutIdle(void);
struct ponto to_filter(PixelLab * , int , int , int );
void low_pass_average_1_9(PixelLab *);
void low_pass_average_1_25(PixelLab *);
void low_pass_average_1_49(PixelLab *);
void Gaussian_filter(PixelLab *);
void Gaussian_filter_approximate(PixelLab *);
void high_pass_sobel_vertical(PixelLab *);
void high_pass_sobel_horizontal(PixelLab *);
void high_pass_sobel_both(PixelLab *);
void high_pass_prewitt_vertical(PixelLab *);
void high_pass_prewitt_horizontal(PixelLab *);
void high_pass_prewitt_both(PixelLab *);
void incializar(void);

void init (void) {
	string caminho = "../figs/" + string(edittext->get_text()) + ".png";

	img1 = new PixelLab();
	img1->Read(caminho.c_str());

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

	/*   GLUI   */
	GLUI *glui = GLUI_Master.create_glui("Filtros");

	edittext = new GLUI_EditText( glui, "Nome do Arquivo:", textFile, CMD_BTN_12, pointer_cb );
		edittext->set_w( 200 );
	bImportar = new GLUI_Button(glui, "Importar Arquivo", CMD_BTN_12, pointer_cb);

	obj_panel = new GLUI_Panel(glui, "Filtros no dominio do espaço" );

	new GLUI_Button(obj_panel, "Passa-baixa - Media (1/9)", CMD_BTN_1, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-baixa - Media (1/25)", CMD_BTN_2, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-baixa - Media (1/49)", CMD_BTN_3, pointer_cb);
	new GLUI_Button(obj_panel, "Filtro Gaussiano (1/273)", CMD_BTN_4, pointer_cb);
	new GLUI_Button(obj_panel, "Filtro Gaussiano (1/16)", CMD_BTN_5, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-alta - Sobel (vertical)", CMD_BTN_6, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-alta - Sobel (horizontal)", CMD_BTN_7, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-alta - Sobel (ambos)", CMD_BTN_8, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-alta - Prewitt (vertical)", CMD_BTN_9, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-alta - Prewitt (horizontal)", CMD_BTN_10, pointer_cb);
	new GLUI_Button(obj_panel, "Passa-alta - Prewitt (ambos)", CMD_BTN_11, pointer_cb);

	new GLUI_Button(glui, "Sair", 0, (GLUI_Update_CB)exit);

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

	//interpolation(controlX, controlY, img1);

	glutSwapBuffers();
  	glutPostRedisplay();
}

void displayImage3(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	low_pass_average_1_9(img1);

	glutSwapBuffers();
}

void displayImage4(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	low_pass_average_1_25(img1);
	
	glutSwapBuffers();
}

void displayImage5(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	low_pass_average_1_49(img1);

	glutSwapBuffers();
}

void displayImage6(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	Gaussian_filter(img1);

	glutSwapBuffers();
}
void displayImage7(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	high_pass_sobel_vertical(img1);

	glutSwapBuffers();
}
void displayImage8(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	high_pass_sobel_horizontal(img1);

	glutSwapBuffers();
}
void displayImage9(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	high_pass_prewitt_vertical(img1);

	glutSwapBuffers();
}
void displayImage10(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	high_pass_prewitt_horizontal(img1);
	
	glutSwapBuffers();
}
void displayImage11(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	Gaussian_filter_approximate(img1);	

}

void displayImage12(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	high_pass_sobel_both(img1);

}

void displayImage13(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	high_pass_prewitt_both(img1);

}

void pointer_cb (GLUI_Control* control){
	if (control->get_id() == CMD_BTN_1) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage3);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_2) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage4);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_3) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage5);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_4) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage11);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_5) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage6);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_6) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage7);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_7) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage8);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_8) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage12);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_9) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage9);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_10) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage10);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_11) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage13);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_11) {
		glutDestroyWindow(window3);
		window3 = glutCreateWindow("Edited Image");
		//Janela da Editada
		glutSetWindow (window3); // Change current window to 2
		glutDisplayFunc(displayImage13);
		glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
		glutPositionWindow(800, 100);
	}
	if (control->get_id() == CMD_BTN_12) {
		incializar();
	}
}

void myGlutIdle( void ) {
	if (glutGetWindow() != main_window)
	{
		glutSetWindow(main_window);
		glutPostRedisplay();
	}
}

void low_pass_average_1_9(PixelLab *image) 
{
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			if (x == 0)
			{
				if (y == 0)
				{
					//Baixo - esquerda
					newPixel = to_filter(image, x, y, 2);
					img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
					continue;
				}
				if (y+1 == image->GetWidth())
				{
					//Cima - esquerda
					newPixel = to_filter(image, x, y, 3);
					img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
					continue;
				}
				else
				{
					//Vertical esquerda
					newPixel = to_filter(image, x, y, 4);
					img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
					continue;
				}
			}
			if (x+1 == image->GetHeight())
			{
				if (y == 0)
				{
					//Baixo - direita
					newPixel = to_filter(image, x, y, 5);
					img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
					continue;
				}
				if (y+1 == image->GetWidth())
				{
					//Cima - direita
					newPixel = to_filter(image, x, y, 6);
					img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
					continue;
				}
				else
				{
					//Vertical direita
					newPixel = to_filter(image, x, y, 7);
					img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
					continue;
				}
			}
			if (y == 0)
			{
				//Horizontal baixo
				newPixel = to_filter(image, x, y, 8);
				img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
				continue;
			}
			if (y+1 == image->GetWidth())
			{
				//Horizontal cima
				newPixel = to_filter(image, x, y, 9);
				img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
				continue;
			}
			else
			{
				//Regiao central
				newPixel = to_filter(image, x, y, 1);
				img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
			}
		}
	}
}

void low_pass_average_1_25(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 10);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void low_pass_average_1_49(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 12);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void Gaussian_filter(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 12);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void Gaussian_filter_approximate(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 17);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void high_pass_sobel_vertical(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 13);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void high_pass_sobel_horizontal(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 14);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void high_pass_prewitt_vertical(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 15);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void high_pass_prewitt_horizontal(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 16);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void high_pass_sobel_both(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 18);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

void high_pass_prewitt_both(PixelLab *image) {
	
	struct ponto newPixel;

	for (int x = 0; x < image->GetHeight(); x++) 
	{
		for (int y = 0; y < image->GetWidth(); y++) 
		{
			newPixel = to_filter(image, x, y, 19);
			img2->SetRGB(x, y, (uByte)newPixel.r, (uByte)newPixel.g, (uByte)newPixel.b);
		}
	}	
}

struct ponto to_filter(PixelLab *image, int x, int y, int matrix)
{
		struct ponto filter;
		struct ponto filter2;

		int verificador = 0;
		
		filter.r = 0;
		filter.g = 0;
		filter.b = 0;

		switch (matrix)
		{
			case 1:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 2:
				for (int i = x; i < x+2; i++)
				{
					for (int j = y; j < y+2; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 3:
				for (int i = x-1; i < x; i++)
				{
					for (int j = y; j < y+2; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 4:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y; j < y+2; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 5:
				for (int i = x; i < x+2; i++)
				{
					for (int j = y-1; j < y; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 6:
				for (int i = x-1; i < x; i++)
				{
					for (int j = y-1; j < y; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 7:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y-1; j < y; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 8:
				for (int i = x; i < x+2; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 9:
				for (int i = x-1; i < x; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						filter.r += image->GetR(i, j)/9;
						filter.g += image->GetG(i, j)/9;
						filter.b += image->GetB(i, j)/9;
					}
				}
				break;
			case 10:
				for (int i = x-2; i < x+3; i++)
				{
					for (int j = y-2; j < y+3; j++)
					{
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							filter.r += image->GetR(i, j)/25;
							filter.g += image->GetG(i, j)/25;
							filter.b += image->GetB(i, j)/25;
						}
						
					}
				}
				break;
			case 11:
				for (int i = x-3; i < x+4; i++)
				{
					for (int j = y-3; j < y+4; j++)
					{
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							filter.r += image->GetR(i, j)/49;
							filter.g += image->GetG(i, j)/49;
							filter.b += image->GetB(i, j)/49;
						}
						
					}
				}
				break;
			case 12:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						verificador++;
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							if ((verificador % 2) != 0 && verificador != 5)
							{
								filter.r += image->GetR(i, j)/16;
								filter.g += image->GetG(i, j)/16;
								filter.b += image->GetB(i, j)/16;
							}
							if ((verificador%2) == 0)
							{
								filter.r += (image->GetR(i, j)*2)/16;
								filter.g += (image->GetG(i, j)*2)/16;
								filter.b += (image->GetB(i, j)*2)/16;
							}
							if (verificador == 5)
							{
								filter.r += (image->GetR(i, j)*4)/16;
								filter.g += (image->GetG(i, j)*4)/16;
								filter.b += (image->GetB(i, j)*4)/16;
							}
						}
						
					}
				}
				break;
			case 13:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						verificador++;
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							if (verificador == 1 || verificador == 7)
							{
								filter.r += image->GetR(i, j)*-1;
								filter.g += image->GetG(i, j)*-1;
								filter.b += image->GetB(i, j)*-1;
							}
							if (verificador == 3 || verificador == 9)
							{
								filter.r += image->GetR(i, j);
								filter.g += image->GetG(i, j);
								filter.b += image->GetB(i, j);
							}
							if (verificador == 4)
							{
								filter.r += image->GetR(i, j)*-2;
								filter.g += image->GetG(i, j)*-2;
								filter.b += image->GetB(i, j)*-2;
							}
							if (verificador == 6)
							{
								filter.r += image->GetR(i, j)*2;
								filter.g += image->GetG(i, j)*2;
								filter.b += image->GetB(i, j)*2;
							}
						}
						
					}
				}

				if((abs(filter.r += image->GetR(x, y))) + (abs(filter.g += image->GetR(x, y))) + (abs(filter.b += image->GetR(x, y))) < 60)
				{   
				    filter.r = 255;
				    filter.g = 255;
				    filter.b = 255;
				}
				else
				{
				    filter.r = 0;
				    filter.g = 0;
				    filter.b = 0;
				}
				break;
			case 14:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						verificador++;
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							if (verificador == 1 || verificador == 3)
							{
								filter.r += image->GetR(i, j)*-1;
								filter.g += image->GetG(i, j)*-1;
								filter.b += image->GetB(i, j)*-1;
							}
							if (verificador == 7 || verificador == 9)
							{
								filter.r += image->GetR(i, j);
								filter.g += image->GetG(i, j);
								filter.b += image->GetB(i, j);
							}
							if (verificador == 2)
							{
								filter.r += image->GetR(i, j)*-2;
								filter.g += image->GetG(i, j)*-2;
								filter.b += image->GetB(i, j)*-2;
							}
							if (verificador == 8)
							{
								filter.r += image->GetR(i, j)*2;
								filter.g += image->GetG(i, j)*2;
								filter.b += image->GetB(i, j)*2;
							}
						}
						
					}
				}
				if((abs(filter.r += image->GetR(x, y))) + (abs(filter.g += image->GetR(x, y))) + (abs(filter.b += image->GetR(x, y))) < 60)
				{   
				    filter.r = 255;
				    filter.g = 255;
				    filter.b = 255;
				}
				else
				{
				    filter.r = 0;
				    filter.g = 0;
				    filter.b = 0;
				}
				break;
			case 15:
				for (int j = y-1; j < y+2; j++)
				{
					for (int i = x-1; i < x+2; i++)
					{
						verificador++;
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							if (verificador < 4)
							{
								filter.r += image->GetR(i, j)*-1;
								filter.g += image->GetG(i, j)*-1;
								filter.b += image->GetB(i, j)*-1;
							}
							if (verificador > 6)
							{
								filter.r += image->GetR(i, j);
								filter.g += image->GetG(i, j);
								filter.b += image->GetB(i, j);
							}
						}
						
					}
				}
				if((abs(filter.r += image->GetR(x, y))) + (abs(filter.g += image->GetR(x, y))) + (abs(filter.b += image->GetR(x, y))) < 60)
				{   
				    filter.r = 255;
				    filter.g = 255;
				    filter.b = 255;
				}
				else
				{
				    filter.r = 0;
				    filter.g = 0;
				    filter.b = 0;
				}
				break;
			case 16:
				for (int i = x-1; i < x+2; i++)
				{
					for (int j = y-1; j < y+2; j++)
					{
						verificador++;
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							if (verificador < 4)
							{
								filter.r += image->GetR(i, j)*-1;
								filter.g += image->GetG(i, j)*-1;
								filter.b += image->GetB(i, j)*-1;
							}
							if (verificador > 6)
							{
								filter.r += image->GetR(i, j);
								filter.g += image->GetG(i, j);
								filter.b += image->GetB(i, j);
							}
						}
						
					}
				}
				if((abs(filter.r += image->GetR(x, y))) + (abs(filter.g += image->GetR(x, y))) + (abs(filter.b += image->GetR(x, y))) < 60)
				{   
				    filter.r = 255;
				    filter.g = 255;
				    filter.b = 255;
				}
				else
				{
				    filter.r = 0;
				    filter.g = 0;
				    filter.b = 0;
				}
				break;
			case 17:
				for (int i = x-2; i < x+3; i++)
				{
					for (int j = y-2; j < y+3; j++)
					{
						verificador++;
						if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
						{
							if (verificador == 2 || verificador == 4 || verificador == 6 || verificador == 10 || verificador == 16 || 
								verificador == 20 || verificador == 22 || verificador == 24)
							{
								filter.r += (image->GetR(i, j)*4)/273;
								filter.g += (image->GetG(i, j)*4)/273;
								filter.b += (image->GetB(i, j)*4)/273;
							}
							if (verificador == 1 || verificador == 5 || verificador == 21 || verificador == 25)
							{
								filter.r += image->GetR(i, j)/273;
								filter.g += image->GetG(i, j)/273;
								filter.b += image->GetB(i, j)/273;
							}
							if (verificador == 3 || verificador == 11 || verificador == 15 || verificador == 23)
							{
								filter.r += (image->GetR(i, j)*7)/273;
								filter.g += (image->GetG(i, j)*7)/273;
								filter.b += (image->GetB(i, j)*7)/273;
							}
							if (verificador == 7 || verificador == 9 || verificador == 17 || verificador == 19)
							{
								filter.r += (image->GetR(i, j)*16)/273;
								filter.g += (image->GetG(i, j)*16)/273;
								filter.b += (image->GetB(i, j)*16)/273;
							}
							if (verificador == 8 || verificador == 12 || verificador == 14 || verificador == 18)
							{
								filter.r += (image->GetR(i, j)*26)/273;
								filter.g += (image->GetG(i, j)*26)/273;
								filter.b += (image->GetB(i, j)*26)/273;
							}
							if (verificador == 13)
							{
								filter.r += (image->GetR(i, j)*41)/273;
								filter.g += (image->GetG(i, j)*41)/273;
								filter.b += (image->GetB(i, j)*41)/273;
							}
						}
						
					}
				}
				if((abs(filter.r += image->GetR(x, y))) + (abs(filter.g += image->GetR(x, y))) + (abs(filter.b += image->GetR(x, y))) < 60)
				{   
				    filter.r = 255;
				    filter.g = 255;
				    filter.b = 255;
				}
				else
				{
				    filter.r = 0;
				    filter.g = 0;
				    filter.b = 0;
				}
				break;
			case 18:
                for (int i = x-1; i < x+2; i++)
                {
                    for (int j = y-1; j < y+2; j++)
                    {
                        verificador++;
                        if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
                        {
                            if (verificador == 1 || verificador == 3)
                            {
                                filter.r += image->GetR(i, j)*-1;
                                filter.g += image->GetG(i, j)*-1;
                                filter.b += image->GetB(i, j)*-1;
                            }

                            if (verificador == 4 || verificador == 5 || verificador == 6) 
                            {
                                filter.r += image->GetR(i, j)*0;
                                filter.g += image->GetG(i, j)*0;
                                filter.b += image->GetB(i, j)*0;
                            }

                            if (verificador == 7 || verificador == 9)
                            {
                                filter.r += image->GetR(i, j);
                                filter.g += image->GetG(i, j);
                                filter.b += image->GetB(i, j);
                            }
                            if (verificador == 2)
                            {
                                filter.r += image->GetR(i, j)*-2;
                                filter.g += image->GetG(i, j)*-2;
                                filter.b += image->GetB(i, j)*-2;
                            }
                            if (verificador == 8)
                            {
                                filter.r += image->GetR(i, j)*2;
                                filter.g += image->GetG(i, j)*2;
                                filter.b += image->GetB(i, j)*2;
                            }

                            if (verificador == 1 || verificador == 7)
                            {
                                filter2.r += image->GetR(i, j)*-1;
                                filter2.g += image->GetG(i, j)*-1;
                                filter2.b += image->GetB(i, j)*-1;
                            }

                            if (verificador == 3 || verificador == 9)
                            {
                                filter2.r += image->GetR(i, j)*-2;
                                filter2.g += image->GetG(i, j)*-2;
                                filter2.b += image->GetB(i, j)*-2;
                            }
                            if (verificador == 4)
                            {
                                filter2.r += image->GetR(i, j)*-2;
                                filter2.g += image->GetG(i, j)*-2;
                                filter2.b += image->GetB(i, j)*-2;
                            }

                            if (verificador == 6)
                            {
                                filter2.r += image->GetR(i, j)*2;
                                filter2.g += image->GetG(i, j)*2;
                                filter2.b += image->GetB(i, j)*2;
                            }

                            
                        }
                        
                    }
                }

                if((abs(filter.r += image->GetR(x, y)) + abs(filter2.r += image->GetR(x, y))) + 
                   (abs(filter.g += image->GetG(x, y)) + abs(filter2.g += image->GetG(x, y))) +
                   (abs(filter.b += image->GetB(x, y)) + abs(filter2.b += image->GetB(x, y))) < 60){   

                        filter.r = 255;
                        filter.g = 255;
                        filter.b = 255;

                    }else{
                        filter.r = 0;
                        filter.g = 0;
                        filter.b = 0;

                    }

                break;
            case 19:
                for (int j = y-1; j < y+2; j++)
                {
                    for (int i = x-1; i < x+2; i++)
                    {
                        verificador++;
                        if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
                        {
                            if (verificador < 4)
                            {
                                filter.r += image->GetR(i, j)*-1;
                                filter.g += image->GetG(i, j)*-1;
                                filter.b += image->GetB(i, j)*-1;
                            }
                            if (verificador > 6)
                            {
                                filter.r += image->GetR(i, j);
                                filter.g += image->GetG(i, j);
                                filter.b += image->GetB(i, j);
                            }

                            if ((verificador == 1) || (verificador == 4) || (verificador == 7)) 
                            {
                                filter.r += image->GetR(i, j)*-1;
                                filter.g += image->GetG(i, j)*-1;
                                filter.b += image->GetB(i, j)*-1;
                            }
                            if ((verificador == 3) || (verificador == 6) || (verificador == 9))
                            {
                                filter.r += image->GetR(i, j);
                                filter.g += image->GetG(i, j);
                                filter.b += image->GetB(i, j);
                            }



                        }

                        if((abs(filter.r += image->GetR(x, y)) + abs(filter2.r += image->GetR(x, y))) + 
                           (abs(filter.g += image->GetG(x, y)) + abs(filter2.g += image->GetG(x, y))) +
                           (abs(filter.b += image->GetB(x, y)) + abs(filter2.b += image->GetB(x, y))) < 60){   

                                filter.r = 255;
                                filter.g = 255;
                                filter.b = 255;
                        }else{
                            filter.r = 0;
                            filter.g = 0;
                            filter.b = 0;

                        }
                        
                    }
                }
                break;
		}
	return (filter);
}

void incializar (void) {
	init ();

	// Create two windows
	window1 = glutCreateWindow("Original Image");

  	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	//Janela da imagem
	glutSetWindow (window1);
	glutDisplayFunc(displayImage1);
	glutReshapeWindow(img1->GetWidth(), img1->GetHeight());
	glutPositionWindow(100, 100);
}