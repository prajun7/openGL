/**
 * Graphics Pgm 2 for Prajun Trital
 * 
 * EXTRA CREDIT
 * - 
 * 
 * ARCHITECTURE - TODO - talk about display list, how it is used to build snowman
 * GLUT event-driven generation of a canvas with a 3 boxed snowman, animated snowflakes and a snowblower.
 * Canvas is produced via the display event handler, which is in display_handler. 
 * The `display_handler` calls the `drawSnowman` function to draw the 3 boxed snowman. It also calls the
 * `drawSnowFlake` function twice to draw two snowfalkes on the left and right of the canvas. The `timer_func` 
 * updates the position of the snowflakes, clears the previously drawn snowfalke with the `clearSnowFlake` 
 * function, and updates the `frameCount` variable that decides to change the snowflake orientation. 
 * The `timer_func` uses 66 ms as the time interval to achieve an animation running at approximately 
 * 16 Frame per second(FPS). The `keyboard_func` detects the keyboard presses to activate the animation. 
 * It also sets the `moveUpward` variable to True if the snowflakes are at the bottom of the canvas.
 */

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"
#include <string>

const float SNOWFLAKE_MOVE_DOWN = 10.0f;
const float SNOWFLAKE_MOVE_RIGHT = 2.0f;

// Lenght of the snowflake.
const float SNOWFLAKE_LENGTH = 60.0f;

// Y-cordinate of the initial left most snowflake.
float centerY = 578.0f;

// Y-cordinate of the second snowflake.
float centerYSecond = 578.0f;

// To calculate the distance from the leftmost diagonal of the snowflake, 
// I need to add the horizontal distance based on the cosine of 60 degrees, 
// i.e., centerX = ((60 / 2) * cos(60))
float centerX = 15;

// Other snowflakes start with the left tips 100 units from the left edge screen
float centerXSecond = 100 + 15;

// Display list idx for "Any Mouse Click Will Start" message
GLuint startMessageListIdx;

// Display list idx for "Any Mouse Click Will Start" message
GLuint remainingMessageListIdx;

// Display list idx for snowman
GLuint snowmanListIdx;

bool animationStarted = false;

bool secondSnowflakeActive = false;

// Tracks if the animation is paused or not
bool isPaused = false;

// Maximum number of times the snowblower can be activated.
int snowBlowerRemainingCount = 20;

/**
* Draws a snowflake with a horizontal main line with diagonal lines at ±60 degrees 
* relative to the horizontal axis
* 
* @param centerX x-coordinate of the center point of the snowflake.
*/
void drawSnowFlake(float centerX, float centerY) {
    glColor3f(1.0, 1.0, 1.0);  

    glBegin(GL_LINES); 
      // Horizontal main line
      glVertex3f(centerX - SNOWFLAKE_LENGTH / 2, centerY, 0.0f);
      glVertex3f(centerX + SNOWFLAKE_LENGTH / 2, centerY, 0.0f);
  
      // Diagonal lines at ±60 degrees from the horizontal axis
      glVertex3f(centerX - 15.0f, centerY - 25.98f, 0.0f);
      glVertex3f(centerX + 15.0f, centerY + 25.98f, 0.0f);
  
      glVertex3f(centerX - 15.0f, centerY + 25.98f, 0.0f);
      glVertex3f(centerX + 15.0f, centerY - 25.98f, 0.0f);

    glEnd();
    glFlush();
}

/**
* Draws a box using series of lines.
* We start with bottom-left -> bottom-right -> top-right and top-left, 
* similar to how the GL_POLYGON draws.
* 
* @param length The length of the box
* @param x_start The x-coordinate of the bottonm left
* @param y_start The y-coordinate of the botton left
*/
void drawBox(float length, float x_start, float y_start) {
  glColor3f(1.0, 1.0, 1.0); 
  glBegin(GL_LINES);  
    // Bottom edge
    glVertex3f(x_start, y_start, 0.0f);
    glVertex3f(x_start + length, y_start, 0.0f);

    // Right edge
    glVertex3f(x_start + length, y_start, 0.0f);
    glVertex3f(x_start + length, y_start + length, 0.0f);

    // Top edge
    glVertex3f(x_start + length, y_start + length, 0.0f);
    glVertex3f(x_start, y_start + length, 0.0f);

    // Left edge
    glVertex3f(x_start, y_start + length, 0.0f);
    glVertex3f(x_start, y_start, 0.0f);
  glEnd();
  glFlush();
}

/**
* Draws the 3 box snowman.
*/
void drawSnowman() {
  drawBox(150.0f, 205.0f, 0.0f);
  drawBox(110.0f, 225.0f, 150.0f);
  drawBox(60.0f, 250.0f, 260.0f);
}

