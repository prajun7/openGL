#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"

#define canvas_Width 800
#define canvas_Height 600

// Display list index to draw a diamond.
GLuint diamondList;

// Global variable for the diamond's vertical position.
// Starting at 575 (as in your original translation).
float diamond_y = 575.0f;

// Drop speed per frame (adjust as needed).
const float drop_speed = 5.0f;

// Frame interval in milliseconds for 20 fps (1000ms / 20 = 50ms).
const int frame_interval = 50;

void createDiamondDisplayList() {
    diamondList = glGenLists(1);
    glNewList(diamondList, GL_COMPILE);

      // For UAH's blue color with hex code #0077C8.
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

// This function calls the display list to draw the diamond.
void drawDiamond() {
    glCallList(diamondList);
}

/**
 * The display handler sets up the display and renders the scene.
 * The diamond's y-position is updated via the timer function.
 */
void displayHandler() {
    // Clear the color and depth buffers.
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the modelview matrix.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Translate the diamond. The x and z positions remain fixed.
    glTranslatef(400.0f, diamond_y, -50.0f);
    
    // Draw the diamond.
    drawDiamond();

    glFlush();
}

/**
 * Timer callback to update the diamond's position.
 * It moves the diamond downward and triggers a redisplay.
 */
void timerFunction(int value) {
    // Move the diamond downward by subtracting drop_speed from the y-position.
    diamond_y -= drop_speed;

    // Optional: Reset position if the diamond goes off screen.
    if (diamond_y < 0)
        diamond_y = canvas_Height;  // Reset to the top of the canvas.

    // Mark the current window as needing to be redisplayed.
    glutPostRedisplay();

    // Reset the timer for the next frame.
    glutTimerFunc(frame_interval, timerFunction, 1);
}

char canvas_Name[] = "Diamond Drop";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    // Register the display handler.
    glutDisplayFunc(displayHandler);

    // Create the display list for the diamond.
    createDiamondDisplayList();

    // Set up the timer function for 20 fps.
    glutTimerFunc(frame_interval, timerFunction, 1);

    // Enter the main loop.
    glutMainLoop();

    return 0;
}
