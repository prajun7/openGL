/**
 * Graphics Pgm 5 for Prajun Trital
 * 
 * EXTRA CREDIT
 * - Shift the Scene with U and N keys.
 * - Change the Light Source Intensity with K and L keys.
 * 
 * ARCHITECTURE
 * GLUT event-driven generation of a canvas with lighting and multiple objects, including letters, sphere and a 
 * spindle. The rendering process is controlled by the `displayCallback` function, that calls the series of 
 * display lists to draw the letters, spindle and sphere. The `displayCallback` function utilizes the `drawCube` 
 * function to draw those cubes. The `initDisplayLists` initiates the displaylist for letters, sphere and 
 * spindle. These objects are creted with a series of transformations and material properties to enchance 
 * their appearacnce. The lighting is configured using the `initLighting` fucntion to ensure the scene 
 * is illuminated correctly. The `initLighting` and `initDisplayLists` functions are being called from the main. 
 * The lighting setup uses a single light source placed at the origin and directed along the negative Z-axis. 
 * The `timerCallback` function is responsible for rotating the scene, with a time interval of 22 ms per update, 
 * ensuring that the rotation completes smoothly in approximately 2 seconds. The `keyboardCallback` detect the
 * keyboard press to move the object as well as to change the light intensity. The `gluPerspective` 
 * is used to set the camera's field of view to 90 degrees, with an aspect ratio defined by the window's width 
 * and height, and a near clipping plane at 1.0 and a far clipping plane at 1010.0, ensuring proper perspective 
 * projection for the scene.
 * 
 * EXTRA CREDIT ARCHITECTURE
 * The `sceneYOffset` variable is updated based on keyboard inputs to move the scene up and down, and its value is 
 * used in the `displayCallback` to adjust the scene's position. Similarly, the `lightIntensity` variable is modified
 * through keyboard presses to control the light intensity, and this value is utilized in the `changeLightIntensity`
 * function to dynamically adjust the lighting in the scene.
*/

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "OpenGL445Setup-2025.h"

#define canvas_Width 800
#define canvas_Height 800

// Display lists
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

// Global Y Offset
float sceneYOffset = 0.0f;

// Global light Intensity
float lightIntensity = 1.0f;

/**
 * Draws a cube at the specified position.
 *
 * @param x      X-coordinate of the cube center.
 * @param y      Y-coordinate of the cube center.
 * @param z      Z-coordinate of the cube center.
 * @param solid  If true, renders a solid cube; otherwise, renders a wireframe cube.
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
 * Creates the letter 'U' using 10 solid cubes.
 *
 * Constructs a capital 'U' by arranging 10 solid cubes. 
 * The structure consists of two vertical columns and a connecting base.
 */
void createULetter() {
  GLfloat amb[]  = { 0.0f, 0.467f, 0.784f, 1.0f };  
  GLfloat diff[] = { 0.0f, 0.498f, 1.0f, 1.0f };
  GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat shin  = 60.0f; 

  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR,spec);
  glMaterialf (GL_FRONT, GL_SHININESS,shin);

  // Left vertical column (4 cubes)
  drawCube(-75.0f, -75.0f, 0.0f, true); // Bottom
  drawCube(-75.0f, -25.0f, 0.0f, true);
  drawCube(-75.0f,  25.0f, 0.0f, true);
  drawCube(-75.0f,  75.0f, 0.0f, true); // Top

  // Bottom horizontal bar (2 cubes)
  drawCube(-25.0f, -75.0f, 0.0f, true); // Middle-left
  drawCube( 25.0f, -75.0f, 0.0f, true); // Middle-right

  drawCube( 75.0f, -75.0f, 0.0f, true); // Bottom
  drawCube( 75.0f, -25.0f, 0.0f, true);
  drawCube( 75.0f,  25.0f, 0.0f, true);
  drawCube( 75.0f,  75.0f, 0.0f, true); // Top
}

