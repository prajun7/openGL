/**
 * Graphics Pgm 2 for Prajun Trital
 * 
 * EXTRA CREDIT
 * - The snowman has a carrot nose and stick arms.
 * - Snowblower is limited to 20 uses.
 * - The animation pauses for 1 second when the 'p' key is pressed.
 * 
 * ARCHITECTURE
 * GLUT event-driven generation of a canvas with a 3 boxed snowman, animated snowflakes and a snowblower.
 * Canvas is produced via the display event handler, which is in `displayHandler`, which calls the series of 
 * display lists to draw text on the screen using `GLUT_BITMAP` and draws the snowman using the `drawSnowman` 
 * function. The `timerHandler` updates the position of the snowflakes and uses 66 ms as the time interval
 * to achieve an animation running at approximately 16 Frame per second(FPS). The `keyboardHandler` detects 
 * the keyboard press to activate the snowblower, while the`mouseHandler` detects the mouse click to start 
 * the animation. The `initDisplayLists` initiates the four displayLists to record a set of drawing commands 
 * for snowman, and three different messages. The `initDisplayLists` is called in the main function.
 * Compared to Program 1, I no longer use a `clearSnowFlake` method to clear the snowflakes, and I do not 
 * draw the snowman or snowflakes in the `timerHandler`. Instead, I use `glutPostRedisplay()` to request a new draw.
 * Additionally, I changed `glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)` to `glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)`
 * in the `OpenGL445Setup-2025.h` file to enable double buffering, which effectively eliminates screen 
 * flickering in static content.
 * 
 * EXTRA CREDIT ARCHITECTURE
 * The `drawSnowman` function renders a snowman with a carrot nose and stick arms. The 'snowBlowerRemainingCount'
 * variable tracks the remaining number of times the snowblower can be used. The `keyboardHandler` updates
 * the `snowBlowerRemainingCount`, and the `displayHandler` displays the reamining count. The `isPaused` boolean
 * tracks whether the animation is paused. The `keyboardHandler` sets `isPaused` to true and triggers 
 * a `resumeAnimation` functtion that runs after 1 seconds, setting `isPaused` back to false. The `timerHandler` 
 * exits early without updating the x and y coordinates of snowflakes when `isPaused` is true.
 */

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>
#include "OpenGL445Setup-2025.h"
#include <string>

const float SNOWFLAKE_MOVE_DOWN = 10.0f;
const float SNOWFLAKE_MOVE_RIGHT = 2.0f;

const float SNOWFLAKE_LENGTH = 60.0f;

// Y-cordinate of the initial left most snowflake.
float centerY = 578.0f;

// Y-cordinate of the second snowflake.
float centerYSecond = 578.0f;

// To calculate the distance from the leftmost diagonal of the snowflake, 
// I need to add the horizontal distance based on the cosine of 60 degrees, 
// i.e., centerX = ((60 / 2) * cos(60))
float centerX = 15.0f;

// Other snowflakes start with the left tips 100 units from the left edge screen.
float centerXSecond = 100.0f + 15.0f;

// Display list idx for "Any Mouse Click Will Start" message.
GLuint startMessageListIdx;

// Display list idx for "Remaining" text.
GLuint remainingMessageListIdx;

// Display list idx for "Press p to pause, once started" message.
GLuint pauseMessageListIdx;

// Display list idx to draw snowman.
GLuint snowmanListIdx;

// Tracks if the animation started or not. Used in `mouseHandler` and `displayHandler`.
bool animationStarted = false;

// Tracks if the second snowflake is active.
bool secondSnowflakeActive = false;

// Tracks if the animation is paused or not. Used in `keyboardHandler` and `TimerHandler`
bool isPaused = false; 

// Maximum number of times the snowblower can be activated.
int snowBlowerRemainingCount = 20;

