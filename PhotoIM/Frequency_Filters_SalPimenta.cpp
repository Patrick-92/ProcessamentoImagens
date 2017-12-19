#include "../PixelLab/src/PixelLab/pixelLab.h"
#ifdef WIN32
//#include <windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <iostream>
#include <GL/glui.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct _COMPLEX {
	double	real;
	double	imag;
} COMPLEX, *pCOMPLEX;

int FFT(int dir, int m, double *x, double *y);
int FFT2D(COMPLEX **c, int x, int y, int dir);

int altura = 0;
int largura = 0;
int raio = 0;
int ** mascara;
int mouseX = 0;
int mouseY = 0;
int mousebotao = -1;

int ruido = 0 ;

COMPLEX **principal = NULL;


PixelLab *img = NULL;
PixelLab *imgEspectro = NULL;
PixelLab *imgEspectroInv  = NULL;

#define CMD_BTN_1 100
#define CMD_BTN_2 101
#define CMD_BTN_3 102
#define CMD_BTN_4 102

using namespace std;

string textFile, textRadius, textImport;

int main_window;
int enable_textbox = 0;
int frequencyX = 0;
int frequencyY = 0;
int flagFilter = 0;
float porcentagem = 0;

GLUI_TextBox     *moo;
GLUI_EditText    *edittext, *edittext2, *edittext3;
GLUI_Panel       *obj_panel;
GLUI_Button	*bImportar, *bAplicarFiltroFrequencia, *bSair, *bFile, *bMedian;

void displayOriginal(void);
void Modify_ideal_high_pass_mask(void);
void Modify_ideal_low_pass_mask (void);

int janela1, janela2, janela3;
void do_Fourier();
void acertaImagem(PixelLab *img);
void FourierInverso(COMPLEX **c, PixelLab *imgDest);
void pointer_cb (GLUI_Control* );
void myGlutIdle(void);
void modificaMascara();

void redisplay() {
    glutSetWindow(janela1);
    glutPostRedisplay();
    glutSetWindow(janela2);
    glutPostRedisplay();
    glutSetWindow(janela3);
    glutPostRedisplay();
}

void idle() {
   glutPostRedisplay();
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );

}

void salEPimenta(float porcentagem, PixelLab *imgDest) {
    srand (time(NULL));
    float aux;
    for (int i = 0; i < imgDest->GetHeight(); i++) {
        for (int j = 0; j < imgDest->GetWidth(); j++) {
            aux = rand() % 100;
            if (aux<(porcentagem/2))
                imgDest->SetGrayValue(j,i,255);
            else
                if (aux<porcentagem)
                    imgDest->SetGrayValue(j,i,0);
        }
    }

    redisplay();

}

void filtroMediana(PixelLab *imgDest) {
    int vetor[9];
    int count;
    int mediana;

    for (int i = 0; i < imgDest->GetWidth(); i++) {
        for (int j = 0; j < imgDest->GetHeight(); j++) {

            count = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    if ((i+k < imgDest->GetWidth()) && (j+l < imgDest->GetHeight()) && (i+k >=0 ) && (j+l>= 0)) {
                        vetor[count] = imgDest->GetGrayValue(i+k,j+l);
                        count++;
                    }
                }
            }

            qsort(vetor,count,sizeof(int),compare);
            mediana = vetor[(int)(count/2)];
            imgDest->SetGrayValue(i,j,mediana);
        }
    }

    redisplay();

}

void mouse(int botao, int estado, int x, int y) {
    if (estado == GLUT_UP) {
        mousebotao = -1;
    }
    if (estado == GLUT_DOWN) {
        mousebotao = botao;
        mouseX = x;
        mouseY = y;
        modificaMascara();
        do_Fourier();       
    }
}

static void key(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'm':     filtroMediana(img);
                    break;

      case 27 :
      case 'q':
         exit(0);
      break;
   }
   glutPostRedisplay();
}


