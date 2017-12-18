/*
  Name:        test10.Glui.Example.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        26/02/2010
  Last Update: 21/06/2016
  Description: Use a GLUI Interface to change image
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <stdlib.h>
#include "GL/glui.h"
#include "GL/glut.h"

// Image Objects
PixelLab *img    = NULL;
PixelLab *imgMod = NULL;

GLUI_RadioGroup *radio;
int main_window;
int UI_width  = 0;
int option = 0;
int brightness = 0;
int prevBrightness = 0;

char input[512];
char output[512];

void idle()
{
   if ( glutGetWindow() != main_window)
      glutSetWindow(main_window);
   glutPostRedisplay();
}

void computeUIWidth()
{
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = img->GetWidth()-vw;
}

void reshape( int x, int y )
{
  glutPostRedisplay();
}

static void display(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glRasterPos2i(-1, -1); // Fix raster position
   imgMod->ViewImage();
   GLUI_Master.auto_set_viewport();

   glutSwapBuffers();
}

void modifyImage(char channel)
{
   if(img)
   {
      for(int y = 0; y < img->GetHeight(); y++)
      {
         for(int x = 0; x < img->GetWidth(); x++)
         {
            if(channel == 'A') imgMod->SetRGB( x, y, img->GetR(x,y),img->GetG(x,y),img->GetB(x,y));
            if(channel == 'R') imgMod->SetRGB( x, y, img->GetR(x,y),0,0);
            if(channel == 'G') imgMod->SetRGB( x, y, 0,img->GetG(x,y),0);
            if(channel == 'B') imgMod->SetRGB( x, y, 0,0,img->GetB(x,y));
         }
      }
   }
}

void control(int value)
{
   if(value == 1)
   {
      img->Read(input);
      imgMod->Copy(img);
      glutReshapeWindow(imgMod->GetWidth() + UI_width, imgMod->GetHeight());
   }
   if(value == 2)
   {
      imgMod->Save(output);
   }

   if(value == 3) // Radio Button
   {
      if(option == 0) modifyImage('A');
      if(option == 1)
      {
         modifyImage('A');
         imgMod->ConvertToGrayScale();
      }
      if(option == 2) modifyImage('R');
      if(option == 3) modifyImage('G');
      if(option == 4) modifyImage('B');
   }
   if(value == 4)
   {
      if( brightness > prevBrightness)
         imgMod->AddValueToChannels(1);
      else
         imgMod->AddValueToChannels(-1);
      prevBrightness = brightness;
   }
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
        break;
        case 'm':
            imgMod->Copy(img);
            modifyImage('R');
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   strcpy(input, "../figs/woman.png");
   strcpy(output, "../figs/output.png");

   img    = new PixelLab(input);
   imgMod = new PixelLab();
   imgMod->Copy(img);

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitWindowPosition(100,100);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   main_window = glutCreateWindow("GLUI EXAMPLE");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);

   // GLUI
   // Use the first line above to get the interface in the same window as the graphics
   // Use the second line (comment the first) to get the interface in a separeted window
   GLUI *glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );
   //GLUI *glui = GLUI_Master.create_glui( "" );

   GLUI_Master.set_glutReshapeFunc(reshape);

   GLUI_Panel *io_panel = glui->add_panel( (char *) "Input/Output" );
   GLUI_EditText *edit1, *edit2;
   edit1 = glui->add_edittext_to_panel (io_panel, (char *) "", GLUI_EDITTEXT_TEXT, input);
   edit2 = glui->add_edittext_to_panel (io_panel, (char *) "", GLUI_EDITTEXT_TEXT, output);
   edit1->set_w(185);
   edit2->set_w(185);
   glui->add_column_to_panel( io_panel, false);
   GLUI_Button *b1 = glui->add_button_to_panel( io_panel, (char*) "Load", 1, control );
   GLUI_Button *b2 = glui->add_button_to_panel( io_panel, (char*) "Save", 2, control );
   b1->set_w(50);
   b2->set_w(50);

   GLUI_Panel *channel_panel = glui->add_panel((char *) "Channels" );
   glui->add_statictext_to_panel( channel_panel, (char *) "Warning: Channels change will reset image");
   radio = glui->add_radiogroup_to_panel(channel_panel, &option, 3, control);
   glui->add_radiobutton_to_group( radio, (char *) "Original" );
   glui->add_radiobutton_to_group( radio, (char *) "Grayscale" );
   glui->add_radiobutton_to_group( radio, (char *) "Red" );
   glui->add_radiobutton_to_group( radio, (char *) "Green" );
   glui->add_radiobutton_to_group( radio, (char *) "Blue" );
   glui->add_separator_to_panel( channel_panel );
   GLUI_Spinner* spinner;
   spinner = glui->add_spinner_to_panel( channel_panel, (char*)"Increase/Decrease brightness", GLUI_SPINNER_INT, &brightness, 4, control);

   glui->add_button((char *) "Quit", 0,(GLUI_Update_CB)exit );

   glui->set_main_gfx_window( main_window );
   GLUI_Master.set_glutIdleFunc( idle );
   computeUIWidth(); // Compute the size of the user interface
   glutReshapeWindow(imgMod->GetWidth() + UI_width, imgMod->GetHeight());

   glutMainLoop();

   return 1;
}
