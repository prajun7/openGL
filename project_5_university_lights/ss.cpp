/** 
 *  EXTRA CREDIT: Scene shifts (U/N keys)  
 *               and light control (K/L keys)  
 *  
 *  ARCHITECTURE OVERVIEW  
 *  - Renders spinning UAH letters using OpenGL  
 *  - U, H are solid cubes; A is wireframe  
 *  - Red sphere sits in A's center hole  
 *  - All objects rotate about a gray spindle tip  
 *  - Perspective view with Phong lighting  
 *  
 *  DESIGN & METHODOLOGY  
 *  - Display lists used for U, A, H, spindle  
 *  - 10 cubes per letter (50x50x50 units each)  
 *  - Spindle is one triangle (base 50, height 75)  
 *  - Material settings define color and shine  
 *    - U/H: UAH blue, shiny  
 *    - A: grey wireframes  
 *    - Sphere: candy apple red  
 *    - Spindle: dull gray  
 *  
 *  TRANSFORMATIONS  
 *  - Scene rotates about point (10, 0, -400)  
 *  - Scene can shift up/down (U/N keys)  
 *  
 *  LIGHTING & SHADING  
 *  - Light/viewer at origin  
 *  - Phong illumination, no glColor calls  
 *  - Real-time light strength control (K/L keys)  
 *  - Smooth shading, front faces only  
 *  
 *  CODE ROADMAP  
 *  - main(): setup and GLUT config  
 *  - display_func(): draws scene each frame  
 *    ^ HANDLER for glutDisplayFunc()  
 *  - timer_func(): rotates letters smoothly  
 *    ^ HANDLER for glutTimerFunc()  
 *  - keyboard_func(): input for quit, pause,  
 *    scene shift, and light control  
 *    ^ HANDLER for glutKeyboardFunc()  
 *  - createDisplayLists(): builds all geometry  
 *    (U, A, H letters and spindle)  
 */


//  g++ ss.cpp -o ss.exe -Wall   -I/opt/homebrew/opt/glew/include   -I/opt/homebrew/opt/freeglut/include   -L/opt/homebrew/opt/glew/lib   -L/opt/homebrew/opt/freeglut/lib   -framework OpenGL -framework GLUT   -lglew -lglut

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include "OpenGL445Setup-2025.h"

const int CANVAS_WIDTH = 800;
const int CANVAS_HEIGHT = 800;
char canvas_Name[] = "UAH Phong Illumination";

// Global helpers
float rotationAngle = 0.0f;
float sceneYOffset = 0.0f;
float lightIntensity = 1.0f;
bool pauseAnimation = false;
int TIMER_INTERVAL = 25;

// Display lists
GLuint letterUList, letterAList, letterHList, spindleList;

// Creates a display list for the letter U using 10 solid cubes (GLUT solid primitives).
// Letter U is colored in UAH blue with specular highlights to emphasize Phong illumination.
// Cube size: 50x50x50. Layout: two vertical columns (4 cubes each) and 2 bottom connectors.
void createLetterUList() {
    // Generate a unique display list ID and start compiling the list
    letterUList = glGenLists(1);
    glNewList(letterUList, GL_COMPILE);

    // Set material properties: UAH blue with moderate shininess for specular reflection
    GLfloat mat_ambient[]  = {0.0f, 0.2f, 0.5f, 1.0f};   // Slight ambient glow, not overpowering
	GLfloat mat_diffuse[]  = {0.0f, 0.498f, 1.0f, 1.0f}; // Matches #007fff
	GLfloat mat_specular[] = {0.9f, 0.9f, 0.9f, 1.0f};   // highly significant specularity
	GLfloat shininess[]    = {55.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // Left vertical column: 4 solid cubes placed along Y-axis
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(0, 25 + i * 50, 0); // Centers each cube
        glutSolidCube(50);
        glPopMatrix();
    }

    // Right vertical column: 4 solid cubes, offset 150 units in X
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(150, 25 + i * 50, 0);
        glutSolidCube(50);
        glPopMatrix();
    }

    // Bottom horizontal connectors: 2 cubes at base connecting the legs
    glPushMatrix(); glTranslatef(50, 25, 0); glutSolidCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(100, 25, 0); glutSolidCube(50); glPopMatrix();

    glEndList(); // Finish compiling display list
}

