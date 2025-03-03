#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"
#include <cstring>

#define canvas_Width 800
#define canvas_Height 600

// Display list index to draw a diamond.
GLuint diamondList;

// Display list index to draw the landing zone.
GLuint landingZoneList;

// Display list index for the "Fuel" label.
GLuint fuelLabelList; 

// Display list index for the "Fuel" label.
GLuint youWinLabelList; 

// Starting fuel value.
int fuel = 200; 

// Diamond's vertical position.
float diamond_y = 575.0f;

// Diamond's horizontal position.
float diamond_x = 400.0f;

// Starting position (used when simulation begins).
float diamond_initial_y = 575.0f;  

bool simulation_started = false;

float simulation_time = 0.0f;       // Time (in seconds) since the drop started.
float gravity = 0.0f;               // Current gravity acceleration (ft/s^2).

// Gravity constants (in ft/s^2; negative means downward acceleration).
const float MOON_GRAVITY = -5.31f;  // Our Moon's gravity.
const float IO_GRAVITY   = -5.9f;   // Io's gravity constant.

// Frame interval in milliseconds for 20 fps (1000ms / 20 = 50ms).
const int frame_interval = 50;

void drawDiamond() {
  // For UAH's blue color with hex code #0077C8.
  // Hex Breakdown:
  // Red: 00 (0 in decimal)
  // Green: 77 (119 in decimal)
  // Blue: C8 (200 in decimal)

  // Converting to normalized RGB (dividing by 255):
  // Red: 0 / 255 = 0.0
  // Green: 119 / 255 ≈ 0.467
  // Blue: 200 / 255 ≈ 0.784
  glColor3f(0.0f, 0.467f, 0.784f);

  glPushMatrix();
    // Scaling the octahedron so that each edge is about 25 units long.
    glScalef(25.0f, 25.0f, 25.0f);
    glutWireOctahedron();
  glPopMatrix();
}

void drawLandingZone() {
  // 10 units = 10% of 600 = 60
  // 40 units = 40% of 800 = 320
  
  glBegin(GL_LINE_LOOP);
    // Bottom edge of the rectangle at y=0
    glVertex3f(10.0f, 0.0f, -50.0f);     // Bottom-left
    glVertex3f(270.0f, 0.0f, -50.0f);    // Bottom-right

    // Right side of the rectangle (top edge at y=50)
    glVertex3f(270.0f, 50.0f, -50.0f);   // Top-right corner

    // Right edge of the dip (inner top)
    glVertex3f(165.0f, 50.0f, -50.0f);   // Right edge of dip

    // Triangular dip (25 units deep; bottom at y=7 so the dip touches the red line)
    glVertex3f(140.0f, 7.0f, -50.0f);    // Bottom of the dip (center)

    // Left edge of the dip (inner top)
    glVertex3f(115.0f, 50.0f, -50.0f);   // Left edge of dip

    // Left side of the rectangle (top edge at y=50)
    glVertex3f(10.0f, 50.0f, -50.0f);    // Top-left corner
  glEnd();
}

bool isPointInsideLandingDip(float pointX, float pointY) {
  // Landing dip defined by vertices: (115,50), (165,50), (140,7)
  if (pointY < 7.0f || pointY > 50.0f) {
    return false;
}
// At y=50, boundaries are 115 and 165; at y=7, they converge at 140.
float t = (50.0f - pointY) / 43.0f;  // 50 - 7 = 43
float leftBoundary  = 115.0f + 25.0f * t;
float rightBoundary = 165.0f - 25.0f * t;
return (pointX >= leftBoundary && pointX <= rightBoundary);
}

void initDisplayList() {
  diamondList = glGenLists(1);
  glNewList(diamondList, GL_COMPILE);
    drawDiamond();   
  glEndList();

  landingZoneList = glGenLists(2);
    glNewList(landingZoneList, GL_COMPILE);
    drawLandingZone();   
  glEndList();

  fuelLabelList = glGenLists(3);
  glNewList(fuelLabelList, GL_COMPILE);
    const char* fuelLabel = "Fuel";
    for (size_t i = 0; i < strlen(fuelLabel); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fuelLabel[i]);
    }
  glEndList();

  youWinLabelList = glGenLists(4);
  glNewList(youWinLabelList, GL_COMPILE);
    const char* youWinLabel = "YOU WIN";
    for (size_t i = 0; i < strlen(youWinLabel); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, youWinLabel[i]);
    }
  glEndList();
}

