#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

// Constants for window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Constants for grid
#define GRID_COLOR_R 200
#define GRID_COLOR_G 200
#define GRID_COLOR_B 200
#define MAIN_AXIS_COLOR_R 255
#define MAIN_AXIS_COLOR_G 255
#define MAIN_AXIS_COLOR_B 255
#define GRID_SPACING 50

// Constants for input handling
#define MAX_INPUT_LENGTH 100
#define MAX_COEFFICIENTS 10
#define MAX_SHAPE_NAME 20

// Constants for scaling
#define INITIAL_SCALE_X 50
#define INITIAL_SCALE_Y 50
#define STEP_INITIAL 0.0001
#define MIN_SCALE 5
#define MAX_SCALE 100

// Function prototypes
void drawGrid(double gridSpacing);
void drawAxes();
double evaluateFunction(double x, const char* func);
void plotFunction(const char* func, double r, double g, double b);
void plotEllipse(double a, double b);
void plotParabola(double a, double b, double c);
void plotHyperbola(double a, double b);
void plotLine(double m, double c);
void plotFunctions();
void plotActiveShape();
void drawUI();
void handleUserInput(unsigned char key);
void toggleFunction(unsigned char key);
void testEvaluateFunction();

// Global variables
double scaleX = INITIAL_SCALE_X, scaleY = INITIAL_SCALE_Y;
double step = STEP_INITIAL;
double offsetX = 1, offsetY = 1;

bool showSin = true, showCos = true, showTan = false, showExp = false, show_xSquare = false;
bool showGridFlag = true;

bool showTextBox = false;
char userInput[MAX_INPUT_LENGTH] = "";
int inputIndex = 0;
double coefficients[MAX_COEFFICIENTS] = {0};
char activeShape[MAX_SHAPE_NAME] = "";

bool isPanning = false;
int lastMouseX, lastMouseY;

// Function to draw the grid and axes
void drawGrid(double gridSpacing) {
    if (!showGridFlag) return;

    // Draw vertical grid lines
    iSetColor(GRID_COLOR_R, GRID_COLOR_G, GRID_COLOR_B);
    for (double x = -WINDOW_WIDTH / 2; x <= WINDOW_WIDTH / 2; x += gridSpacing) {
        iLine(WINDOW_WIDTH / 2 + x + offsetX * scaleX, 0, WINDOW_WIDTH / 2 + x + offsetX * scaleX, WINDOW_HEIGHT);
    }

    // Draw horizontal grid lines
    for (double y = -WINDOW_HEIGHT / 2; y <= WINDOW_HEIGHT / 2; y += gridSpacing) {
        iLine(0, WINDOW_HEIGHT / 2 + y + offsetY * scaleY, WINDOW_WIDTH, WINDOW_HEIGHT / 2 + y + offsetY * scaleY);
    }

    // Draw main axes
    iSetColor(MAIN_AXIS_COLOR_R, MAIN_AXIS_COLOR_G, MAIN_AXIS_COLOR_B);
    iLine(0, WINDOW_HEIGHT / 2 + offsetY * scaleY, WINDOW_WIDTH, WINDOW_HEIGHT / 2 + offsetY * scaleY); // X-axis
    iLine(WINDOW_WIDTH / 2 + offsetX * scaleX, 0, WINDOW_WIDTH / 2 + offsetX * scaleX, WINDOW_HEIGHT);  // Y-axis
}

void drawAxes() {
    drawGrid(GRID_SPACING);
}

// Function to evaluate mathematical functions
double evaluateFunction(double x, const char* func) {
    if (strcmp(func, "sin(x)") == 0) return sin(x);
    if (strcmp(func, "cos(x)") == 0) return cos(x);
    if (strcmp(func, "tan(x)") == 0) return tan(x);
    if (strcmp(func, "exp(x)") == 0) return exp(x);
    if (strcmp(func, "x^2") == 0) return x * x;
    return 0; // Default for invalid function
}