// Creates a display list for the letter H using 10 solid cubes (GLUT solid primitives).
// Letter H is colored in UAH blue with specular highlights, matching the U.
// Structure: two vertical columns of 4 cubes each, connected by 2 central horizontal cubes.
void createLetterHList() {
    // Generate and start compiling display list for letter H
    letterHList = glGenLists(1);
    glNewList(letterHList, GL_COMPILE);

     // Set material properties: UAH blue with moderate shininess for specular reflection
    GLfloat mat_ambient[]  = {0.0f, 0.2f, 0.5f, 1.0f};   // Slight ambient glow, not overpowering
	GLfloat mat_diffuse[]  = {0.0f, 0.498f, 1.0f, 1.0f}; // Matches #007fff
	GLfloat mat_specular[] = {0.9f, 0.9f, 0.9f, 1.0f};   // highly significant specularity
	GLfloat shininess[]    = {55.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // Left vertical column: 4 stacked cubes
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(0, 25 + i * 50, 0);
        glutSolidCube(50);
        glPopMatrix();
    }

    // Right vertical column: 4 stacked cubes, 150 units right
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(150, 25 + i * 50, 0);
        glutSolidCube(50);
        glPopMatrix();
    }

    // Center bridge: 2 horizontal cubes centered at vertical midpoint (y = 100)
    glPushMatrix(); glTranslatef(50, 100, 0); glutSolidCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(100, 100, 0); glutSolidCube(50); glPopMatrix();

    glEndList(); // End display list definition
}
// Creates a display list for the letter A using 10 wireframe cubes (GLUT wire primitives).
// A is rendered in medium silver/grey with high specular reflection.
// The cubes form two legs, a mid crossbar, and a top row with a center hole.
// A red sphere (added later) will be placed in the central hole.
void createLetterAList() {
    // Generate and begin compiling display list for letter A
    letterAList = glGenLists(1);
    glNewList(letterAList, GL_COMPILE);

    // Set material properties: medium grey, almost silver with strong specular highlights
   	GLfloat mat_ambient[]  = {0.4f, 0.4f, 0.4f, 1.0f};    // Soft base ambient
	GLfloat mat_diffuse[]  = {0.827f, 0.827f, 0.827f, 1.0f}; // Matches #d3d3d3
    GLfloat mat_specular[] = {0.9f, 0.9f, 0.9f, 1.0f}; // highly significant specularity
    GLfloat shininess[]    = {80.0f}; // High shininess for strong specular reflection

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // Bottom row (feet): 2 cubes at x = 0 and x = 200
    glPushMatrix(); glTranslatef(0, 25, 0); glutWireCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(200, 25, 0); glutWireCube(50); glPopMatrix();

    // Row above feet: 4 cubes forming the middle horizontal crossbar
    glPushMatrix(); glTranslatef(25, 75, 0); glutWireCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(75, 75, 0); glutWireCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(125, 75, 0); glutWireCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(175, 75, 0); glutWireCube(50); glPopMatrix();

    // Diagonal legs connecting crossbar to top: 2 cubes offset inward
    glPushMatrix(); glTranslatef(50, 125, 0); glutWireCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(150, 125, 0); glutWireCube(50); glPopMatrix();

    // Top row: 2 cubes with a 50-unit gap between (centered at 100)
    glPushMatrix(); glTranslatef(75, 175, 0); glutWireCube(50); glPopMatrix();
    glPushMatrix(); glTranslatef(125, 175, 0); glutWireCube(50); glPopMatrix();

    glEndList(); // Complete display list for letter A
}

// Creates a display list for the spindle, modeled as a triangle (base 50, height 75).
// The spindle is attached to the top of the A and acts as the pivot point for rotation.
// It is rendered in dull gray with low specular reflection and faces perpendicular to the Z-axis.
void createSpindleList() {
    // Generate and compile the display list for the spindle
    spindleList = glGenLists(1);
    glNewList(spindleList, GL_COMPILE);

    // Set dull gray material for spindle (low reflectance)
    GLfloat mat_ambient[]  = {0.4f, 0.4f, 0.4f, 1.0f}; // mostly lit by ambient
    GLfloat mat_diffuse[]  = {0.5f, 0.5f, 0.5f, 1.0f}; // strong dull gray
    GLfloat mat_specular[] = {0.1f, 0.1f, 0.1f, 1.0f}; // low specularity
    GLfloat shininess[]    = {10.0f}; // barely affected by lighting

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // Draw triangle (face lies in the XY plane, perpendicular to Z-axis)
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);       // Tip (bottom center)
        glVertex3f(-25.0f, 75.0f, 0.0f);    // Top left
        glVertex3f(25.0f, 75.0f, 0.0f);     // Top right
    glEnd();

    glEndList(); // Finish spindle display list
}

// Initializes all display lists used in the scene.
// Each letter (U, A, H) and the spindle are compiled into their own display lists
// to improve rendering efficiency and scene organization.
void createDisplayLists() {
    createLetterUList();   // Builds display list for letter U (solid cubes)
    createLetterAList();   // Builds display list for letter A (wireframe cubes)
    createLetterHList();   // Builds display list for letter H (solid cubes)
    createSpindleList();   // Builds display list for spindle (gray triangle)
}