/**
 * Creates the letter 'H' using 10 solid cubes.
 *
 * It constructs a capital 'H' by arranging 10 solid cubes.
 * The structure consists of two vertical columns and a central 
 * horizontal connector bar.
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

  // Vertical center coordinates for the 4 rows of cubes
  const float y_bottom = -75.0f;
  const float y_mid_bottom = -25.0f;
  const float y_mid_top = 25.0f;
  const float y_top = 75.0f;

  // Horizontal center coordinates for the 4 columns of cubes
  const float x_left = -75.0f;
  const float x_mid_left = -25.0f;
  const float x_mid_right = 25.0f;
  const float x_right = 75.0f;

  // Vertical center for the connecting bar
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

  // Horizontal connector
  drawCube(x_mid_left, y_connector_center, 0.0f, true); 
  drawCube(x_mid_right, y_connector_center, 0.0f, true);
}

/**
 * Creates the letter 'A' using 10 wireframe cubes and adds red sphere.
 * 
 * It constructs the capital letter 'A' by arranging 10 wireframe cubes
 * across four vertical layers. A candy-apple-red sphere is also placed between
 * the inner cubes of the third layer.
 */
void createALetter() {
  GLfloat amb1[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diff1[] = { 0.75f,0.75f,0.75f,1.0f };
  GLfloat spec1[] = { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat shin1  = 50.0f; 

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb1);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff1);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec1);
  glMaterialf (GL_FRONT, GL_SHININESS, shin1);

  const float z_pos = 0.0f; 

  // Layer 1 (Bottom): Cubes 1, 2
  drawCube(-87.5f, -75.0f, z_pos, false); // Cube 1 
  drawCube( 112.5f, -75.0f, z_pos, false); // Cube 2 

  // Layer 2: Cubes 3, 4, 5, 6
  drawCube(-62.5f, -25.0f, z_pos, false); // Cube 3
  drawCube(-12.5f, -25.0f, z_pos, false); // Cube 4 
  drawCube( 37.5f, -25.0f, z_pos, false); // Cube 5 
  drawCube( 87.5f, -25.0f, z_pos, false); // Cube 6 

  // Layer 3: Cubes 7, 8
  drawCube(-37.5f,  25.0f, z_pos, false); // Cube 7 
  drawCube( 62.5f,  25.0f, z_pos, false); // Cube 8 

  // Layer 4 (Top): Cubes 9, 10
  drawCube(-12.5f,  75.0f, z_pos, false); // Cube 9
  drawCube( 37.5f,  75.0f, z_pos, false); // Cube 10

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
   const float sphere_center_y = 25.0f;

   glPushMatrix();
     glTranslatef(sphere_center_x, sphere_center_y, z_pos);
     glutSolidSphere(SPHERE_RADIUS, 30, 30);
   glPopMatrix();
}

/**
 * Creates the inverted triangular spindle with base pointing upwards with
 * base of 50 units and height of 75.
 */
void createSpindle() {
  GLfloat amb[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diff[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat spec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat shin  = 10.0f; 

  glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  spec);
  glMaterialf (GL_FRONT, GL_SHININESS, shin);

  const float base_width = 50.0f;
  const float height = 75.0f;
  const float half_base = base_width / 2.0f;
  const float z_pos = 0.0f;

  glBegin(GL_TRIANGLES);
    // Define vertices relative to the tip (0,0)
    glVertex3f(0.0f, 0.0f, z_pos);         // Tip (at the local origin)
    glVertex3f(-half_base, height, z_pos); // Base Left vertex (above the tip)
    glVertex3f(half_base, height, z_pos);  // Base Right vertex (above the tip)
  glEnd();
}

/**
 * It compiles the characters 'U', 'A', and 'H',
 * as well as the spindle object, into OpenGL display lists.
 * 
 * Each element is assigned a unique display list ID:
 * - `uLetterList` — Compiles the 'U' letter.
 * - `aLetterList` — Compiles the 'A' letter.
 * - `spindleList` — Compiles the spindle.
 * - `hLetterList` — Compiles the 'H' letter.
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

    spindleList = glGenLists(3); 
    glNewList(spindleList, GL_COMPILE);
      createSpindle(); 
    glEndList();
    
    // Create display list for the H letter.
    hLetterList = glGenLists(4);
    glNewList(hLetterList, GL_COMPILE);
      createHLetter();
    glEndList();
}

/**
 * Initializes the lighting configuration for the scene.
 * It enables depth testing for correct 3D rendering. 
 * Also applies smooth shading.
 */
void initLighting() {
  // Light properties
  // Position the light in world space (x, y, z, w)
  // Set light position at origin, pointing down negative Z
  GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};

  // Ambient light 
  GLfloat lightAmb[]    = { 0.3f, 0.3f, 0.3f, 1.0f }; 

  // Diffuse light
  GLfloat lightDiff[]   = { 1.0f, 1.0f, 1.0f, 1.0f };  

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

  // Using smooth shading
  glShadeModel(GL_SMOOTH);               
}