// Function to plot a generic mathematical function
void plotFunction(const char* func, double r, double g, double b) {
    iSetColor(r, g, b);
    for (double x = -10; x <= 10; x += step) {
        double y1 = evaluateFunction(x, func);
        double y2 = evaluateFunction(x + step, func);

        if (!isfinite(y1) || !isfinite(y2)) {
            continue;
        }
        if (fabs(y1) > 10 || fabs(y2) > 10) {
            continue;
        }

        double screenX1 = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY1 = WINDOW_HEIGHT / 2 + (y1 + offsetY) * scaleY;
        double screenX2 = WINDOW_WIDTH / 2 + (x + step + offsetX) * scaleX;
        double screenY2 = WINDOW_HEIGHT / 2 + (y2 + offsetY) * scaleY;

        iLine(screenX1, screenY1, screenX2, screenY2);
    }
}

// Function to plot an ellipse
void plotEllipse(double a, double b) {
    iSetColor(255, 0, 255); // Purple
    double centerX = WINDOW_WIDTH / 2 + offsetX * scaleX;
    double centerY = WINDOW_HEIGHT / 2 + offsetY * scaleY;
    iEllipse(centerX, centerY, a * scaleX, b * scaleY, 100);
}

// Function to plot a parabola
void plotParabola(double a, double b, double c) {
    iSetColor(0, 255, 255); // Cyan
    for (double x = -10; x <= 10; x += step) {
        double y = a * x * x + b * x + c;
        double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
        iPoint(screenX, screenY);
    }
}

// Function to plot a hyperbola
void plotHyperbola(double a, double b) {
    iSetColor(255, 165, 0); // Orange
    for (double x = -10; x <= -a; x += step) {
        double inner = ((x * x) / (a * a) - 1) * b * b;
        if (inner < 0) continue;
        double y = sqrt(inner);
        double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY1 = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
        double screenY2 = WINDOW_HEIGHT / 2 + (-y + offsetY) * scaleY;
        iPoint(screenX, screenY1);
        iPoint(screenX, screenY2);
    }
    for (double x = a; x <= 10; x += step) {
        double inner = ((x * x) / (a * a) - 1) * b * b;
        if (inner < 0) continue;
        double y = sqrt(inner);
        double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY1 = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
        double screenY2 = WINDOW_HEIGHT / 2 + (-y + offsetY) * scaleY;
        iPoint(screenX, screenY1);
        iPoint(screenX, screenY2);
    }
}

// Function to plot a straight line
void plotLine(double m, double c) {
    iSetColor(0, 255, 0); // Green
    for (double x = -10; x <= 10; x += step) {
        double y = m * x + c;
        double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
        iPoint(screenX, screenY);
    }
}

// Function to plot all toggled functions
void plotFunctions() {
    if (showSin) plotFunction("sin(x)", 0, 0, 255);         // Blue
    if (showCos) plotFunction("cos(x)", 255, 0, 0);         // Red
    if (showTan) plotFunction("tan(x)", 0, 255, 0);         // Green
    if (showExp) plotFunction("exp(x)", 255, 128, 0);       // Orange
    if (show_xSquare) plotFunction("x^2", 128, 0, 255);     // Purple
}

// Function to plot the active shape based on user input
void plotActiveShape() {
    if (strcmp(activeShape, "Ellipse") == 0 && inputIndex >= 2) {
        plotEllipse(coefficients[0], coefficients[1]);
        showTextBox = false;
    } else if (strcmp(activeShape, "Parabola") == 0 && inputIndex >= 3) {
        plotParabola(coefficients[0], coefficients[1], coefficients[2]);
        showTextBox = false;
    } else if (strcmp(activeShape, "Hyperbola") == 0 && inputIndex >= 2) {
        plotHyperbola(coefficients[0], coefficients[1]);
        showTextBox = false;
    } else if (strcmp(activeShape, "StraightLine") == 0 && inputIndex >= 2) {
        plotLine(coefficients[0], coefficients[1]);
        showTextBox = false;
    }
}

