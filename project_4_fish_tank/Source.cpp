/**
 * Graphics Pgm 4 for Prajun Trital
 * 
 * EXTRA CREDIT
 * - The small fish emitts an air bubble from its nose once every 1.2 second.
 * - Click on P button to pause the animation.
 * 
 * ARCHITECTURE
 * GLUT event-driven generation of a canvas with an aquarium. Canvas is produced via the 
 * display event handler, which is in `displayCallback`, that calls the series of display lists 
 * to draw large fish using `drawLargeFish` function, draws small fish using `drawSmallFish` function,
 * and draws the cactus using the `drawBox`. The `timeFunc` updates the large fish x-axis based on the
 * direction the fish is currently pointing and uses 50 ms as the time interval to achieve an animation
 * running at approximately 20 Frame per second(FPS). Four states that are MOVING_LEFT, ROTATING_TO_RIGHT, 
 * MOVING_RIGHT, and ROTATING_TO_LEFT are used to determine the rotation and the direction the fish that it
 * needs to be moved based on its current position. The `keyboardCallback` detect the keyboard press to 
 * quit the simulation. Barycentric interpolation is used to calculate a smooth transition in the fish tail stripe, 
 * blending the color between yellow and deep blue, with a step size of 1.0f used for drawing the points 
 * along the stripe.
 * 
 * EXTRA CREDIT ARCHITECTURE
 * The `timerFunc` adds bubbles into the array called `bubbles`, updates the position of it and
 * also removes the bubble. The `displayCallback` loops over the `bubbles` array to draw each
 * bubbles. The `mouseCallback` detects mouse clicks to toggle the animation's paused
 * state by setting the boolean variable `is_paused`, which is checked within the `timerFunc`.
*/

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"
#include <cstring>
#include <vector>

#define canvas_Width 800
#define canvas_Height 800

// Display list index to draw the large fish.
GLuint largeFishList;

// Display list index to draw the small fish.
GLuint smallFishList; 

// Display list index to draw the pause button.
GLuint pauseButtonList; 

// Display list index to draw the cactus.
GLuint cactusList;

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

// Tracks whether animation is paused
bool is_paused = false;

// Button dimensions and position (in tank coordinates)
const float button_width = 30.0f;
const float button_height = 30.0f;
const float button_x = -390.0f; // 10 units from left edge (-400)
const float button_y = 360.0f;  // Bottom at 360, top at 360 + 30 = 390, 10 units from top (400)

// Bubble structure to hold position
struct Bubble {
  float x, y, z;
};

// Vector to store all active bubbles
std::vector<Bubble> bubbles; 

// Counter to track frames for bubble emission (1.2 s = 24 frames at 20 fps)
int bubble_counter = 0;

// Small fish position constants
const float small_fish_center_x = -325.0f;
const float small_fish_center_y = -350.0f;
const float small_fish_center_z = -400.0f;

const float small_fish_nose_x = small_fish_center_x - 25.0f; // -350.0f

/**
 * Draws the large fish with a wireframe octahedron body and a triangular tail.
 * The body is 150 units wide, 50 units tall, and 25 units deep, 
 * colored in PANTONE True Red (RGB: 1.0, 0.0, 0.0). The tail has a wireframe outline and a vertical stripe 
 * positioned 3/4 of the way along its length from the tip.
 * The stripe interpolates color from yellow (bottom) to deep blue (top) using manual pixel drawing.
 */