/**
 * Renders the animated U-A-H scene with the sphere and spindle.
 * 
 * It is responsible for rendering the entire scene, which includes:
 * - Clearing the screen and depth buffer.
 * - Setting up the model-view matrix.
 * - Rendering a stationary spindle object.
 * - Rotating the letters 'U', 'A', 'H', and a red sphere around the spindle axis.
 *
 * The letters are positioned relative to one another with appropriate gaps and 
 * centered vertically. The rotation is performed around the spindle by translating 
 * to the pivot point, applying the rotation, and translating back.
 */
void displayCallback() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset model‑view
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.0f, sceneYOffset, -400.0f);

  // Draw the spindle
  glPushMatrix();
    glTranslatef(10.0f, 0.0f, 0.0f);   
    glCallList(spindleList);
  glPopMatrix();

  // Rotate the letters (U, A, H and the red sphere) around that spindle
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

    // Draw A + red sphere
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
 * Updates the lighting parameters based on the current light intensity.
 * 
 * This function recalculates the ambient, diffuse, and specular components of 
 * the light using the current `lightIntensity` value. 
 * It is called from keyboard callback when the user increases or decreases the 
 * light intensity.
 */
void changeLightIntensity() {
  GLfloat amb[]  = {0.3f * lightIntensity, 0.3f * lightIntensity, 0.3f * lightIntensity, 1.0f};
  GLfloat diff[] = {0.8f * lightIntensity, 0.8f * lightIntensity, 0.8f * lightIntensity, 1.0f};
  GLfloat spec[] = {1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
}

/**
 * Handles keyboard input for manipulating the University lights and scene.
 *
 * This function responds to specific key presses:
 * - 'u' / 'U': Moves the scene upward by 25 units.
 * - 'n' / 'N': Moves the scene downward by 25 units.
 * - 'k' / 'K': Increases the light intensity by 0.1 units.
 * - 'l' / 'L': Decreases the light intensity by 0.1 units.
 * - 'q' / 'Q': Exits the application.
 *
 * Light intensity changes invoke the `changeLightIntensity()` function to reflect
 * the new settings in the scene.
 *
 * @param key The key pressed by the user.
 * @param x The x-coordinate of the mouse pointer.
 * @param y The y-coordinate of the mouse pointer.
 */
void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 'Q' || key == 'q') {
        exit(0);
    }
    if (key == 'U' || key == 'u') {
      sceneYOffset += 25.0f; // Move up by 25 units 
    }
    if (key == 'N' || key == 'n') {
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

/**
 * Animates the University lights in a counterclockwise direction 
 * by 4 degrees per frame. A full rotation (360 degrees) is completed in approximately 
 * 2 seconds. Since 360 / 4 = 90 steps are required for a full rotation, each step is spaced 
 * roughly 22 milliseconds apart (2000 ms / 90 ≈ 22 ms).
 * 
 * @param val Integer Used to determine which time event to envoke.
 */
void timerCallback(int value) {
  rotationAngle += 4.0f;

  if (rotationAngle >= 360.0f) {
    rotationAngle -= 360.0f;
  }

  glutPostRedisplay();
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
    
    initDisplayLists();
    
    glutMainLoop();
    return 0;
}
