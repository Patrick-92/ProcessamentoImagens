/*
  Name:        test02.3.Creating.Gray.Image.Using.Matrix
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        01/03/2010
  Last Update: 01/09/2016
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
   int h = 256;

   img = new PixelLab();
   img->CreateImage(w, h, 1); // Just one channel

   // Create matrix pointer and use PixelLab's auxiliary function to allocate
   pixel **mat;
   img->AllocatePixelMatrix(&mat, 256, 400);

   // Creates a gradient
   for(int y = 0; y < h; y++)
      for(int x = 0; x < w; x++)
         mat[y][x].value = (y < 255) ? y : 255;

   // Set Data as a Matrix
   img->SetDataAsMatrix( mat );

   // Deallocate pixel Matrix
   img->DeallocatePixelMatrix(&mat, 256, 400);

   //img->Save("../figs/output.png");

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("PixelLab Test - Creating a sintatic image");

   glutDisplayFunc(display);
   glutKeyboardFunc(key);
   glutMainLoop();

   return 1;
}
