/*
  Name:        test03.Creating.Image.Animation.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        04/07/2009
  Last Update: 04/09/2016
  Description: Create an image data from code and make a simple animation
               modifying the file.
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdlib.h>

// Image Object
PixelLab *img = NULL;

int k = 0;
int dir = 1;

void idle()
{
   if(img)
   {
      for(int y = 0; y < img->GetHeight(); y++)
         for(int x = 0; x < img->GetWidth(); x++)
         {
            if(y < k && x < k)
               img->SetRGB(x, y, 255, 255, 0);
            else
               img->SetRGB(x, y, 0, 0, 0);
         }
      if(k>img->GetHeight()) dir = -1;
      if(k < 0)              dir = +1;
      k+=dir;
   }
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

   int w = 400;
   int h = 400;

   img = new PixelLab();
   img->CreateImage(w, h);

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("Image Animation Test");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);

   glutMainLoop();

   return 1;
}