// Function to draw the user interface
void drawUI() {
    iSetColor(255, 255, 255); // White text
    iText(10, 10, "Press 1-5 to toggle functions, 6-9 to plot shapes");
    iText(10, 30, "Use '[' and ']' to zoom, arrow keys to pan");
    iText(10, 50, "Press 'g' to toggle grid, 'q' to exit");

    // Zoom level indicator
    char zoomText[50];
    sprintf(zoomText, "Zoom: %.2f", scaleX);
    iText(10, 70, zoomText);

    // Display current input for shapes
    if (showTextBox) {
        iSetColor(200, 200, 200); // Light gray
        iFilledRectangle(300, 500, 200, 30); // Text box
        iSetColor(0, 0, 0); // Black text
        iText(310, 510, userInput); // Show current input

        // Display instructions based on active shape
        if (strcmp(activeShape, "Ellipse") == 0) {
            iSetColor(255, 255, 255);
            iText(10, 110, "Enter coefficients for Ellipse (a, b):");
            char aText[100], bText[100];
            sprintf(aText, "a = %.2f", coefficients[0]);
            sprintf(bText, "b = %.2f", coefficients[1]);
            iText(10, 130, aText);
            iText(10, 150, bText);
        } else if (strcmp(activeShape, "Parabola") == 0) {
            iSetColor(255, 255, 255);
            iText(10, 110, "Enter coefficients for Parabola (a, b, c):");
            char aText[100], bText[100], cText[100];
            sprintf(aText, "a = %.2f", coefficients[0]);
            sprintf(bText, "b = %.2f", coefficients[1]);
            sprintf(cText, "c = %.2f", coefficients[2]);
            iText(10, 130, aText);
            iText(10, 150, bText);
            iText(10, 170, cText);
        } else if (strcmp(activeShape, "Hyperbola") == 0) {
            iSetColor(255, 255, 255);
            iText(10, 110, "Enter coefficients for Hyperbola (a, b):");
            char aText[100], bText[100];
            sprintf(aText, "a = %.2f", coefficients[0]);
            sprintf(bText, "b = %.2f", coefficients[1]);
            iText(10, 130, aText);
            iText(10, 150, bText);
        } else if (strcmp(activeShape, "StraightLine") == 0) {
            iSetColor(255, 255, 255);
            iText(10, 110, "Enter coefficients for Straight Line (m, c):");
            char mText[100], cText[100];
            sprintf(mText, "m = %.2f", coefficients[0]);
            sprintf(cText, "c = %.2f", coefficients[1]);
            iText(10, 130, mText);
            iText(10, 150, cText);
        }
    }
}

// Function to handle user input from keyboard
void handleUserInput(unsigned char key) {
    if (showTextBox) {
        if (isdigit(key) || key == '.' || key == '-' || key == '\r') {
            if (key == '\r') { // Enter key
                coefficients[inputIndex] = atof(userInput);
                inputIndex++;
                userInput[0] = '\0'; // Clear input

                // Check if all required coefficients are entered
                int requiredCoefficients = 0;
                if (strcmp(activeShape, "Ellipse") == 0) requiredCoefficients = 2;
                else if (strcmp(activeShape, "Parabola") == 0) requiredCoefficients = 3;
                else if (strcmp(activeShape, "Hyperbola") == 0) requiredCoefficients = 2;
                else if (strcmp(activeShape, "StraightLine") == 0) requiredCoefficients = 2;

                if (inputIndex >= requiredCoefficients) {
                    plotActiveShape();
                }
            } else if (key == '\b') { // Backspace
                int len = strlen(userInput);
                if (len > 0) {
                    userInput[len - 1] = '\0';
                }
            } else if (key != '\b' && strlen(userInput) < MAX_INPUT_LENGTH - 1) { // Append character
                int len = strlen(userInput);
                userInput[len] = key;
                userInput[len + 1] = '\0';
            }
        }
    } else {
        toggleFunction(key);
    }
}

