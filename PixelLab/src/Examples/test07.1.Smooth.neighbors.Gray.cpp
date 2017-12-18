/*
  Name:        test07.Smooth.neighbors.Gray.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        04/07/2009
  Last Update: 02/09/2016
  Description: Load an image from file and convert it to a pixel matrix.
               Manipulate the matrix and perform a simple smooth procedure.

*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdlib.h>

void modifyImage(int);

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

void modifyImage(int iteractions = 1)
{
   uByte v;
   if(img)
   {
      for(int ite = 0; ite < iteractions; ite++)
         for(int y = 1; y < img->GetHeight()-1; y++)
            for(int x = 1; x < img->GetWidth()-1; x++)
            {
               v = (img->GetGrayValue(x-1, y) + img->GetGrayValue(x+1, y) +
                    img->GetGrayValue(x, y-1) + img->GetGrayValue(x, y+1))/4;
               img->SetGrayValue(x, y, v);
            }
   }
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   img = new PixelLab();

   img->Read("../figs/lena.png");

   // Convert input first
   img->ConvertToGrayScale();

   // Simple Smooth
   modifyImage(20);

   // If you want to save the result, uncomment the next line
   //img->Save("../figs/output.png");

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("Smooth test");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);

   glutMainLoop();

   return 1;
}