void modificaMascara() {

    int x = 0;
    int y = 0;
    for (int i = mouseX - raio ; i < mouseX + raio; i++) {
        for (int j = mouseY - raio ; j < mouseY + raio; j++) {
            if(i>=0 && j>=0 && i< largura && j<altura) {
                x = i - mouseX;
                y = j - mouseY;
                if (sqrt(pow(x,2) + pow(y,2)) <= raio) {
                    mascara[i][j] = 1;
                    //mascara[largura-i][altura-j] =1;
                }
            }

        }
    }
}

void Modify_ideal_high_pass_mask () {
	int x = 0;
    int y = 0;
    int tempX = imgEspectro->GetWidth()/2;
    int tempY = imgEspectro->GetHeight()/2;

    for (int i = tempX - raio ; i < tempX + raio; i++) {
        for (int j = tempY - raio ; j < tempY + raio; j++) {
            if(i>=0 && j>=0 && i< largura && j<altura) {
                x = i - tempX;
                y = j - tempY;
                if ( sqrt(pow(x,2) + pow(y,2)) <= raio) {
                    mascara[i][j] =1;
                    mascara[largura-i][altura-j] =1;
                }
            }

        }
    }
}

void Modify_ideal_low_pass_mask () {
	int x = 0;
    int y = 0;
    int tempX = imgEspectro->GetWidth()/2;
    int tempY = imgEspectro->GetHeight()/2;

    for (int i = 1 ; i < imgEspectro->GetWidth(); i++) {
        for (int j = 1 ; j < imgEspectro->GetHeight(); j++) {
            if(i>=0 && j>=0 && i< largura && j<altura) {
                x = i - tempX;
                y = j - tempY;
                if ( sqrt(pow(x,2) + pow(y,2)) > raio) {
                    mascara[i][j] =1;
                    mascara[largura-i][altura-j] =1;
                }
            }

        }
    }
}

static void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   img->ViewImage();
   glutSwapBuffers();
}

static void display2(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   imgEspectro->ViewImage();
   glutSwapBuffers();
}

static void display3(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   imgEspectroInv->ViewImage();
   glutSwapBuffers();
}

void FourierInverso(COMPLEX **c, PixelLab *imgDest) {

    double magnitude = 0;

    pixel **pixels;
    pixels = (pixel**)malloc(altura*sizeof(pixel*));
    for (int i = 0; i < altura; i++)
        pixels[i] = (pixel*)malloc(largura*sizeof(pixel));

    double **pixelsAux;
    pixelsAux = (double**)malloc(altura*sizeof(double*));
    for (int i = 0; i < altura; i++)
        pixelsAux[i] = (double*)malloc(largura*sizeof(double));

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if(mascara[i][j] == 1) {
                c[j][altura-i].real = 0;
                c[j][altura-i].imag = 0;
            }
        }
    }

    FFT2D(c, altura, largura, -1);
    for (int i = 0; i < img->GetHeight(); i++)
        for (int j = 0; j < img->GetWidth(); j++) {
            magnitude = c[i][j].real*pow(-1, i+j);
            magnitude = (magnitude>255)?255:magnitude;
            magnitude = (magnitude<0)?0:magnitude;
            pixels[i][j].value = (uByte) magnitude ;
    }


    imgDest->ConvertPixelsToData(pixels);
}