/**
 * The display handler sets up the display and renders the scene.
 * The diamond's y-position is updated via the timer function.
 */
void displayHandler() {
    // Clear the color and depth buffers.
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Resets the modelview matrix to ensure transformations start fresh each frame.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPushMatrix();
      glTranslatef(diamond_x, diamond_y, -50.0f);
      glCallList(diamondList);
    glPopMatrix();

    glCallList(landingZoneList);

    glColor3f(0.0f, 0.0f, 0.0f);

    // Draw the "Fuel" label.
    glRasterPos2i(740, 570);
    glCallList(fuelLabelList);

    // Draw the numeric fuel value below the label.
    glRasterPos2i(740, 550);
    char fuelStr[20];
    snprintf(fuelStr, sizeof(fuelStr), "%d", fuel);
    for (size_t i = 0; i < strlen(fuelStr); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fuelStr[i]);
    }

    // Check if the diamond has landed successfully.
    if (isPointInsideLandingDip(diamond_x, diamond_y - 25.0f)) {
      glRasterPos2i((canvas_Width / 2) - 50, canvas_Height / 2);
      glCallList(youWinLabelList);
    }

    // Draw a red line 7 pixels from the bottom
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINES);
      glVertex3f(0.0f, 7.0f, -50.0f);
      glVertex3f(canvas_Width, 7.0f, -50.0f);
    glEnd();
    
    glFlush();
}


/**
 * Timer callback to update the diamond's position.
 * It moves the diamond downward and triggers a redisplay.
 */
void timerFunction(int value) {
  float dt = frame_interval / 1000.0f; // Convert frame interval to seconds.
  
  if (simulation_started) {
      simulation_time += dt;
      // Compute the new vertical position using: p(t) = p0 + 0.5 * a * t^2.
      // Since the diamond falls from the rest, the initial velocity is 0
      diamond_y = diamond_initial_y + 0.5f * gravity * (simulation_time * simulation_time);

      float tip_x = diamond_x;
      float tip_y = diamond_y - 25.0f;  

      // Landing dip triangle vertices: A = (115, 50), B = (165, 50), C = (140, 25)
      if (isPointInsideLandingDip(tip_x, tip_y)) {
        // Snap the diamond into perfect alignment inside the dip:
        diamond_x = 140.0f;    // Center horizontally with the dip.
        diamond_y = 32.0f;     // So that the lower tip (diamond_y - 25) becomes 7, matching the dip's bottom.
        simulation_started = false;
        simulation_time = 0.0f;
    }
  }
  
  glutPostRedisplay();
  glutTimerFunc(frame_interval, timerFunction, 1);
}

/**
 * Keyboard Handler triggers the animation when the key `M` or `I` is pressed.
 * When the key `M` is pressed it uses moon's gravity and when `I` is pressed it uses Io's gravity.
 * The key 'H', 'J' and 'U' are used to move the diamond to left, right and up respectively
 */
void keyboardHandler(unsigned char key, int x, int y) {
  if (!simulation_started) {
    if (key == 'm' || key == 'M') {
      gravity = MOON_GRAVITY;
      simulation_started = true;
      simulation_time = 0.0f;
      diamond_initial_y = diamond_y; 
    }
    if (key == 'i' || key == 'I') {
      gravity = IO_GRAVITY;
      simulation_started = true;
      simulation_time = 0.0f;
      diamond_initial_y = diamond_y;
    }
  }

  // Only move if the simulation has started
  if (simulation_started) {
    if (key == 'h' || key == 'H') {
        diamond_x -= 4.0f;   // Moves left 4 units
    }
    if (key == 'j' || key == 'J') {
        diamond_x += 4.0f;   // Moves right 4 units
    }
    if (key == 'u' || key == 'U') {
      if (fuel > 0) {
          diamond_y += 5.0f; 
          diamond_initial_y += 5.0f; // For continuous gravity simulation.
          fuel -= 5; 
      }
    }
  }
}

char canvas_Name[] = "Diamond Drop";

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    // Register the display handler.
    glutDisplayFunc(displayHandler);

    glutDisplayFunc(displayHandler);
    glutKeyboardFunc(keyboardHandler);

    // Create the display list for the diamond.
    initDisplayList();

    // Set up the timer function for 20 fps.
    glutTimerFunc(frame_interval, timerFunction, 1);

    // Enter the main loop.
    glutMainLoop();

    return 0;
}
