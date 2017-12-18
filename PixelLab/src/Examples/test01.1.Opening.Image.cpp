/*
  Name:        test01.Opening.Image.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        04/07/2009
  Last Update: 02/03/2016
  Description: Load an image file.
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdlib.h>

// Image Object
PixelLab *img = NULL;

static void display(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
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

   // Create PixelLab Object
   img = new PixelLab();

   // Reading Image
   // Instantiate image object and load image
   //img->Read("../figs/gears.bmp");
   //img->Read("../figs/gears.png");
   //img->Read("../figs/gearsGray.png");
   //img->Read("../figs/lena.png");
   //img->Read("../figs/lenaGray.png");
   img->Read("../figs/woman.png");

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("PixelLab Testaaa");

   glutDisplayFunc(display);
   glutKeyboardFunc(key);
   glutMainLoop();

   return 1;
}