COMPLEX** Fourier(PixelLab *img, PixelLab *imgDest, int dir) {

    //matriz da imagem
    pixel **pixels;
    pixels = (pixel**)malloc(altura*sizeof(pixel*));
    for (int i = 0; i < altura; i++)
        pixels[i] = (pixel*)malloc(largura*sizeof(pixel));
    img->ConvertDataToPixels(pixels);

    //matriz do espectro sem a normaliza��o
    double **pixelsAux;
    pixelsAux = (double**)malloc(altura*sizeof(double*));
    for (int i = 0; i < altura; i++)
        pixelsAux[i] = (double*)malloc(largura*sizeof(double));


    COMPLEX **complexo;
    complexo = (COMPLEX**)malloc(altura*sizeof(COMPLEX*));
    for (int i = 0; i < altura; i++)
        complexo[i] = (COMPLEX*)malloc(largura*sizeof(COMPLEX));

    for (int i = 0; i < altura; i++)
        for (int j = 0; j < largura; j++) {
            complexo[i][j].real = pow(-1, i+j)*pixels[i][j].value;
            complexo[i][j].imag = 0.0;
        }

    FFT2D(complexo, img->GetHeight(), img->GetWidth(), dir);

    double magnitude = 0;
    float max = -9999999;
    float min =  9999999;

    for (int i = 0; i < img->GetHeight(); i++)
        for (int j = 0; j < img->GetWidth(); j++) {
            magnitude = sqrt(complexo[i][j].real*complexo[i][j].real + complexo[i][j].imag*complexo[i][j].imag);
            pixelsAux[i][j] = log2(magnitude);
            max = (pixelsAux[i][j] > max)?pixelsAux[i][j]:max;
            min = (pixelsAux[i][j] < min)?pixelsAux[i][j]:min;
    }

    float constante = (255/(max-min));

    for (int i = 0; i < img->GetHeight(); i++)
        for (int j = 0; j < img->GetWidth(); j++) {
            if(mascara[j][altura-i] == 1) {
                pixels[i][j].value = 0;
            } else {
                pixelsAux[i][j] -= min;
                pixelsAux[i][j] *= constante;
                pixelsAux[i][j] = (pixelsAux[i][j]>255)?255:pixelsAux[i][j];
                pixelsAux[i][j] = (pixelsAux[i][j]<0)?0:pixelsAux[i][j];
                pixels[i][j].value = (uByte) pixelsAux[i][j];
            }

        }


    imgDest->ConvertPixelsToData(pixels);
    return complexo;

}

void acertaImagem(){
    int countAltura;
    int countLargura;
    PixelLab *imgAux = NULL;

    for (countLargura=2; countLargura<largura; countLargura*=2);
    for (countAltura=2; countAltura<altura; countAltura*=2);

    imgAux = new PixelLab();
    imgAux->CreateImage(countLargura,countAltura);
    imgAux->ConvertToGrayScale();

    for (int j = 0; j < largura; j++) {
        for (int i = 0; i < altura; i++){
            imgAux->SetGrayValue(j,i,img->GetGrayValue(j,i));
        }
    }
    altura = countAltura;
    largura = countLargura;
    img = imgAux;

}

int Powerof2(int nx, int *m, int *twopm) {
	int pwr;
	*m = 0;
	for (pwr = 1; pwr < nx; pwr = pwr*2) {
		*m = *m + 1;
	}
	*twopm = pwr;
	return(TRUE);
}

int FFT(int dir,int m,double *x,double *y)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1;
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1)
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<nn;i++) {
         x[i] /= (double)nn;
         y[i] /= (double)nn;
      }
   }
   return(TRUE);
}

int FFT2D(COMPLEX **c,int nx,int ny,int dir)
	{
   int i,j;
   int m,twopm;
   double *real,*imag;

   /* Transform the rows */
   real = (double *)malloc(nx * sizeof(double));
   imag = (double *)malloc(nx * sizeof(double));
   if (real == NULL || imag == NULL)
      return(FALSE);
   if (!Powerof2(nx,&m,&twopm) || twopm != nx) {
       return(FALSE);
   }

   for (j=0;j<ny;j++) {
      for (i=0;i<nx;i++) {
         real[i] = c[i][j].real;
         imag[i] = c[i][j].imag;
      }
      FFT(dir,m,real,imag);
      for (i=0;i<nx;i++) {
         c[i][j].real = real[i];
         c[i][j].imag = imag[i];
      }
   }
   free(real);
   free(imag);

   /* Transform the columns */
   real = (double *)malloc(ny * sizeof(double));
   imag = (double *)malloc(ny * sizeof(double));
   if (real == NULL || imag == NULL)
      return(FALSE);
   if (!Powerof2(ny,&m,&twopm) || twopm != ny)
      return(FALSE);
   for (i=0;i<nx;i++) {
      for (j=0;j<ny;j++) {
         real[j] = c[i][j].real;
         imag[j] = c[i][j].imag;
      }
      FFT(dir,m,real,imag);
      for (j=0;j<ny;j++) {
         c[i][j].real = real[j];
         c[i][j].imag = imag[j];
      }
   }
   free(real);
   free(imag);

   return(TRUE);
}