/**
 * Animates the two snowflakes to make them move upward or downward using frame based animation.
 * The animation alternates between even and odd frames, drawing different snowflake patterns.
 * 
 * @param val Integer Used to determine which time event to envoke.
 */
/**
 * Timer function to animate both snowflakes.
 */
void timerHandler(int val) {
  if (isPaused) {
    // Skip the animation updates if paused
    return;
  } 

  // Move first snowflake
  if (centerY > 20) { 
      centerY -= SNOWFLAKE_MOVE_DOWN;  // Moves down
      centerX += SNOWFLAKE_MOVE_RIGHT;  // Moves right
  } else {
      centerY = 578; // Reset to top
      centerX = 15;
  }

  // Activate second snowflake when first is halfway
  if (centerY < 300) {
      secondSnowflakeActive = true;
  }

  // Move second snowflake
  if (secondSnowflakeActive) {
      if (centerYSecond > 20) {
          centerYSecond -= SNOWFLAKE_MOVE_DOWN;
          centerXSecond += SNOWFLAKE_MOVE_RIGHT;
      } else {
          centerYSecond = 578;
          centerXSecond = 100 + 15;
          secondSnowflakeActive = false; 
      }
  }

  glutPostRedisplay();
  glutTimerFunc(66, timerHandler, 1);
}

/**
 * Mouse click handler to start snowflake animation.
 */
void mouseHandler(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (!animationStarted) {
        animationStarted = true;  // Hide the message on first click.
        glutPostRedisplay();  // Force a redraw to remove the message.
    }
    glutTimerFunc(66, timerHandler, 1);
  }
}

void resumeAnimation(int val) {
  isPaused = false;
  glutTimerFunc(66, timerHandler, 1);
}

void keyboardHandler(unsigned char key, int x, int y) {
  if ((key == 'b' || key == 'B') && snowBlowerRemainingCount > 0) {
      // Displace snowflakes (if within the vertical range 200 to 400) rightward by 4 units.
      if (centerY >= 200 && centerY <= 400) {
          centerX += 4.0f;
      }
      if (secondSnowflakeActive && centerYSecond >= 200 && centerYSecond <= 400) {
          centerXSecond += 4.0f;
      }
      snowBlowerRemainingCount--;  // Decrement the activations counter.
      glutPostRedisplay();
  }

  // Pause the animation if 'p' is pressed
  if (key == 'p' || key == 'P') {
    if (!isPaused) {
        isPaused = true;
        glutTimerFunc(1000, resumeAnimation, 0);  // Resume after 1 second
    }
  }
}

void initDisplayList() {
  // Create display list for snowman
  snowmanListIdx = glGenLists(1);
  glNewList(snowmanListIdx, GL_COMPILE);
    drawSnowman();
  glEndList();

  // Create a display list for the startup message.
  startMessageListIdx = glGenLists(2);
  glNewList(startMessageListIdx, GL_COMPILE);
      glRasterPos2i(180, 300);
      const char* startMsg = "Any Mouse Click Will Start";
      for (const char* c = startMsg; *c != '\0'; c++) {
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
      }
  glEndList();

  remainingMessageListIdx = glGenLists(3);  
  glNewList(remainingMessageListIdx, GL_COMPILE);
    glRasterPos2i(450, 580);
    const char* remainingMsg = "Remaining ";
    for (const char* c = remainingMsg; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
  glEndList();
}

/**
* The display handler sets up the display and draws the initial scene with a snowman and two snowflakes.
* The background is cleared to black, and the objects are rendered in white by default.
*/
void displayHandler()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glCallList(snowmanListIdx);

  if (!animationStarted) {
      glCallList(startMessageListIdx);
  }

  drawSnowFlake(centerX, centerY);
  if (secondSnowflakeActive) {
    drawSnowFlake(centerXSecond, centerYSecond);
  }

  // Call display list for static text
  glCallList(remainingMessageListIdx);

  glRasterPos2i(540, 580);
  char numStr[20]; 
  snprintf(numStr, sizeof(numStr), "%d", snowBlowerRemainingCount);
  for (int i = 0; i < strlen(numStr); i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, numStr[i]);
  }

  glutSwapBuffers(); 
}

#define canvas_Width 600
#define canvas_Height 600

char canvas_Name[] = "Snowblower Simulation";

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  initDisplayList();

  glutDisplayFunc(displayHandler);
  glutMouseFunc(mouseHandler);
  glutKeyboardFunc(keyboardHandler);

  glutMainLoop(); 
  return 0;
}