/**
* Draws a snowflake with a horizontal main line with diagonal lines at ±60 degrees 
* relative to the horizontal axis.
* 
* @param centerX x-coordinate of the center point of the snowflake.
* @param centerY y-coordinate of the center point of the snowflake.
*/
void drawSnowFlake(float centerX, float centerY) {
    glColor3f(1.0f, 1.0f, 1.0f);  

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
* We start with bottom-left -> bottom-right -> top-right and top-left.
* 
* @param length The length of the box
* @param x_start The x-coordinate of the bottonm left
* @param y_start The y-coordinate of the botton left
*/
void drawBox(float length, float x_start, float y_start) {
  glColor3f(1.0f, 1.0f, 1.0f); 
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
* Draws the 3-box snowman with 2 arms, 2 fingers and an orange nose.
*/
void drawSnowman() {
  drawBox(150.0f, 205.0f, 0.0f);
  drawBox(110.0f, 225.0f, 150.0f);
  drawBox(60.0f, 250.0f, 260.0f);

  glBegin(GL_LINES);
    // Left Arm (diagonal upward-left)
    glVertex3f(225.0f, 205.0f, 0.0f);  
    glVertex3f(197.0f, 233.0f, 0.0f); 

    glVertex3f(197.0f, 233.0f, 0.0f);
    glVertex3f(190.0f, 243.0f, 0.0f); 

    glVertex3f(197.0f, 233.0f, 0.0f);
    glVertex3f(189.0f, 225.0f, 0.0f);  

    // Right Arm 
    glVertex3f(335.0f, 205.0f, 0.0f);  
    glVertex3f(363.0f, 233.0f, 0.0f);  

    glVertex3f(363.0f, 233.0f, 0.0f);
    glVertex3f(370.0f, 243.0f, 0.0f);  

    glVertex3f(363.0f, 233.0f, 0.0f);
    glVertex3f(371.0f, 225.0f, 0.0f); 
  glEnd();

  // Nose
  glColor3f(1.0f, 0.5f, 0.0f); 
    glBegin(GL_LINES);
    glVertex3f(242.0f, 290.0f, 0.0f);  
    glVertex3f(257.0f, 294.0f, 0.0f);  

    glVertex3f(242.0f, 290.0f, 0.0f);
    glVertex3f(257.0f, 286.0f, 0.0f);
  glEnd();

  // set color back to white
  glColor3f(1.0f, 1.0f, 1.0f);
}

/**
 * Timer Handler animates the snowflakes to move downward amd rightward. 
 * It adds a second snowflake when the first one reaches halfway. 
 * When the animation is paused it returns without updating the x and y coordinates
 * of the snowflakes.
 * 
 * @param val Integer Used to determine which time event to envoke.
 */
void timerHandler(int val) {
  if (isPaused) {
    // Skips the animation updates if paused
    return;
  } 

  // Moves first snowflake
  if (centerY > 20.0f) { 
      centerY -= SNOWFLAKE_MOVE_DOWN;  
      centerX += SNOWFLAKE_MOVE_RIGHT;
  } else {
      centerY = 578.0f; // Resets to top
      centerX = 15.0f;
  }

  // Activates second snowflake when first is halfway
  if (centerY < 300.0f) {
      secondSnowflakeActive = true;
  }

  // Moves second snowflake
  if (secondSnowflakeActive) {
      if (centerYSecond > 20.0f) {
          centerYSecond -= SNOWFLAKE_MOVE_DOWN;
          centerXSecond += SNOWFLAKE_MOVE_RIGHT;
      } else {
          centerYSecond = 578.0f;
          centerXSecond = 100.0f + 15.0f;
          secondSnowflakeActive = false; 
      }
  }

  glutPostRedisplay();
  glutTimerFunc(66, timerHandler, 1);
}

/**
 * Mouse click handler triggers the animation using glutTimerFunc.
 */
void mouseHandler(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (!animationStarted) {
        animationStarted = true; 

        // Should be inside the if-block so that the multiple mouse click
        // won't keep triggering the timerHandler.
        glutTimerFunc(66, timerHandler, 1);
      }
  }
}

/**
 * Method to resumeAnimation once it is paused.
 * Used in `keyboardHandler`.
 */
void resumeAnimation(int val) {
  isPaused = false;
  glutTimerFunc(66, timerHandler, 1);
}

/**
 * Keyboard Handler triggers the snow blower when the key `b` is pressed.
 * It also, triggers the pause when the key `p` is pressed. 
 */
void keyboardHandler(unsigned char key, int x, int y) {
  if ((key == 'b' || key == 'B') && snowBlowerRemainingCount > 0) {
      // Displace snowflakes (if within the vertical range 200 to 400) rightward by 4 units.
      if (centerY >= 200.0f && centerY <= 400.0f) {
          centerX += 4.0f;
      }
      if (secondSnowflakeActive && centerYSecond >= 200.0f && centerYSecond <= 400.0f) {
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

/**
 * Method to init the DisplayLists.
 * Used in main function.
 */
void initDisplayLists() {
  // Display list for snowman.
  snowmanListIdx = glGenLists(1);
  glNewList(snowmanListIdx, GL_COMPILE);
    drawSnowman();
  glEndList();

  // Display list for the startup message.
  startMessageListIdx = glGenLists(2);
  glNewList(startMessageListIdx, GL_COMPILE);
    glRasterPos2i(180, 300);
    const char* startMsg = "Any Mouse Click Will Start";
    for (const char* c = startMsg; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
  glEndList();

  // Display list for text remaining.
  remainingMessageListIdx = glGenLists(3);  
  glNewList(remainingMessageListIdx, GL_COMPILE);
    glRasterPos2i(450, 580);
    const char* remainingMsg = "Remaining ";
    for (const char* c = remainingMsg; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
  glEndList();

  // Display list for pause message.
  pauseMessageListIdx = glGenLists(4);
  glNewList(pauseMessageListIdx, GL_COMPILE);
    glRasterPos2i(160, 280);
    const char* pauseMsg = "Press p to pause, once started.";
    for (const char* c = pauseMsg; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
  glEndList();  
}

/**
* The display handler sets up the display and renders the initial scene with a snowman and a snowflakes.alignas
* It executed the series of display lists to render the texts and snowman.
* The background is cleared to dark grey, and the objects are rendered in white by default.
*/
void displayHandler()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glCallList(snowmanListIdx);

  if (!animationStarted) {
      glCallList(startMessageListIdx);
      glCallList(pauseMessageListIdx);
  }

  drawSnowFlake(centerX, centerY);
  if (secondSnowflakeActive) {
    drawSnowFlake(centerXSecond, centerYSecond);
  }

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

  initDisplayLists();

  glutDisplayFunc(displayHandler);
  glutMouseFunc(mouseHandler);
  glutKeyboardFunc(keyboardHandler);

  glutMainLoop(); 
  return 0;
}