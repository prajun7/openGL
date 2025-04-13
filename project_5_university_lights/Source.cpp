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
GLuint sphereList;

// Constant for cube size
const float CUBE_SIZE = 50.0f;

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

  // User specified bottom-left coords converted to center coords:
  // Cube 1: (0, 0) -> (25, 25)
  // Cube 2: (150, 0) -> (175, 25)
  // Cube 3: (25, 50) -> (50, 75)
  // Cube 4: (75, 50) -> (100, 75)
  // Cube 5: (125, 50) -> (150, 75)
  // Cube 6: (175, 50) -> (200, 75)
  // Cube 7: (50, 100) -> (75, 125)
  // Cube 8: (150, 100) -> (175, 125)
  // Cube 9: (50, 150) -> (75, 175)
  // Cube 10: (150, 150) -> (175, 175)

  // Calculate center offset to center the whole letter around (0,0)
  // X-range of centers: 25 to 200. Midpoint X = (25 + 200) / 2 = 112.5.
  // Y-range of centers: 25 to 175. Midpoint Y = (25 + 175) / 2 = 100.0.
  // const float x_offset = 112.5f; // Removed
  // const float y_offset = 100.0f; // Removed
  const float z_pos = 0.0f; // Keep all cubes in the same Z-plane locally

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
}


/**
 * Creates the red sphere inside the A letter.
 */
void createSphere() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glPushMatrix();
      glTranslatef(0.0f, 50.0f, 0.0f); // Position in the "hole" of A
      glutSolidSphere(25.0f, 30, 30); // Diameter of 50 units
    glPopMatrix();
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
    aLetterList = glGenLists(1);
    glNewList(aLetterList, GL_COMPILE);
      createALetter();
    glEndList();
    
    // Create display list for the sphere in A.
    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE);
      createSphere();
    glEndList();
    
    // Create display list for the H letter.
    hLetterList = glGenLists(1);
    glNewList(hLetterList, GL_COMPILE);
      createHLetter();
    glEndList();
}

/**
 * The display callback function that renders the animated scene.
 */
void displayCallback() {
  // Clear the screen with a black background.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glTranslatef(0.0f, 0.0f, -500.0f); 

  const float u_width = 200.0f;
  const float a_width = 250.0f; 
  const float h_width = 200.0f;
  const float gap = 50.0f;

  // Calculate total width to center the block
  const float total_width = u_width + gap + a_width + gap + h_width;
  const float start_x = -total_width / 2.0f; // 

  const float u_center_x = start_x + u_width / 2.0f;               
  const float a_center_x = u_center_x + u_width / 2.0f + gap + a_width / 2.0f; 
  const float h_center_x = a_center_x + a_width / 2.0f + gap + h_width / 2.0f; 

  const float local_base_y = -100.0f;
  const float target_base_y = -200.0f;
  const float center_y = target_base_y - local_base_y;

  // Draw the U letter.
  glPushMatrix();
    glTranslatef(u_center_x, center_y, 0.0f); // Apply calculated X and Y translation
    glCallList(uLetterList);
  glPopMatrix();

  // Draw the A letter.
  glPushMatrix();
    glTranslatef(a_center_x, center_y, 0.0f); // Apply calculated X and Y translation
    glCallList(aLetterList);
    // glCallList(sphereList); // Sphere is likely not part of A anymore
  glPopMatrix();

  // Draw the H letter.
  glPushMatrix();
    glTranslatef(h_center_x, center_y, 0.0f); // Apply calculated X and Y translation
    glCallList(hLetterList);
  glPopMatrix();

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

char canvas_Name[] = "University Lights";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    my_setup(canvas_Width, canvas_Height, canvas_Name);
    
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    
    // Enable depth testing for proper 3D rendering.
    glEnable(GL_DEPTH_TEST);
    
    // Create the display lists for the U, A, H letters.
    initDisplayLists();
    
    glutMainLoop();
    return 0;
}
