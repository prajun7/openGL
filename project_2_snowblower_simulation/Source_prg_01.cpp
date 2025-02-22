/**
 * Graphics Pgm 2 for Prajun Trital
 * 
 * EXTRA CREDIT
 * - The snowfalke changes its orientation based on the frame count - odd frame and even frame.
 * - When the two snowflake reaches the bottom of the canvas, the user is prompted to restart 
 *      the animation of snowflakes. When the user presses any key, it moves the two gray snowflakes upwards.
 * - Second snowflake is added on the right side of the screens.
 * 
 * ARCHITECTURE
 * GLUT event-driven generation of a canvas with a 3 boxed snowman, animated snowflakes and a snowblower.
 * Canvas is produced via the display event handler, which is in display_func. 
 * The `display_func` calls the `drawSnowman` function to draw the 3 boxed snowman. It also calls the
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

// Lenght of the snowflake.
const float length = 60.0f;

// Y-cordinate of the initial snowflake.
float centerY = 584.0f;

// Determines if the snowflake should move upward or downward.
bool moveUpward = false;

// Tracks the frame count and draws different orientation of snowflake.
int frameCount = 0;

// My last name starts with T, so I need to take 70. 
// To calculate the distance from the leftmost diagonal of the snowflake, 
// I need to add the horizontal distance based on the 
// sine of 60 degrees (sin(60) = opposite/hypotenuse).
// so, centerX = 70 + ((60 / 2) * sin(60))
const float centerX = 70 + 25.98;

// For the second flare on the right side
// centerX_second = 530 - ((60 / 2) * sin(60))
const float centerX_second = 530 - 25.98;

/**
 * Draws a snowflake with a vertical main line with diagonal lines at ±60 degrees 
 * relative to the vertical axis.
 * 
 * Computes the vertical and horizontal distance using, 
 * ((60 / 2) * sin(60)) = 25.98
 * ((60 / 2) * cos(60)) = 15
 * 
 * @param centerX X-coordinate of the center point of the snowflake.
 */
void drawOddSnowFlake(float centerX) {
    // Vertical main line
    glVertex3f(centerX, centerY - length / 2, 0.0f);
    glVertex3f(centerX, centerY + length / 2, 0.0f);

    // Diagonal lines at ±60 degrees from the vertical axis
    glVertex3f(centerX - 25.98f, centerY + 15.0f, 0.0f);
    glVertex3f(centerX + 25.98f, centerY - 15.0f, 0.0f);

    glVertex3f(centerX - 25.98f, centerY - 15.0f, 0.0f);
    glVertex3f(centerX + 25.98f, centerY + 15.0f, 0.0f);
}

/**
 * Draws a snowflake with a horizontal main line with diagonal lines at ±60 degrees 
 * relative to the horizontal axis
 * 
 * @param centerX x-coordinate of the center point of the snowflake.
 */
void drawEvenSnowFlake(float centerX) {
    // Horizontal main line
    glVertex3f(centerX - length / 2, centerY, 0.0f);
    glVertex3f(centerX + length / 2, centerY, 0.0f);

    // Diagonal lines at ±60 degrees from the horizontal axis
    glVertex3f(centerX - 15.0f, centerY - 25.98f, 0.0f);
    glVertex3f(centerX + 15.0f, centerY + 25.98f, 0.0f);

    glVertex3f(centerX - 15.0f, centerY + 25.98f, 0.0f);
    glVertex3f(centerX + 15.0f, centerY - 25.98f, 0.0f);
}

/**
 * Draws a snowflake at a (centerX, centerY) position.
 * The shape and appearance of the snowflake depend on whether it is an 
 * "even frame" or an "odd frame."
 * The snowflake is constructed using 4 right-angled triangles by calculating 
 * additional points
 * using sine and cosine functions to determine their lengths.
 * 
 * @param centerX x-coordinate of the center of the snowflake.
 * @param isEvenFrame A boolean indicating the style of the snowflake:
 *                    - `true` for horizontal base with diagonal lines
 *                    - `false` for vertical base with diagonal lines
 */