// Main display callback function.
// Clears the screen, draws the spindle and rotating UAH letters with lighting and transformations applied.
// Includes rendering of the red sphere inside the A. Uses perspective projection and smooth shading.
void display_func() {
    // Clear background (black) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply any vertical offset from user input (U/N keys)
	glTranslatef(0, sceneYOffset, 0);

    // Draw spindle (stationary triangle at top of A)
    glPushMatrix();
        glTranslatef(10, 0, -400);  // Spindle positioned at (10, 0, -400)
        glCallList(spindleList);
    glPopMatrix();

    // Apply rotation transformation for UAH letters + sphere
    glPushMatrix();
        glTranslatef(10, 0, -425);           // Move to pivot point
        glRotatef(rotationAngle, 0, 1, 0);   // Rotate around Y-axis
        glTranslatef(-10, 0, 425);           // Move back to original frame

        // Draw letter U on the left
        glPushMatrix();
            glTranslatef(-340, -200, -425);  // Final position of U, offset by -750/2+10+25
            glCallList(letterUList);
        glPopMatrix();

        // Draw letter A in center
        glPushMatrix();
            glTranslatef(-90, -200, -425);   // Final position of A -750/2+10+25+250
            glCallList(letterAList);

            GLfloat amb[]  = {0.5f, 0.015f, 0.0f, 1.0f}; // Half strength of main color for subtle ambient glow
			GLfloat diff[] = {1.0f, 0.031f, 0.0f, 1.0f}; // Full #ff0800 color
            GLfloat spec[] = {0.6f, 0.6f, 0.6f, 1.0f};
			GLfloat shin[] = {40.0f}; // Strong specular highlight

            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
            glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
            glMaterialfv(GL_FRONT, GL_SHININESS, shin);

            // Draw the sphere inside A hole (diameter 50, centered at 100, 125, 0)
            glPushMatrix();
                glTranslatef(100, 125, 0);
                glutSolidSphere(25, 20, 20);
            glPopMatrix();
        glPopMatrix();

        // Draw letter H on the right
        glPushMatrix();
            glTranslatef(210, -200, -425);  // Final position of H -750/2+10+25+250+300
            glCallList(letterHList);
        glPopMatrix();

    glPopMatrix(); // End of rotating group

    glutSwapBuffers(); // Present the rendered frame (double buffering)
}

// Timer callback function for frame-by-frame animation.
// Increases the rotation angle and requests a redisplay every TIMER_INTERVAL ms.
// Rotation is paused/resumed using the P key.
void timer_func(int value) {
    // If not paused, increment the rotation angle
    if (!pauseAnimation) {
        rotationAngle += 4.0f;
        if (rotationAngle >= 360.0f)
            rotationAngle -= 360.0f; // Keep angle in [0, 360) range
    }

    glutPostRedisplay(); // Request screen redraw
    glutTimerFunc(TIMER_INTERVAL, timer_func, 0); // Re-register timer
}

// Handles keyboard input for quitting, pausing animation, scene shifting, and light control.
// EXTRA CREDIT: Scene shifts with U/N keys and light intensity control with K/L keys.
void keyboard_func(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': case 'Q':
            exit(0); // Quit the program
            break;
        case 'p': case 'P':
            pauseAnimation = !pauseAnimation; // Toggle pause/resume animation
            break;
        case 'u': case 'U':
            sceneYOffset += 25.0f; // Move scene up by 25 units (EXTRA CREDIT)
            break;
        case 'n': case 'N':
            sceneYOffset -= 25.0f; // Move scene down by 25 units (EXTRA CREDIT)
            break;
        case 'k': case 'K':
            lightIntensity += 0.1f; // Increase light intensity (EXTRA CREDIT)
            break;
        case 'l': case 'L':
            lightIntensity -= 0.1f; // Decrease light intensity (EXTRA CREDIT)
            break;
    }

    // Clamp light intensity to valid range [0.1, 2.0]
    if (lightIntensity < 0.1f) lightIntensity = 0.1f;
    if (lightIntensity > 2.0f) lightIntensity = 2.0f;

    // Update light parameters based on current intensity
    GLfloat amb[]  = {0.2f * lightIntensity, 0.2f * lightIntensity, 0.2f * lightIntensity, 1.0f};
    GLfloat diff[] = {0.8f * lightIntensity, 0.8f * lightIntensity, 0.8f * lightIntensity, 1.0f};
    GLfloat spec[] = {1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
}

// Main entry point: initializes OpenGL, sets up lighting, shading, and display lists.
// Launches the main event loop using GLUT.
int main(int argc, char **argv) {
    // Initialize GLUT and create window
    glutInit(&argc, argv);
    my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, canvas_Name); // setup from header

    // Set light position at origin, pointing down negative Z
    GLfloat light_dir[] = {0.0f, 0.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

	// Ambient light — simulates background/global lighting
	GLfloat amb[]  = {0.2f, 0.2f, 0.2f, 1.0f};
	// Diffuse light — simulates direct illumination from the light
	GLfloat diff[] = {0.8f, 0.8f, 0.8f, 1.0f};
	// Specular light — controls how intense highlights are
	GLfloat spec[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    // Enable required OpenGL features
    glEnable(GL_DEPTH_TEST);               // Enable Z-buffering
    glEnable(GL_LIGHTING);                 // Enable lighting
    glEnable(GL_LIGHT0);                   // Enable light source 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Ensure solid cube rendering
    glShadeModel(GL_SMOOTH);               // Use smooth shading

    // Compile display lists for letters and spindle
    createDisplayLists();

    // Register callback functions
    glutDisplayFunc(display_func);              // Rendering function
    glutKeyboardFunc(keyboard_func);            // Keyboard handler
    glutTimerFunc(TIMER_INTERVAL, timer_func, 0); // Animation loop

    // Enter main event loop
    glutMainLoop();

    return 0;
}