// Function to toggle function visibility and initiate shape plotting
void toggleFunction(unsigned char key) {
    switch (key) {
        case '1': showSin = !showSin; break;           // Toggle sin(x)
        case '2': showCos = !showCos; break;           // Toggle cos(x)
        case '3': showTan = !showTan; break;           // Toggle tan(x)
        case '4': showExp = !showExp; break;           // Toggle exp(x)
        case '5': show_xSquare = !show_xSquare; break; // Toggle x^2
        case '6': showTextBox = true; strcpy(activeShape, "Ellipse"); inputIndex = 0; break;
        case '7': showTextBox = true; strcpy(activeShape, "Parabola"); inputIndex = 0; break;
        case '8': showTextBox = true; strcpy(activeShape, "Hyperbola"); inputIndex = 0; break;
        case '9': showTextBox = true; strcpy(activeShape, "StraightLine"); inputIndex = 0; break;
        case '[': 
            if (scaleX < MAX_SCALE && scaleY < MAX_SCALE) {
                scaleX *= 1.1; 
                scaleY *= 1.1;
                step /= 1.1; // Increase detail when zooming in
            }
            break;
        case ']':
            if (scaleX > MIN_SCALE && scaleY > MIN_SCALE) {
                scaleX /= 1.1; 
                scaleY /= 1.1;
                step *= 1.1; // Decrease detail when zooming out
            }
            break;
        case 'g':
            showGridFlag = !showGridFlag; // Toggle grid visibility
            break;
        case 'q':
            exit(0); // Exit program
            break;
    }
}

// Function to handle keyboard input
void iKeyboard(unsigned char key) {
    handleUserInput(key);
}

// Function to handle mouse clicks
void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isPanning = true;
        lastMouseX = mx;
        lastMouseY = my;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isPanning = false;
    }
}

// Function to handle passive mouse movement
void iPassiveMouseMove(int mx, int my) {
    // Intentionally left empty
}

// Function to handle active mouse movement
void iMouseMove(int mx, int my) {
    if (isPanning) {
        offsetX += (mx - lastMouseX) / (scaleX * 10);
        offsetY += (my - lastMouseY) / (scaleY * 10);
        lastMouseX = mx;
        lastMouseY = my;
    }
}

// Function to handle special keyboard inputs (arrow keys)
void iSpecialKeyboard(unsigned char key) {
    switch (key) {
        case GLUT_KEY_LEFT:
            offsetX -= 0.1; // Pan left
            break;
        case GLUT_KEY_RIGHT:
            offsetX += 0.1; // Pan right
            break;
        case GLUT_KEY_UP:
            offsetY += 0.1; // Pan up
            break;
        case GLUT_KEY_DOWN:
            offsetY -= 0.1; // Pan down
            break;
    }
}

// Function to run basic unit tests for evaluateFunction
void testEvaluateFunction() {
    assert(evaluateFunction(0, "sin(x)") == 0);
    assert(fabs(evaluateFunction(M_PI / 2, "sin(x)") - 1) < 1e-6);
    assert(fabs(evaluateFunction(0, "cos(x)") - 1) < 1e-6);
    assert(fabs(evaluateFunction(0, "tan(x)") - 0) < 1e-6);
    assert(fabs(evaluateFunction(1, "x^2") - 1) < 1e-6);
    assert(fabs(evaluateFunction(2, "x^2") - 4) < 1e-6);
    printf("All evaluateFunction tests passed.\n");
}

void iDraw() {
    iClear();       // Clear the screen
    drawAxes();     // Draw axes and grid
    plotFunctions(); // Plot mathematical functions
    plotActiveShape(); // Plot user-defined shape
    drawUI();       // Draw user interface elements
}

// Main function
int main() {
    testEvaluateFunction(); // Run unit tests
    iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Graph Plotter with Shapes");
    return 0;
}
