/**
 * Graphics Pgm 5 for Prajun Trital
 * 
 * EXTRA CREDIT
 * 
 * ARCHITECTURE WITH ANIMATION & PERSPECTIVE VIEW
*/

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "OpenGL445Setup-2025.h"

#define canvas_Width 800
#define canvas_Height 800

// Display list indices
GLuint uLetterList;
GLuint aLetterList;
GLuint hLetterList;
GLuint spindleList;

// Constant for cube size
const float CUBE_SIZE = 50.0f;

// Constant for sphere radius (diameter 50)
const float SPHERE_RADIUS = 25.0f;

// Global rotation angle for animation
float rotationAngle = 0.0f;

/**
 * Draws a cube at the specified position.
 */
void drawCube(float x, float y, float z, bool solid) {
    glPushMatrix();
      glTranslatef(x, y, z);
      if (solid) {
          glutSolidCube(CUBE_SIZE);
      } else {
          glutWireCube(CUBE_SIZE);
      }
    glPopMatrix();
}

/**
 * Creates the U letter using solid cubes.
 */
/**
 * Creates the U letter using 10 solid cubes.
 * Total dimensions: 200x200 (4 cubes wide, 4 cubes high).
 */
void createULetter() {
  glColor3f(0.0f, 0.75f, 1.0f); 

  // Left vertical column (4 cubes) - Center X = -75
  drawCube(-75.0f, -75.0f, 0.0f, true); // Bottom
  drawCube(-75.0f, -25.0f, 0.0f, true);
  drawCube(-75.0f,  25.0f, 0.0f, true);
  drawCube(-75.0f,  75.0f, 0.0f, true); // Top

  // Bottom horizontal bar (2 cubes) - Center Y = -75
  // These fill the gap between the left and right columns
  drawCube(-25.0f, -75.0f, 0.0f, true); // Middle-left
  drawCube( 25.0f, -75.0f, 0.0f, true); // Middle-right

  drawCube( 75.0f, -75.0f, 0.0f, true); // Bottom
  drawCube( 75.0f, -25.0f, 0.0f, true);
  drawCube( 75.0f,  25.0f, 0.0f, true);
  drawCube( 75.0f,  75.0f, 0.0f, true); // Top
}

/**
 * Creates the A letter using wireframe cubes.
 */
void createALetter() {
  glColor3f(1.0f, 1.0f, 1.0f); // White color for wireframe

  const float z_pos = 0.0f; 

  // Draw cubes using pre-calculated centered coordinates relative to local (0,0,0)
  // Layer 1 (Bottom): Cubes 1, 2
  drawCube(-87.5f, -75.0f, z_pos, false); // Cube 1 
  drawCube( 112.5f, -75.0f, z_pos, false); // Cube 2 

  // Layer 2: Cubes 3, 4, 5, 6
  drawCube(-62.5f, -25.0f, z_pos, false); // Cube 3 (50.0 - 112.5, 75.0 - 100.0)
  drawCube(-12.5f, -25.0f, z_pos, false); // Cube 4 (100.0 - 112.5, 75.0 - 100.0)
  drawCube( 37.5f, -25.0f, z_pos, false); // Cube 5 (150.0 - 112.5, 75.0 - 100.0)
  drawCube( 87.5f, -25.0f, z_pos, false); // Cube 6 (200.0 - 112.5, 75.0 - 100.0)

  // Layer 3: Cubes 7, 8
  drawCube(-37.5f,  25.0f, z_pos, false); // Cube 7 (75.0 - 112.5, 125.0 - 100.0)
  drawCube( 62.5f,  25.0f, z_pos, false); // Cube 8 (175.0 - 112.5, 125.0 - 100.0)

  // Layer 4 (Top): Cubes 9, 10
  drawCube(-12.5f,  75.0f, z_pos, false); // Cube 9 (75.0 - 112.5, 175.0 - 100.0)
  drawCube( 37.5f,  75.0f, z_pos, false); // Cube 10 (175.0 - 112.5, 175.0 - 100.0)

   // --- Draw Solid Red Sphere in the Gap ---
   glColor3f(1.0f, 0.0f, 0.0f); // Red color

   // Calculate center of the gap between Cube 7 (x=-37.5) and Cube 8 (x=62.5)
   // Gap Center X = (-37.5 + 62.5) / 2 = 25.0 / 2 = 12.5
   // Gap Center Y is the same as Cube 7 and 8's Y center = 25.0
   const float sphere_center_x = 12.5f;
   const float sphere_center_y = 25.0f; // c7y or c8y

   glPushMatrix();
     glTranslatef(sphere_center_x, sphere_center_y, z_pos);
     glutSolidSphere(SPHERE_RADIUS, 30, 30);
   glPopMatrix();
}

/**
 * Creates the inverted triangular spindle geometry with base pointing upwards.
 * Base width 50, height 75. Drawn relative to its tip at (0,0,0).
 */
void createSpindle() {
  glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the spindle

  const float base_width = 50.0f;
  const float height = 75.0f;
  const float half_base = base_width / 2.0f;
  const float z_pos = 0.0f; // Keep in the same Z-plane

  glBegin(GL_TRIANGLES);
    // Define vertices relative to the tip (0,0)
    glVertex3f(0.0f, 0.0f, z_pos);        // Tip (at the local origin)
    glVertex3f(-half_base, height, z_pos); // Base Left vertex (above the tip)
    glVertex3f(half_base, height, z_pos);  // Base Right vertex (above the tip)
  glEnd();
}

