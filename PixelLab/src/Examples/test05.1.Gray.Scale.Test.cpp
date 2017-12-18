/*
  Name:        test05.Gray.Scale.Test.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        04/07/2009
  Last Update: 07/09/2016
  Description: Load an image from file and convert it to grayscale.
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdlib.h>

// Image Object
PixelLab *img = NULL;

void idle()
{
   glutPostRedisplay();
}

static void display(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   img->ViewImage();
   glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27 :
      case 'q':
         exit(0);
      break;
   }
   glutPostRedisplay();
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   img = new PixelLab();

   img->Read("../figs/lena.png");

   // Convert stored bitmap to grayscale
   img->ConvertToGrayScale();

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("GrayScale Test I");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);

   glutMainLoop();

   return 1;
}
