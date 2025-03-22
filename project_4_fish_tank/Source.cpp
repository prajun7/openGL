/**
 * Graphics Pgm 4 for Prajun Trital
 * 
 * EXTRA CREDIT
 * - Added dust partciles that cross the screen horizontally.
 * - The diamond gets displaced by 4 units due to the wind effect.
 * 
 * ARCHITECTURE
 * 
 * EXTRA CREDIT ARCHITECTURE
*/

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"
#include <cstring>
#include <vector>

#define canvas_Width 800
#define canvas_Height 800

GLuint largeFishList;

GLuint smallFishList; 

/**
 * Draws the large fish with a wireframe octahedron body and a triangular tail.
 * The body is 150 units wide, 50 units tall, and 25 units deep, colored in PANTONE True Red (RGB: 1.0, 0.0, 0.0).
 * The tail is 20 units wide, attached to the body at the tip of the triangle, with a length of 10 units.
 * The tail extends to the right of the body.
 */
void drawLargeFish() {
  glColor3f(1.0f, 0.0f, 0.0f); // PANTONE True Red

  // Draw the body (octahedron)
  glPushMatrix();
  glScalef(75.0f, 25.0f, 12.5f); // Scale to 150 wide, 50 tall, 25 deep
  glutWireOctahedron();
  glPopMatrix();

  // Draw the tail (triangle)
  float tail_length = 10.0f;
  float tail_width = 20.0f;
  glBegin(GL_LINE_LOOP);
  glVertex3f(75.0f, 0.0f, 0.0f);               // Tip attached to body (right edge after scaling)
  glVertex3f(75.0f + tail_length, -tail_width / 2, 0.0f); // Bottom of tail base
  glVertex3f(75.0f + tail_length, tail_width / 2, 0.0f);  // Top of tail base
  glEnd();
}

/**
 * Draws the small fish with a wireframe octahedron body and a triangular tail.
 * The body is 50 units wide, 20 units tall, and 10 units deep, colored in PANTONE Tangelo (RGB: 1.0, 0.5, 0.0).
 * The tail is 7 units wide, attached to the body at the tip of the triangle, with a length of 10 units.
 * The tail extends to the right of the body.
 */
void drawSmallFish() {
  glColor3f(1.0f, 0.5f, 0.0f); // PANTONE Tangelo (orange)

  // Draw the body (octahedron)
  glPushMatrix();
  glScalef(25.0f, 10.0f, 5.0f); // Scale to 50 wide, 20 tall, 10 deep
  glutWireOctahedron();
  glPopMatrix();

  // Draw the tail (triangle)
  float tail_length = 10.0f; // Fixed length, consistent with large fish
  float tail_width = 7.0f;
  glBegin(GL_LINE_LOOP);
  glVertex3f(25.0f, 0.0f, 0.0f);               // Tip attached to body (right edge after scaling)
  glVertex3f(25.0f + tail_length, -tail_width / 2, 0.0f); // Bottom of tail base
  glVertex3f(25.0f + tail_length, tail_width / 2, 0.0f);  // Top of tail base
  glEnd();
}

/**
 * Initializes display lists for the large and small fish.
 * Creates and compiles display lists using drawLargeFish and drawSmallFish functions.
 * Must be called after GLUT initialization and before entering the main loop.
 */
void initDisplayLists() {
  // Create display list for the large fish
  largeFishList = glGenLists(1);
  glNewList(largeFishList, GL_COMPILE);
  drawLargeFish();
  glEndList();

  // Create display list for the small fish
  smallFishList = glGenLists(2);
  glNewList(smallFishList, GL_COMPILE);
  drawSmallFish();
  glEndList();
}

/**
 * The display callback function that renders the scene.
 * Clears the screen to yellow and draws the large and small fish at their initial positions
 * using their respective display lists. The large fish is centered at (0, 0, -400), and
 * the small fish is positioned 75 units from the tank's left and 50 units above the bottom,
 * approximated at (-325, -350, -400) relative to the coordinate system.
 */
void displayCallback() {
  glClearColor(0.961f, 0.922f, 0.866f, 1.0f); // Pantone Spun Sugar background
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the large fish
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -400.0f); // Center of mass at (0, 0, -400)
  glCallList(largeFishList);
  glPopMatrix();

  // Draw the small fish
  glPushMatrix();
  glTranslatef(-325.0f, -350.0f, -400.0f); // 75 units from left (-400), 50 units above bottom (-400)
  glCallList(smallFishList);
  glPopMatrix();

  glutSwapBuffers();
}

/**
 * Timer callback to update the diamond's position.
 * It moves the diamond and triggers a redisplay.
 */
void timerFunc(int value) {
 
}

/**
 * Handles keyboard input to control the animation and movement of the diamond.
 *
 * Controls:
 * - Press 'M' to apply the Moon's gravity.
 * - Press 'I' to apply Io's gravity.
 * - Press 'H' to move the diamond left.
 * - Press 'J' to move the diamond right.
 * - Press 'U' to move the diamond up.
 * - Press 'W' to enable the wind effect.
 * - Press 'D' to disable the wind effect.
 */
void keyboardCallback(unsigned char key, int x, int y) {
 
}

char canvas_Name[] = "Aquarium Screen-Saver Simulation";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);

    // Create the display list for the diamond.
    initDisplayLists();

    glutMainLoop();
    return 0;
}
