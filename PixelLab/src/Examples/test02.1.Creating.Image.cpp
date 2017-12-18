/*
  Name:        test02.Creating.Image.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        04/07/2009
  Last Update: 08/09/2016
  Description: Create an image data from code.
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

   int w = 400;
   int h = 300;

   img = new PixelLab();
   img->CreateImage(w, h);

   // Creates a four colour pattern
	for(int y = 0; y < h; y++)
      for(int x = 0; x < w; x++)
         if(y < h/2)
            if(x < w/2) img->SetRGB(x, y, 0, 255, 0);
            else        img->SetRGB(x, y, 255, 0, 0);
         else
            if(x < w/2) img->SetRGB(x, y, 255, 255, 0);
            else        img->SetRGB(x, y, 0, 0, 255);

   img->Save("../figs/output.png");

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("PixelLab Test - Creating a sintatic image");

   glutDisplayFunc(display);
   glutKeyboardFunc(key);
   glutMainLoop();

   return 1;
}
