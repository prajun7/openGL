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

float sceneYOffset = 0.0f;

float lightIntensity = 1.0f;

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
  GLfloat amb[]  = { 0.0f, 0.467f, 0.784f, 1.0f };  
  GLfloat diff[] = { 0.0f, 0.498f, 1.0f, 1.0f };
  GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat shin  = 60.0f; 

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec);
  glMaterialf (GL_FRONT, GL_SHININESS, shin);

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
 * Creates the H letter using 10 solid cubes.
 * Total dimensions: 200x200 (4 cubes wide, 4 cubes high).
 */
void createHLetter() {
  GLfloat amb[]  = { 0.0f, 0.467f, 0.784f, 1.0f };  
  GLfloat diff[] = { 0.0f, 0.498f, 1.0f, 1.0f };
  GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat shin  = 60.0f; 

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec);
  glMaterialf (GL_FRONT, GL_SHININESS, shin);

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
  const float y_connector_center = -50.0f;

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
 * Creates the A letter using wireframe cubes.
 */
void createALetter() {
  // Silver‑like wireframe material
  GLfloat amb1[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diff1[] = { 0.75f,0.75f,0.75f,1.0f };
  GLfloat spec1[] = { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat shin1  = 50.0f;  // moderate shininess

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb1);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff1);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec1);
  glMaterialf (GL_FRONT, GL_SHININESS, shin1);

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

  // Now for candy‑apple‑red sphere: hex #FF0800 → (1.0, 0.031, 0.0) 
  GLfloat amb2[]  = { 0.5f, 0.015f, 0.0f, 1.0f };
  GLfloat diff2[] = { 1.0f, 8/255.0f, 0.0f, 1.0f };
  GLfloat spec2[] = { 0.6f, 0.6f, 0.6f, 1.0f };
  GLfloat shin2  = 50.0f; 

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb2);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff2);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec2);
  glMaterialf (GL_FRONT, GL_SHININESS, shin2);

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
  GLfloat amb[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diff[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat spec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat shin  = 10.0f;  // very broad, dull highlights

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec);
  glMaterialf (GL_FRONT, GL_SHININESS, shin);

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

void initLighting() {
  // Light properties
  // Position the light in world space (x, y, z, w)
  // Set light position at origin, pointing down negative Z
  GLfloat lightPos[] = {0.0f, 0.0f, 1.0f, 0.0f};

  // Ambient light 
  GLfloat lightAmb[]    = { 0.3f, 0.3f, 0.3f, 1.0f }; 

  // Diffuse light
  GLfloat lightDiff[]   = { 0.8f, 0.8f, 0.8f, 1.0f };  

  // Specular light
  GLfloat lightSpec[]   = { 1.0f, 1.0f, 1.0f, 1.0f };  

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

  // Enable depth testing for proper 3D rendering.
  glEnable(GL_DEPTH_TEST); 

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Use smooth shading
  glShadeModel(GL_SMOOTH);               
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

  glTranslatef(0.0f, sceneYOffset, -400.0f);

  // Draw the static spindle at world (10,0,-400)
  glPushMatrix();
    glTranslatef(10.0f, 0.0f, -400.0f);   
    glCallList(spindleList);
  glPopMatrix();

  // Rotate only the letters (U, A, H and the red sphere) around that spindle
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

void changeLightIntensity() {
  // if (lightIntensity < 0.1f) lightIntensity = 0.1f;
  // if (lightIntensity > 2.0f) lightIntensity = 2.0f;

  // Update light parameters based on current intensity
  GLfloat amb[]  = {0.3f * lightIntensity, 0.3f * lightIntensity, 0.3f * lightIntensity, 1.0f};
  GLfloat diff[] = {0.8f * lightIntensity, 0.8f * lightIntensity, 0.8f * lightIntensity, 1.0f};
  GLfloat spec[] = {1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
}

/**
 * Handles keyboard input to exit the program.
 */
void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 'Q' || key == 'q') {
        exit(0);
    }
    if (key == 'U' || key == 'u') {
      sceneYOffset += 25.0f; // Move up by 25 units 
    }
    if (key == 'D' || key == 'd') {
      sceneYOffset -= 25.0f; // Move down by 25 units 
    }
    if (key == 'K' || key == 'k') {
      lightIntensity += 0.1f; // Increase light intensity
      changeLightIntensity();
    }
    if (key == 'L' || key == 'l') {
      lightIntensity -= 0.1f; // Decrease light intensity
      changeLightIntensity();
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

    initLighting();
    
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);

    glutTimerFunc(22, timerCallback, 0);
    
    
    // Create the display lists for the U, A, H letters.
    initDisplayLists();
    
    glutMainLoop();
    return 0;
}
