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

PixelLab *img1 = NULL, *img2 = NULL, *img3 = NULL;

int windth, height, UI_width = 0;

int histo[256];
int posX = 128; // posiçao x do marcador no histograma
bool passiveMotion = false;
bool flag_manual_threshold = false;
int structuring_matrix_size = 0;

#define CMD_BTN_1 100
#define CMD_BTN_2 101
#define CMD_BTN_3 102

string textFile, textRadius;
int main_window;

GLUI_TextBox     *moo;
GLUI_EditText    *edittext, *edittext2;
GLUI_Panel       *obj_panel;
GLUI_Button	*bImportar, *bSair, *bErosion, *bDilation;

void displayImage1(void);
void displayImage3(void);
void displayImageTomCurve(void);
void mouse(int , int , int , int );
void motion(int , int );
void displayHistogram (void);
void histogram_data (PixelLab *image);
void automatic_threshold (PixelLab *, PixelLab *);
void manual_threshold (PixelLab *, PixelLab *);
int **structuring_element (void);
void morphological_erosion (void);
void morphological_dilation (void);
int erosion (PixelLab *, int , int , int **);
int dilation (PixelLab *, int , int , int **);
void pointer_cb (GLUI_Control* );
void myGlutIdle(void);
void incializar(void);

void init (void) {
	string caminho = "../figs/" + string(edittext->get_text()) + ".png";

	img1 = new PixelLab();
	img1->Read(caminho.c_str());

	img2 = new PixelLab();
	img2->CreateImage(img1->GetWidth(), img1->GetHeight());

	img3 = new PixelLab();
	img3->CreateImage(img1->GetWidth(), img1->GetHeight());
}

void reshape( int x, int y ) {
  glutPostRedisplay();
}

void computeUIWidth() {
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = vw;
}

static void key(unsigned char key, int x, int y)
{
   switch (key)
   {
      case '1':
      			posX --;
      			break;
      case '2':
      			posX ++;
      			break;
      case 'a':
      			automatic_threshold(img1,img2);
      			flag_manual_threshold = false;
      			break;
      case 'm':
      			flag_manual_threshold = true;
      			break;
      case 'e':
      			morphological_erosion();
      			break;
      case 'd':
      			morphological_dilation();
      			break;
      case 'q':
         exit(0);
      break;
   }
   glutPostRedisplay();
}

// Main Function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	/*---------------GLUI-------------------*/

	GLUI *glui = GLUI_Master.create_glui("Filtros na frequencia");

	edittext = new GLUI_EditText( glui, "Nome do Arquivo:", textFile, CMD_BTN_1, pointer_cb );
		edittext->set_w( 200 );
	bImportar = new GLUI_Button(glui, "Importar Arquivo", CMD_BTN_1, pointer_cb);

	bSair = new GLUI_Button(glui, "Sair", 0, (GLUI_Update_CB)exit);
		bSair->set_alignment( GLUI_ALIGN_RIGHT );
		bSair->set_w(50);

	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return 0;
}

void displayImage1(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	glutSwapBuffers();
}

void displayHistogram (void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Clean z-buffer
	glClearColor(1.0, 1.0, 1.0, 0.0);

	//Desenhando o marcador no histograma
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(posX, 0.0);
	glVertex2f(posX, 4000.0);

	//Desenhando o histograma
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= 255; i++)
	{
		glPopMatrix();
		glBegin(GL_LINES);
		glVertex2f(i, 0.0);
		glVertex2f(i, histo[i]);
		glEnd();
		glPopMatrix();
	}

	if (flag_manual_threshold == true)
	{
		manual_threshold(img1, img2);
	}

	glutSwapBuffers();
  	glutPostRedisplay();	
}

void displayImage3(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img3->ViewImage();

	glutSwapBuffers();
}

// Motion callback
void motion(int x, int y)
{
	// Inverte mouse para que origem fique no canto inferior esquerdo da janela
	// (por default, a origem � no canto superior esquerdo)
	y = 255 - y;

	posX = x;
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
		}
		else
			printf("\nBotao esquerdo solto na posicao [%d, %d].", x, y);  // GLUT_UP
	}
	else
		printf("\nOutro botao do mouse pressionado.");
}

void histogram_data (PixelLab *image){
	for (int x = 0; x < image->GetHeight(); x++)
	{
		for (int y = 0; y < image->GetWidth(); y++)
		{
			histo[(int) image->GetGrayValue(x,y)] ++;			
		}
	}
}

void manual_threshold (PixelLab *original_image, PixelLab *edited_image) {
	for (int x = 0; x < original_image->GetHeight(); x++)
	{
		for (int y = 0; y < original_image->GetWidth(); y++)
		{
			if ((int) original_image->GetGrayValue(x,y) <= posX)
			{
				edited_image->SetGrayValue(x, y, (uByte) 0);
			}
			else
			{
				edited_image->SetGrayValue(x, y, (uByte) 255);
			}
		}
	}	
	glutPostWindowRedisplay(window3);
}