void drawSnowFlake(float centerX, bool isEvenFrame) {
    if (moveUpward) {
        glColor3f(0.5f, 0.5f, 0.5f);  // Gray if snowflake is moving upward
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);   // White if snowfalke is moving downward
    }

    glBegin(GL_LINES);

    if (isEvenFrame) {
        drawEvenSnowFlake(centerX);
    } else {
        drawOddSnowFlake(centerX);
    }

    glEnd();
    glFlush();
}

/**
 * Draws a box using series of lines.
 * We start with bottom-left -> bottom-right -> top-right and top-left, 
 * similar to how the GL_POLYGON draws.
 * 
 * @param length The length of the box
 * @param x_start The x-coordinate of the box
 * @param y_start The y-coordinate of the box
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
    drawBox(150.0f, 320.0f, 0.0f);
    drawBox(110.0f, 340.0f, 150.0f);
    drawBox(60.0f, 365.0f, 260.0f);
}

/**
 * I initially used glClear(GL_COLOR_BUFFER_BIT) in the timer_function, 
 * but it clears the entire screen, including the box. 
 * Redrawing the box would fix this, but it flickers the screen.
 * 
 * So, I am drawing a black rectangle to cover the previous snowflake.
 * 
 * @param centerX The x-coordinate of the center of the snowflake.
 * @param centerY The y-coordinate of the center of the snowflake.
 */
void clearSnowFlake(float centerX, float centerY)
{
  glColor3f(0.0, 0.0, 0.0); 
  glBegin(GL_POLYGON);
      glVertex3f(centerX - (length / 2), centerY - (length / 2), 0.0f);
      glVertex3f(centerX + (length / 2), centerY - (length / 2), 0.0f);
      glVertex3f(centerX + (length / 2), centerY + (length / 2), 0.0f);
      glVertex3f(centerX - (length / 2), centerY + (length / 2), 0.0f);
  glEnd();
}

/**
 * Animates the two snowflakes to make them move upward or downward using frame based animation.
 * The animation alternates between even and odd frames, drawing different snowflake patterns.
 * 
 * - If `moveUpward` is true, the snowflakes move upward until `centerY` reaches 584.
 * - If `moveUpward` is false, the snowflakes move downward until `centerY` reaches 20.
 * 
 * @param val Integer Used to determine which time event to envoke.
 */
void timer_function(int val)
{
    clearSnowFlake(centerX, centerY);
    clearSnowFlake(centerX_second, centerY);

    frameCount++;

    if (moveUpward) {
        if (centerY < 584) {
            centerY += 15;  
            glutTimerFunc(66, timer_function, 1); 
        } else {            
            // When the snowflakes reach the top, the frame count is adjusted to end on an odd frame
            if (frameCount % 2 == 0) {
                frameCount++;
            }
        }
    } else {
        if (centerY > 20) { 
            centerY -= 15;  
            glutTimerFunc(66, timer_function, 1); 
        } else {
            printf("Press any key to move the snowflakes upward\n"); 
        }
    }

    bool isEvenFrame = (frameCount % 2 == 0);
    drawSnowFlake(centerX, isEvenFrame);
    drawSnowFlake(centerX_second, isEvenFrame);
    glFlush();
}

/**
 * The keyboard handler triggers the animation using `glutTimerFunc`.
 * It also sets the moveUpward to True once the snowflake reaches the bottom of the screen
 * 
 * @param key Key pressed by the user.
 * @param x X-coordinate of mouse when the key is pressed
 * @param y Y-coordinate of mouse when the key is pressed
 */
void keyboard_func(unsigned char key, int x, int y)
{
    if (centerY <= 20) {
        moveUpward = true; 
    }
    glutTimerFunc(66, timer_function, 1); 
}

/**
 * Sets up the display and draws the initial scene with a snowman and two snowflakes.
 * The background is cleared to black, and the objects are rendered in white by default.
 */
void display_func()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
        
    drawSnowman();

    drawSnowFlake(centerX, false);
    drawSnowFlake(centerX_second, false);

    glFlush(); 
}

#define canvas_Width 600
#define canvas_Height 600

char canvas_Name[] = "Wintry Scene";

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    printf("Any Key Click Will Start\n");  

    glutKeyboardFunc(keyboard_func);
    glutDisplayFunc(display_func);

    glutMainLoop(); 
    return 0;
}