void drawLargeFish() {
  glColor3f(1.0f, 0.0f, 0.0f); // Hex #FF0000 (pure red) PANTONE True Red

  // Draw the body (octahedron)
  glPushMatrix();
    glScalef(75.0f, 25.0f, 12.5f); // Scale to 150 wide, 50 tall, 25 deep
    glutWireOctahedron();
  glPopMatrix();

  // Tail
  float tail_length = 20.0f;
  float tail_width_at_base = 20.0f;

  // Tip of the tail (attached to the body's right edge)
  float tip_x = 75.0f;
  float tip_y = 0.0f;

  // Base of the tail
  float base_x = tip_x + tail_length;
  float base_bottom_y = -tail_width_at_base / 2.0f;
  float base_top_y = tail_width_at_base / 2.0f;

  // Calculate Stripe Position (3/4 along tail length) ---
  float fraction_along_length = 0.75f; // Position fraction (0=tip, 1=base)
  float stripe_x = tip_x + tail_length * fraction_along_length;
  float stripe_height = tail_width_at_base * fraction_along_length; // Height scales linearly
  float stripe_bottom_y = -stripe_height / 2.0f;
  float stripe_top_y = stripe_height / 2.0f;

  // Draw Wireframe Tail Outline 
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_LOOP);
    glVertex3f(tip_x, tip_y, 0.0f);             // Tip
    glVertex3f(base_x, base_bottom_y, 0.0f);    // Bottom of tail base
    glVertex3f(base_x, base_top_y, 0.0f);       // Top of tail base
  glEnd();

  // Define stripe endpoint colors
  float color_bottom[3] = {1.0f, 1.0f, 0.0f}; // Yellow
  float color_top[3]    = {0.0f, 0.0f, 0.5f}; // Deep blue

  // Step size for pixel drawing along the vertical stripe
  float step = 1.0f;

  glBegin(GL_POINTS);
    // Loop vertically along the calculated stripe position
    for (float y = stripe_bottom_y; y <= stripe_top_y; y += step) {
        // Calculating interpolation factor 't' (0 at bottom, 1 at top of the stripe)
        float t;
        t = (y - stripe_bottom_y) / (stripe_top_y - stripe_bottom_y);
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;

        // Interpolate color: color = (1 - t) * color_bottom + t * color_top
        float R = (1.0f - t) * color_bottom[0] + t * color_top[0];
        float G = (1.0f - t) * color_bottom[1] + t * color_top[1];
        float B = (1.0f - t) * color_bottom[2] + t * color_top[2];

        // Set the interpolated color for the current pixel
        glColor3f(R, G, B);

        // Draw the pixel at the calculated position on the stripe (using stripe_x)
        glVertex3f(stripe_x, y, 0.0f);
    }
  glEnd();
}

/**
 * Draws the small fish with a wireframe octahedron body and a triangular tail.
 * The body is 50 units wide, 20 units tall, and 10 units deep, 
 * colored in PANTONE Tangelo (RGB: 1.0, 0.5, 0.0).
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
  float tail_length = 7.0f;
  float tail_width = 7.0f;
  glBegin(GL_LINE_LOOP);
    glVertex3f(25.0f, 0.0f, 0.0f);               // Tip attached to body (right edge after scaling)
    glVertex3f(25.0f + tail_length, -tail_width / 2, 0.0f); // Bottom of tail base
    glVertex3f(25.0f + tail_length, tail_width / 2, 0.0f);  // Top of tail base
  glEnd();
}

/**
 * Draws the pause button in the upper left corner labeled 'P'
 */
void drawPauseButton() {
  // Draw button as a filled rectangle
  glColor3f(0.0f, 0.0f, 0.0f); 
  glBegin(GL_QUADS);
    glVertex3f(button_x, button_y, -400.0f);                    // Bottom-left
    glVertex3f(button_x + button_width, button_y, -400.0f);     // Bottom-right
    glVertex3f(button_x + button_width, button_y + button_height, -400.0f); // Top-right
    glVertex3f(button_x, button_y + button_height, -400.0f);    // Top-left
  glEnd();

  // Draw 'P' text
  glColor3f(1.0f, 1.0f, 1.0f);  
  glRasterPos3f(button_x + 10.0f, button_y + 10.0f, -400.0f); 
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'P');
}

/**
 * Draws the Box using GL_QUADS.
 * This boxes are use for Cactus drawing.
 */
void drawBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
  // Front face outline
  glBegin(GL_LINE_LOOP);
    glVertex3f(minX, minY, maxZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(minX, maxY, maxZ);
  glEnd();
  // Back face outline
  glBegin(GL_LINE_LOOP);
    glVertex3f(minX, minY, minZ);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, maxY, minZ);
  glEnd();
  // Left face outline
  glBegin(GL_LINE_LOOP);
    glVertex3f(minX, minY, minZ);
    glVertex3f(minX, minY, maxZ);
    glVertex3f(minX, maxY, maxZ);
    glVertex3f(minX, maxY, minZ);
  glEnd();
  // Right face outline
  glBegin(GL_LINE_LOOP);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(maxX, maxY, minZ);
  glEnd();
  // Top face outline
  glBegin(GL_LINE_LOOP);
    glVertex3f(minX, maxY, minZ);
    glVertex3f(maxX, maxY, minZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(minX, maxY, maxZ);
  glEnd();
  // Bottom face outline
  glBegin(GL_LINE_LOOP);
    glVertex3f(minX, minY, minZ);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(minX, minY, maxZ);
  glEnd();
}