void automatic_threshold (PixelLab *original_image, PixelLab *edited_image) {
	float mi_1 = 0.0, mi_2 = 0.0, temp_T = 0.0, denominador1 = 0.0, denominador2 = 0.0, 
	medium_tone = 0.0, altura = (float) original_image->GetHeight(), largura = (float) original_image->GetWidth();
	float diference_T = 10;
	bool flag = false; 

	for (int i = 0; i < 255; ++i)
	{
		cout << histo[i] << endl;
		medium_tone += (float) (histo[i] * i);
	}

	medium_tone = medium_tone/ (altura * largura);

	while(diference_T < (medium_tone - temp_T))
	{
		if (flag == true)
		{
			medium_tone = temp_T;
		}
		for (int i = 0; i <= 255; i++)
	{
		if (histo[i] <= medium_tone)
		{
			mi_1 = mi_1 + i;
			denominador1++;
		}
		else
		{
			mi_2 = mi_2 + i;
			denominador2++;
		}
	}
		cout << mi_1 << " " << mi_2 << endl;
		temp_T = ((mi_1/denominador1) + (mi_2/denominador2)) / 2;
		mi_1 = 0.0;
		mi_2 = 0.0;
		denominador1 = 0.0;
		denominador2 = 0.0;
		flag = true;
	}

	medium_tone = temp_T;

	cout << medium_tone << endl;

	for (int x = 0; x < original_image->GetHeight(); x++)
	{
		for (int y = 0; y < original_image->GetWidth(); y++)
		{
			if ((int) original_image->GetGrayValue(x,y) <= medium_tone)
			{
				edited_image->SetGrayValue(x, y, (uByte) 0);
			}
			else
			{
				edited_image->SetGrayValue(x, y, (uByte) 255);
			}
		}
	}	
	glutPostWindowRedisplay(window3);	
}

int** structuring_element (void) {
	int x = 0, valor = 0;
	bool nImparFlag = false, valor1_ou_0 = false;
	
	while (nImparFlag == false) {
		cout << "Digite o tamanho do elemento estruturante (somente valores impares):" << endl;
		cin >> x;

		if ( (x%2) != 0)
		{
			nImparFlag = true;
		}
	}

	int **structuring_element = (int**)malloc(x * sizeof(int*)); //Aloca um Vetor de Ponteiros
	structuring_matrix_size = x;
 
  	for (int i = 0; i < x; i++){ //Percorre as linhas do Vetor de Ponteiros
       structuring_element[i] = (int*) malloc(x * sizeof(int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
       for (int j = 0; j < x; j++){ //Percorre o Vetor de Inteiros atual.
            structuring_element[i][j] = 0; //Inicializa com 0.
       }
    }

	cout << "Monte os valores da matrix do elemento estruturante" << endl;

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < x; j++)
		{
			while (valor1_ou_0 == false) {
				cout << "Valor [" << i << "] [" << j << "]:" << endl;
				cin >> valor;
				if (valor == 1 || valor == 0)
				{
					valor1_ou_0 = true;
				}
				else
					cout << "Valor invalido! Digite Novamente o valor!" << endl;
			}
			structuring_element[i][j] = valor;
			valor1_ou_0 = false;
		}
	}

	return structuring_element;

}

void morphological_dilation (void) {
	int **structuringElement = structuring_element();
	int valorCinza, contador = 0;

	automatic_threshold(img1,img2);

	for (int x = 0; x < img1->GetHeight(); x++)
	{
		for (int y = 0; y < img1->GetWidth(); y++)
		{
			valorCinza = dilation(img2, x, y, structuringElement);
			img3->SetGrayValue(x, y, (uByte) valorCinza);
			contador++;
		}
	}
}

void morphological_erosion (void) {
	int **structuringElement = structuring_element();
	int valorCinza, contador = 0;

	automatic_threshold(img1,img2);

	for (int x = 0; x < img1->GetHeight(); x++)
	{
		for (int y = 0; y < img1->GetWidth(); y++)
		{
			valorCinza = erosion(img2, x, y, structuringElement);
			img3->SetGrayValue(x, y, (uByte) valorCinza);
			contador++;
		}
	}
}

int dilation (PixelLab *image, int x, int y, int **structuringElement){
	int variation = structuring_matrix_size/2;
	int k = 0, l = 0;

	for (int i = x-variation; i <= x+variation; i++)
	{
		l = 0;
		for (int j = y-variation; j <= y+variation; j++)
		{
			if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
			{
				if ((int) image->GetGrayValue(i,j) != (structuringElement[k][l]*255))
				{
					return 0;
				}
			}
			l++;
		}
		k++;
	}
	return 255;
}

int erosion (PixelLab *image, int x, int y, int **structuringElement){
	int variation = structuring_matrix_size/2;
	int k = 0, l = 0;

	for (int i = x-variation; i <= x+variation; i++)
	{
		l = 0;
		for (int j = y-variation; j <= y+variation; j++)
		{
			if (i > 0 && j > 0 && i < image->GetHeight() && j < image->GetWidth())
			{
				if ((int) image->GetGrayValue(i,j) == (structuringElement[k][l]*255))
				{
					return 255;
				}
			}
			l++;
		}
		k++;
	}
	return 0;
}

void incializar (void) {
	init ();

	histogram_data(img1);

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

  	//Janela da Editada
	glutSetWindow (window3); // Change current window to 2
	glutDisplayFunc(displayImage3);
	glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
	glutPositionWindow(800, 100);
	glutKeyboardFunc(key);

	GLUI *glui = GLUI_Master.create_glui("Filtros na frequencia");

	obj_panel = new GLUI_Panel(glui, "Tipo de filtro" );

	bErosion = new GLUI_Button(obj_panel, "Erosao", CMD_BTN_2, pointer_cb);
	bDilation = new GLUI_Button(obj_panel, "Dilatacao", CMD_BTN_3, pointer_cb);

	GLUI_Master.set_glutIdleFunc(myGlutIdle);

}

void myGlutIdle( void ) {
	if (glutGetWindow() != main_window)
	{
		glutSetWindow(main_window);
		glutPostRedisplay();
	}
}

void pointer_cb (GLUI_Control* control){
	if (control->get_id() == CMD_BTN_1) {
		incializar();		
	}
	if (control->get_id() == CMD_BTN_2) {
		morphological_erosion();
	}
	if (control->get_id() == CMD_BTN_3) {
		morphological_dilation();
	}
}