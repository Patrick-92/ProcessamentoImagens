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

#define CMD_BTN_1 100
#define CMD_BTN_2 101
#define CMD_BTN_3 102
#define CMD_BTN_4 103
#define CMD_BTN_5 104
#define CMD_BTN_6 105
#define CMD_BTN_7 106
#define CMD_BTN_8 107

using namespace std;

int main_window;

GLUI_TextBox     *moo;
GLUI_Panel       *obj_panel, *obj_panel2, *obj_panel3, *obj_panel4, *obj_panel5;
GLUI_Button	*bTomCurve, *bSpaceFilter, *bSpaceFilter2, *bFrequencyFilters, *bNoiseAdittionMedianFilter,
*bImageSegmentation, *bMorphologicalProcessing, *bMorphologicalProcessing2, *bSair;

void pointer_cb (GLUI_Control* );
void myGlutIdle(void);

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   /*---------------GLUI-------------------*/

	GLUI *glui = GLUI_Master.create_glui("Framework");

		obj_panel = new GLUI_Panel(glui, "Curva de Nível" );
    obj_panel2 = new GLUI_Panel(glui, "Filtro no domínio do espaço" );
    obj_panel3 = new GLUI_Panel(glui, "Filtro no domínio da frequência" );
    obj_panel4 = new GLUI_Panel(glui, "Segmentação por threshold automatico e manual" );
    obj_panel5 = new GLUI_Panel(glui, "Filtros morfológicos" );

	bTomCurve = new GLUI_Button(obj_panel, "Iniciar", CMD_BTN_1, pointer_cb);
		bTomCurve->set_w(80);

  bSpaceFilter = new GLUI_Button(obj_panel2, "Operaçoes", CMD_BTN_2, pointer_cb);
    bSpaceFilter->set_w(80);

  bSpaceFilter2 = new GLUI_Button(obj_panel2, "Filtros", CMD_BTN_3, pointer_cb);
    bSpaceFilter2->set_w(80);
    
  bFrequencyFilters = new GLUI_Button(obj_panel3, "Filtros", CMD_BTN_4, pointer_cb);
    bFrequencyFilters->set_w(80);

  bNoiseAdittionMedianFilter = new GLUI_Button(obj_panel3, "Adiçao de ruido e mediana", CMD_BTN_5, pointer_cb);
    bNoiseAdittionMedianFilter->set_w(80);

  bImageSegmentation = new GLUI_Button(obj_panel4, "Iniciar", CMD_BTN_6, pointer_cb);
    bImageSegmentation->set_w(80);

  bMorphologicalProcessing = new GLUI_Button(obj_panel5, "Erosao de dilatacao", CMD_BTN_7, pointer_cb);
    bMorphologicalProcessing->set_w(80);

  bMorphologicalProcessing2 = new GLUI_Button(obj_panel5, "Extrair bordas", CMD_BTN_8, pointer_cb);
    bMorphologicalProcessing2->set_w(80);

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
		system("./tomCurve");
	}
	if (control->get_id() == CMD_BTN_2) {
    system("");
	}
  if (control->get_id() == CMD_BTN_3) {
    system("./spaceFilterII");
  }
  if (control->get_id() == CMD_BTN_4) {
    system("./frequencyFilters");
  }
  if (control->get_id() == CMD_BTN_5) {
    system("./frequencyFiltersSalPimenta");
  }
  if (control->get_id() == CMD_BTN_6) {
    system("./imageSegmentation");
  }
  if (control->get_id() == CMD_BTN_7) {
    system("./morphologicalProcessing");
  }
  if (control->get_id() == CMD_BTN_8) {
    system("./morphologicalProcessing2");
  }
}