#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"

// Display list idx to draw a diamond.
GLuint diamondList;


void createDiamondDisplayList() {
  diamondList = glGenLists(1);  
    glNewList(diamondList, GL_COMPILE);

    // For UAH's blue color with the hex code #0077C8:
    // Hex Breakdown:
    // Red: 00 (0 in decimal)
    // Green: 77 (119 in decimal)
    // Blue: C8 (200 in decimal)
    // Convert to normalized RGB (divide by 255):

    // Red: 0 / 255 = 0.0
    // Green: 119 / 255 ≈ 0.467
    // Blue: 200 / 255 ≈ 0.784
    glColor3f(0.0f, 0.467f, 0.784f);

    glPushMatrix();
      // Scale the octahedron so that each edge is about 25 units long.
      glScalef(25.0f, 25.0f, 25.0f);
      glutWireOctahedron();
    glPopMatrix();
  glEndList();
}

// This function calls the display list to draw the diamond
void drawDiamond() {
  glCallList(diamondList);
}


/**
* The display handler sets up the display and renders the initial scene with a diamond and a landing zone.
* The background is cleared to yellow, and the objects are rendered in white by default.
*/
void displayHandler()
{
  // Clear the color and depth buffers
  glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(400.0f, 575.0f, -50.0f);

  // Draw the diamond after setting up the transformation.
  drawDiamond();

  glFlush(); 
}

#define canvas_Width 800
#define canvas_Height 600

char canvas_Name[] = "Diamond Drop";

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  glutDisplayFunc(displayHandler);

  createDiamondDisplayList();


  glutMainLoop(); 
  return 0;
}