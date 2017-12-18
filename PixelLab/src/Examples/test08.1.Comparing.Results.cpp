/*
  Name:        test08.Comparing.Results.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        07/07/2009
  Last Update: 08/09/2016
  Description: This test presents how to compare the original image
               and a processed one using the keyboard.
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdlib.h>

// Image Objects
PixelLab *img = NULL;
PixelLab *imgOriginal = NULL;
bool showOriginalImage = false;

void idle()
{
   glutPostRedisplay();
}

static void display(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if( showOriginalImage )
      imgOriginal->ViewImage();
   else
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
      case 'o': // Original Image
         showOriginalImage = true;
         glutSetWindowTitle("Original Image");
      break;
      case 'p': // Processed Image
         showOriginalImage = false;
         glutSetWindowTitle("Processed Image");
      break;

   }
   glutPostRedisplay();
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   imgOriginal = new PixelLab("../figs/lena.png");

   img         = new PixelLab();
   img->Copy( imgOriginal );

   // Convert stored bitmap to grayscale
   img->ConvertToGrayScale();

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("Press 'o' to show original image and 'p' to show the processed image");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);

   glutMainLoop();

   return 1;
}