/**
 * Creates the H letter using 10 solid cubes.
 * Total dimensions: 200x200 (4 cubes wide, 4 cubes high).
 */
void createHLetter() {
  glColor3f(0.0f, 0.75f, 1.0f); // Light blue color

  // Define vertical center coordinates for the 4 rows of cubes
  const float y_bottom = -75.0f;
  const float y_mid_bottom = -25.0f;
  const float y_mid_top = 25.0f;
  const float y_top = 75.0f;

  // Define horizontal center coordinates for the 4 columns of cubes
  const float x_left = -75.0f;
  const float x_mid_left = -25.0f;
  const float x_mid_right = 25.0f;
  const float x_right = 75.0f;

  // Vertical center for the connecting bar
  // Average of the two middle vertical centers (-25 and 25) is 0
  const float y_connector_center = 0.0f;

  // Left vertical column (4 cubes)
  drawCube(x_left, y_bottom, 0.0f, true);
  drawCube(x_left, y_mid_bottom, 0.0f, true);
  drawCube(x_left, y_mid_top, 0.0f, true);
  drawCube(x_left, y_top, 0.0f, true);

  // Right vertical column (4 cubes)
  drawCube(x_right, y_bottom, 0.0f, true);
  drawCube(x_right, y_mid_bottom, 0.0f, true);
  drawCube(x_right, y_mid_top, 0.0f, true);
  drawCube(x_right, y_top, 0.0f, true);

  // Middle horizontal connectors (2 cubes) - Centered vertically
  drawCube(x_mid_left, y_connector_center, 0.0f, true); // Connect at y=0
  drawCube(x_mid_right, y_connector_center, 0.0f, true); // Connect at y=0
}

/**
 * Initializes display lists for U, A, H letters and the sphere.
 */
void initDisplayLists() {
    // Create display list for the U letter.
    uLetterList = glGenLists(1);
    glNewList(uLetterList, GL_COMPILE);
      createULetter();
    glEndList();
    
    // Create display list for the A letter.
    aLetterList = glGenLists(2);
    glNewList(aLetterList, GL_COMPILE);
      createALetter();
    glEndList();

    spindleList = glGenLists(3); // Assign index to spindleList
    glNewList(spindleList, GL_COMPILE);
      createSpindle(); // Compile the spindle drawing function
    glEndList();
    
    // Create display list for the H letter.
    hLetterList = glGenLists(4);
    glNewList(hLetterList, GL_COMPILE);
      createHLetter();
    glEndList();
}

/**
 * The display callback function that renders the animated scene,
 * rotating the entire U‑A‑H characters about the spindle axis.
 */
void displayCallback() {
  // Clear the screen
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset model‑view
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // 1) Pull the whole scene back so world‑Z = -400 lies at camera Z = 0
  glTranslatef(0.0f, 0.0f, -400.0f);

  // --- Draw the static spindle at world (10,0,-400) ---
  glPushMatrix();
    glTranslatef(10.0f, 0.0f, 0.0f);     // pivot in camera space
    glCallList(spindleList);
  glPopMatrix();

  // --- Rotate only the letters (U, A, H and the red sphere) around that pivot ---
  glPushMatrix();
    // Move pivot to origin
    glTranslatef(10.0f, 0.0f, 0.0f);
    // Apply rotation
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    // Move pivot back
    glTranslatef(-10.0f, 0.0f, 0.0f);

    // Compute centers
    const float u_width = 200.0f, a_width = 250.0f, h_width = 200.0f, gap = 50.0f;
    const float total_w = u_width + gap + a_width + gap + h_width;
    const float start_x = -total_w * 0.5f;
    const float u_cx = start_x + u_width * 0.5f;
    const float a_cx = u_cx + u_width * 0.5f + gap + a_width * 0.5f;
    const float h_cx = a_cx + a_width * 0.5f + gap + h_width * 0.5f;
    const float center_y = (-200.0f) - (-100.0f);

    // Draw U
    glPushMatrix();
      glTranslatef(u_cx, center_y, 0.0f);
      glCallList(uLetterList);
    glPopMatrix();

    // Draw A + red sphere (but no spindle inside here)
    glPushMatrix();
      glTranslatef(a_cx, center_y, 0.0f);
      glCallList(aLetterList);
    glPopMatrix();

    // Draw H
    glPushMatrix();
      glTranslatef(h_cx, center_y, 0.0f);
      glCallList(hLetterList);
    glPopMatrix();
  glPopMatrix();

  // Swap buffers
  glutSwapBuffers();
}



/**
 * Handles keyboard input to exit the program.
 */
void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 'Q' || key == 'q') {
        exit(0);
    }
}

void timerCallback(int value) {
  rotationAngle += 4.0f;
  if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
  glutPostRedisplay();
  // 90 steps × 4° = 360° over 2000 ms → ∼22 ms per step
  glutTimerFunc(22, timerCallback, 0);
}

char canvas_Name[] = "University Lights";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    my_setup(canvas_Width, canvas_Height, canvas_Name);
    
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);

    glutTimerFunc(22, timerCallback, 0);
    
    // Enable depth testing for proper 3D rendering.
    glEnable(GL_DEPTH_TEST);
    
    // Create the display lists for the U, A, H letters.
    initDisplayLists();
    
    glutMainLoop();
    return 0;
}
