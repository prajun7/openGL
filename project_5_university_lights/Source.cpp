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
void createULetter() {
    glColor3f(0.0f, 0.75f, 1.0f); // Light blue color
    // Left vertical bar of U (3 cubes)
    drawCube(-125.0f, 0.0f, 0.0f, true);
    drawCube(-125.0f, 50.0f, 0.0f, true);
    drawCube(-125.0f, 100.0f, 0.0f, true);
    // Bottom horizontal bar of U (3 cubes)
    drawCube(-75.0f, 0.0f, 0.0f, true);
    drawCube(-25.0f, 0.0f, 0.0f, true);
    drawCube(25.0f, 0.0f, 0.0f, true);
    // Right vertical bar of U (3 cubes)
    drawCube(75.0f, 0.0f, 0.0f, true);
    drawCube(75.0f, 50.0f, 0.0f, true);
    drawCube(75.0f, 100.0f, 0.0f, true);
}

/**
 * Creates the A letter using wireframe cubes.
 */
void createALetter() {
    glColor3f(1.0f, 1.0f, 1.0f); // White color for wireframe
    // Left diagonal bar of A (3 cubes)
    drawCube(-50.0f, 0.0f, 0.0f, false);
    drawCube(-25.0f, 50.0f, 0.0f, false);
    drawCube(0.0f, 100.0f, 0.0f, false);
    // Right diagonal bar of A (2 cubes)
    drawCube(50.0f, 0.0f, 0.0f, false);
    drawCube(25.0f, 50.0f, 0.0f, false);
    // Middle horizontal bar of A (3 cubes)
    drawCube(-25.0f, 25.0f, 0.0f, false);
    drawCube(0.0f, 25.0f, 0.0f, false);
    drawCube(25.0f, 25.0f, 0.0f, false);
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
 * Creates the H letter using solid cubes.
 */
void createHLetter() {
    glColor3f(0.0f, 0.75f, 1.0f); // Light blue color
    // Left vertical bar of H (3 cubes)
    drawCube(-75.0f, 0.0f, 0.0f, true);
    drawCube(-75.0f, 50.0f, 0.0f, true);
    drawCube(-75.0f, 100.0f, 0.0f, true);
    // Middle horizontal bar of H (2 cubes)
    drawCube(-25.0f, 50.0f, 0.0f, true);
    drawCube(25.0f, 50.0f, 0.0f, true);
    // Right vertical bar of H (3 cubes)
    drawCube(75.0f, 0.0f, 0.0f, true);
    drawCube(75.0f, 50.0f, 0.0f, true);
    drawCube(75.0f, 100.0f, 0.0f, true);
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
 * Idle callback that updates the rotation angle for animation.
 */
void idleCallback() {
    rotationAngle += 0.1f;
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
    glutPostRedisplay();
}

/**
 * The display callback function that renders the animated scene.
 */
void displayCallback() {
    // Clear the screen with a black background.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    // Move the camera away from the origin.
    glTranslatef(0.0f, 0.0f, -500.0f);
    // Apply the rotation animation.
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    
    // Draw the U letter.
    glPushMatrix();
      glTranslatef(-250.0f, 0.0f, 0.0f);
      glCallList(uLetterList);
    glPopMatrix();
    
    // Draw the A letter and the sphere.
    glPushMatrix();
      glTranslatef(0.0f, 0.0f, 0.0f);
      glCallList(aLetterList);
      glCallList(sphereList);
    glPopMatrix();
    
    // Draw the H letter.
    glPushMatrix();
      glTranslatef(250.0f, 0.0f, 0.0f);
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

char canvas_Name[] = "UAH 3D Letters Display - Animated";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    my_setup(canvas_Width, canvas_Height, canvas_Name);
    
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutIdleFunc(idleCallback);
    
    // Enable depth testing for proper 3D rendering.
    glEnable(GL_DEPTH_TEST);
    
    // Create the display lists for the U, A, H letters.
    initDisplayLists();
    
    glutMainLoop();
    return 0;
}