void do_Fourier(){
   principal = Fourier(img,imgEspectro,1);
   FourierInverso(principal,imgEspectroInv);
   redisplay();

}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   /*---------------GLUI-------------------*/

  GLUI *glui = GLUI_Master.create_glui("Filtros na frequencia - Mediana");

  edittext3 = new GLUI_EditText( glui, "Nome do Arquivo:", textImport, CMD_BTN_3, pointer_cb );
    edittext3->set_w( 200 );
  bImportar = new GLUI_Button(glui, "Importar Arquivo", CMD_BTN_3, pointer_cb);

  edittext = new GLUI_EditText( glui, "Porcentagem de Ruido:", textFile, CMD_BTN_1, pointer_cb );
    edittext->set_w( 200 );
  bImportar = new GLUI_Button(glui, "Aplicar Sal e Pimenta", CMD_BTN_1, pointer_cb);

    obj_panel = new GLUI_Panel(glui, "Filtro na Frequencia" );

  edittext2 = new GLUI_EditText( obj_panel, "Raio:", textRadius, CMD_BTN_2, pointer_cb );
    edittext2->set_w( 150 );
  bAplicarFiltroFrequencia = new GLUI_Button(obj_panel, "Aplicar manualmente", CMD_BTN_2, pointer_cb);
    bAplicarFiltroFrequencia->set_w(80);

  bMedian = new GLUI_Button(obj_panel, "Aplicar mediana", CMD_BTN_4, pointer_cb);
    bMedian->set_w(80);

  bSair = new GLUI_Button(glui, "Sair", 0, (GLUI_Update_CB)exit);
    bSair->set_alignment( GLUI_ALIGN_RIGHT );
    bSair->set_w(50);

  GLUI_Master.set_glutIdleFunc(myGlutIdle);

  glutMainLoop();

   return 1;
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
    ruido = atoi(string(edittext->get_text()).c_str());
    salEPimenta(ruido,img);
	}
	if (control->get_id() == CMD_BTN_2) {
		raio = atoi(string(edittext2->get_text()).c_str());
	}
  if (control->get_id() == CMD_BTN_3) {
    displayOriginal();
  }
  if (control->get_id() == CMD_BTN_4) {
    filtroMediana(img);
  }
}

void displayOriginal(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer

	glutDestroyWindow(janela1);
	glutDestroyWindow(janela2);
	glutDestroyWindow(janela3);

  string caminho = "../figs/" + string(edittext3->get_text()) + ".png";

  img = new PixelLab(caminho.c_str());
  img->ConvertToGrayScale();
  altura = img->GetHeight();
  largura = img->GetWidth();

  acertaImagem();

	mascara = (int**)malloc(altura*sizeof(int*));
	for (int i = 0; i < altura; i++)
	mascara[i] = (int*)malloc(largura*sizeof(int));


	glutInitWindowSize(img->GetWidth(),img->GetHeight());
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	janela1 = glutCreateWindow("Original");
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutDisplayFunc(display);

	imgEspectro = new PixelLab();
	imgEspectro->CreateImage(largura,altura);
	imgEspectro->ConvertToGrayScale();
	glutInitWindowSize(largura,altura);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	janela2 = glutCreateWindow("Espectro Original");
	glutPositionWindow(0,25);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutDisplayFunc(display2);
	glutMouseFunc(mouse);

	imgEspectroInv = new PixelLab();
	imgEspectroInv->CreateImage(largura,altura);
	imgEspectroInv->ConvertToGrayScale();
	glutInitWindowSize(largura,altura);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	janela3 = glutCreateWindow("Imagem Filtro");
	glutPositionWindow(0,25);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutDisplayFunc(display3);


	do_Fourier();

	glutSwapBuffers();
}

