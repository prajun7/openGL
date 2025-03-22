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
  float tail_length = 20.0f;
  float tail_width = 20.0f;
  glBegin(GL_LINE_LOOP);
    glVertex3f(75.0f, 0.0f, 0.0f);               // Tip attached to body (right edge after scaling)
    glVertex3f(75.0f + tail_length, -tail_width / 2, 0.0f); // Bottom of tail base
    glVertex3f(75.0f + tail_length, tail_width / 2, 0.0f);  // Top of tail base
  glEnd();

  // Define tail geometry using variables
  float tip_x = 75.0f;             // x-coordinate of the tip
  float tip_y = 0.0f;              // y-coordinate of the tip
  float base_x = tip_x + tail_length; // x-coordinate of the base
  float base_bottom_y = -tail_width / 2; // y-coordinate of bottom base
  float base_top_y = tail_width / 2;     // y-coordinate of top base

  // Define tail vertices
  float A[3] = {tip_x, tip_y, 0.0f};          // Tip
  float B[3] = {base_x, base_bottom_y, 0.0f}; // Bottom base (yellow)
  float C[3] = {base_x, base_top_y, 0.0f};    // Top base (deep blue)

  // Define colors
  float color_A[3] = {1.0f, 1.0f, 0.0f}; // Yellow for tip
  float color_B[3] = {1.0f, 1.0f, 0.0f}; // Yellow for bottom
  float color_C[3] = {0.0f, 0.0f, 0.5f}; // Deep blue for top

  // Bounding box
  float min_x = tip_x;         // Min x is the tip
  float max_x = base_x;        // Max x is the base
  float min_y = base_bottom_y; // Min y is the bottom base
  float max_y = base_top_y;    // Max y is the top base

  // Step size for pixel drawing
  float step = 0.5f;

  glBegin(GL_POINTS);
    for (float x = min_x; x <= max_x; x += step) {
      for (float y = min_y; y <= max_y; y += step) {
        // Compute barycentric coordinates
        float denom = (B[1] - C[1]) * (A[0] - C[0]) + (C[0] - B[0]) * (A[1] - C[1]);
        float alpha = ((B[1] - C[1]) * (x - C[0]) + (C[0] - B[0]) * (y - C[1])) / denom;
        float beta = ((C[1] - A[1]) * (x - C[0]) + (A[0] - C[0]) * (y - C[1])) / denom;
        float gamma = 1.0f - alpha - beta;

        // Check if point is inside the triangle
        if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1) {
            // Interpolate color
            float R = alpha * color_A[0] + beta * color_B[0] + gamma * color_C[0];
            float G = alpha * color_A[1] + beta * color_B[1] + gamma * color_C[1];
            float B = alpha * color_A[2] + beta * color_B[2] + gamma * color_C[2];
            
            glColor3f(R, G, B);
            glVertex3f(x, y, 0.0f);
        }
      }
    }
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
