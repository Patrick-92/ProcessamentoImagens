/*
  Name:        test09.Saving.Modified.Image.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Date:        04/07/2009
  Last Update: 01/08/2016
  Description: Load and Save a modified image.
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdio.h>
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

void modifyImage()
{
   // Modify the image making it a little darker
   int modValue = 100;
   if(img)
      for(int y = 0; y < img->GetHeight(); y++)
         for(int x = 0; x < img->GetWidth(); x++)
            img->SetRGB(x, y, (img->GetR(x,y) < modValue) ? 0 : img->GetR(x,y) - modValue,
                              (img->GetG(x,y) < modValue) ? 0 : img->GetG(x,y) - modValue,
                              (img->GetB(x,y) < modValue) ? 0 : img->GetB(x,y) - modValue);

}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   char input[]  = {"../figs/lena.png"};
   char output[] = {"../figs/output.png"};

   img = new PixelLab();

   img->Read(input);

   // Modifica os canais RGB da imagem
   modifyImage();

   img->Save(output);

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("Convert Data to Pixel");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);

   glutMainLoop();

   return 1;
}