/**
 * Initializes display lists for the large fish, small fish, cactus and pause button.
 * This is called in main method.
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

  pauseButtonList = glGenLists(3);
  glNewList(pauseButtonList, GL_COMPILE);
    drawPauseButton();
  glEndList();

  cactusList = glGenLists(4);
  glNewList(cactusList, GL_COMPILE);
    glColor3f(0.0f, 0.5f, 0.0f); // Dark green
    // Main box
    drawBox(50.0f, -400.0f, -425.0f, 100.0f, -225.0f, -375.0f);
    // Left horizontal arm 
    drawBox(0.0f, -312.5f, -425.0f, 50.0f, -292.5f, -375.0f);
    // Left upward arm
    drawBox(0.0f, -292.5f, -425.0f, 12.0f, -232.5f, -375.0f); 
    // Right horizontal arm
    drawBox(100.0f, -268.75f, -425.0f, 150.0f, -248.75f, -375.0f);
    // Box 5: Right upward arm (
    drawBox(138.0f, -248.75f, -425.0f, 150.0f, -188.75f, -375.0f); 
  glEndList();
}

/**
 * The display callback function that renders the scene.
 * Clears the screen to PANTONE Spun Sugar and draws the large and small fish at their initial positions
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
    glRotatef(rotation_angle, 0.0f, 1.0f, 0.0f); 
    glCallList(largeFishList);
  glPopMatrix();

  // Small fish
  glPushMatrix();
    // 75 units from left (-400), 50 units above bottom (-400)
    glTranslatef(small_fish_center_x, small_fish_center_y, small_fish_center_z); 
    glCallList(smallFishList);
  glPopMatrix();

  // Draw bubbles
  glColor3f(0.0f, 0.0f, 0.0f);

  for (size_t i = 0; i < bubbles.size(); i++) {
    glPushMatrix();
      glTranslatef(bubbles[i].x, bubbles[i].y, bubbles[i].z);
      glutWireSphere(5.0f, 10, 10); // Radius 5 = diameter 10, 10 slices/stacks
    glPopMatrix();
  } 

  glCallList(pauseButtonList);
  glCallList(cactusList);
  glutSwapBuffers(); 
}

/**
 * Timer callback to update the diamond's position.
 * It moves the diamond and triggers a redisplay.
 */
void timerFunc(int value) {
  if (!is_paused) {
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

    // Adds a new bubble periodically
    bubble_counter++;
    if (bubble_counter >= 24) { // Example: 1.2 seconds at 20 fps
        Bubble new_bubble = {small_fish_nose_x, small_fish_center_y, small_fish_center_z};
        bubbles.push_back(new_bubble);
        bubble_counter = 0;
    }

    // Moves bubble upward by 5 units per frame
    for (size_t i = 0; i < bubbles.size(); ++i) {
        bubbles[i].y += 5.0f;
    }

    // Remove bubbles that exit the tank (y > 400)
    for (int i = bubbles.size() - 1; i >= 0; --i) {
        if (bubbles[i].y > 400.0f) {
            bubbles.erase(bubbles.begin() + i);
        }
    }
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
  if (key == 'P' || key == 'p') {
    is_paused = !is_paused;
  }
}

/**
 * Mouse callback to handle pause button clicks
 */
void mouseCallback(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Convert screen coordinates (0,0 at top-left) to tank coordinates (-400,400 at bottom-left)
    float tank_x = (float)x * 800.0f / canvas_Width - 400.0f;
    float tank_y = (float)(canvas_Height - y) * 800.0f / canvas_Height - 400.0f;

    // Check if click is within button bounds
    if (tank_x >= button_x && tank_x <= button_x + button_width &&
        tank_y >= button_y && tank_y <= button_y + button_height) {
        is_paused = !is_paused; // Toggle pause state
    }
  }
}

char canvas_Name[] = "Aquarium Screen-Saver Simulation";

int main(int argc, char ** argv) {
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  glutDisplayFunc(displayCallback);
  glutKeyboardFunc(keyboardCallback);
  glutMouseFunc(mouseCallback);

  // Create the display list for the diamond.
  initDisplayLists();

  // Start the animation
  glutTimerFunc(50, timerFunc, 0);

  glutMainLoop();
  return 0;
}
