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

// The fish turns when its nose is within 4 units of fish tank edge
float tank_edge_buffer = 4.0f;

// Tracks the x-position of the fish's center
float fish_x = 0.0f;

// Tracks the fish's rotation about the y-axis (0° when facing right, 180° when facing left).
float rotation_angle = 0.0f; 

// Enumerates the possible states of the fish's animation.
enum State { MOVING_LEFT, ROTATING_TO_RIGHT, MOVING_RIGHT, ROTATING_TO_LEFT };

// Holds the current state, starting with moving left
State current_state = MOVING_LEFT; 

/**
 * Draws the large fish with a wireframe octahedron body and a triangular tail.
 * The body is 150 units wide, 50 units tall, and 25 units deep, colored in PANTONE True Red (RGB: 1.0, 0.0, 0.0).
 * The tail is 20 units wide, attached to the body at the tip of the triangle, with a length of 10 units.
 * The tail extends to the right of the body.
 */
void drawLargeFish() {
  glColor3f(1.0f, 0.0f, 0.0f); // Hex #FF0000 (pure red) PANTONE True Red

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
  glColor3f(1.0f, 0.647f, 0.0f); // Hex #FFA500 PANTONE Tangelo (orange)

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
  // Set the background color to Pantone 12-4401 TCX Spun Sugar (RGB 180, 220, 234)
  glClearColor(0.706f, 0.863f, 0.918f, 1.0f); // Hex #B4DCEA
  glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with the specified color

  // Large fish
  glPushMatrix();
  glTranslatef(fish_x, 0.0f, -400.0f); 
  glRotatef(rotation_angle, 0.0f, 1.0f, 0.0f); // Rotate about y-axis
  glCallList(largeFishList);
  glPopMatrix();

  // Small fish
  glPushMatrix();
  glTranslatef(-325.0f, -350.0f, -400.0f); // 75 units from left (-400), 50 units above bottom (-400)
  glCallList(smallFishList);
  glPopMatrix();

  glutSwapBuffers(); // Swap buffers for double-buffered animation
}

/**
 * Timer callback to update the diamond's position.
 * It moves the diamond and triggers a redisplay.
 */
void timerFunc(int value) {
  switch (current_state) {
      case MOVING_LEFT:
          // Move fish left by 5 units per frame
          fish_x -= 5.0f;
          // Fish faces right (rotation_angle = 0):
          // - Body spans from fish_x - 75 (nose) to fish_x + 75 (back of body)
          // - Tail tip extends to fish_x + 95
          // Check if nose hits left edge (-400 + 4 = -396 for a small buffer)
          if (fish_x - 75.0f <= -400.0f + tank_edge_buffer) {
              current_state = ROTATING_TO_RIGHT; // Start turning to face right
          }
          break;
      case ROTATING_TO_RIGHT:
          // Rotate clockwise (+5° per frame) from 0° to 180°
          rotation_angle += 5.0f;
          if (rotation_angle >= 180.0f) {
              rotation_angle = 180.0f;           // Clamp to exactly 180°
              current_state = MOVING_RIGHT;      // Start moving right
          }
          break;
      case MOVING_RIGHT:
          // Move fish right by 5 units per frame
          fish_x += 5.0f;
          // Fish faces left (rotation_angle = 180):
          // - Body spans from fish_x + 75 (nose) to fish_x - 75 (back of body)
          // - Tail tip extends to fish_x - 95
          // Check if nose hits right edge (400 - 4 = 396 for a small buffer)
          if (fish_x + 75.0f >= 400.0f - tank_edge_buffer) {
              current_state = ROTATING_TO_LEFT;  // Start turning to face left
          }
          break;
      case ROTATING_TO_LEFT:
          // Rotate counterclockwise (-5° per frame) from 180° to 0°
          rotation_angle -= 5.0f;
          if (rotation_angle <= 0.0f) {
              rotation_angle = 0.0f;             // Clamp to exactly 0°
              current_state = MOVING_LEFT;       // Start moving left again
          }
          break;
  }
  glutPostRedisplay();           
  glutTimerFunc(50, timerFunc, 0); 
}

/**
 * Handles keyboard input to control the animation and movement of the diamond.
 *
 * Controls:
 * - Press 'Q' to quit.
 */
void keyboardCallback(unsigned char key, int x, int y) {
  if (key == 'Q' || key == 'q') {
      exit(0); 
  }
}

char canvas_Name[] = "Aquarium Screen-Saver Simulation";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);

    // Create the display list for the diamond.
    initDisplayLists();

    // Start the animation
    glutTimerFunc(50, timerFunc, 0);

    glutMainLoop();
    return 0;
}
