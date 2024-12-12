#include "iGraphics.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cctype>

// Window dimensions
int windowWidth = 800, windowHeight = 600;


// Graph scaling and offsets
double scaleX = 50, scaleY = 50;
double step = 0.0001;   // controls the density of plotted points
double minScale = 5, maxScale = 100;

// User input handling preparation
bool showTextBox = false;
char userInput[100] = "";
int inputIndex = 0;
double coefficients[10] = {0}; // Coefficients for shapes
char activeShape[20] = "";     // Current shape being plotted

//Manage panning and Coordinate display
bool isPanning = false;
int lastMouseX, lastMouseY;
double offsetX = 1, offsetY = 1; // Panning offsets

// Toggle functions
bool showSin = true, showCos = true, showTan = false, showExp = false, show_xSquare = false;

//
bool showGrid = true;




// Draw the coordinate axes
void drawGrid(double gridSpacing) {
    if (!showGrid) return;

    iSetColor(200, 200, 200); // Light gray for grid lines
    
    // Draw vertical grid lines
    for (double x = -windowWidth / 2; x <= windowWidth / 2; x += gridSpacing) {
        iLine(windowWidth / 2 + x + offsetX * scaleX, 0, windowWidth / 2 + x + offsetX * scaleX, windowHeight);
    }
    
    // Draw horizontal grid lines
    for (double y = -windowHeight / 2; y <= windowHeight / 2; y += gridSpacing) {
        iLine(0, windowHeight / 2 + y + offsetY * scaleY, windowWidth, windowHeight / 2 + y + offsetY * scaleY);
    }

    iSetColor(255, 255, 255); // White for main axes
    iLine(0, windowHeight / 2 + offsetY * scaleY, windowWidth, windowHeight / 2 + offsetY * scaleY); // X-axis
    iLine(windowWidth / 2 + offsetX * scaleX, 0, windowWidth / 2 + offsetX * scaleX, windowHeight);  // Y-axis
}

void drawAxes() {
    double gridSpacing = 50; // Spacing for grid lines

    // Draw background grid
    drawGrid(gridSpacing);
}

// Function to evaluate user-defined function
double evaluateFunction(double x, const char* func) {
    // Implement a simple parser for the function (this example is basic, extend as needed)
    if (strcmp(func, "sin(x)") == 0) return sin(x);
    if (strcmp(func, "cos(x)") == 0) return cos(x);
    if (strcmp(func, "tan(x)") == 0) return tan(x);
    if (strcmp(func, "exp(x)") == 0) return exp(x);
    if (strcmp(func, "x^2") == 0) return x * x;
    // Add more parsing rules here
    return 0; // Default (invalid function)
}

// Generic function to plot any graph
void plotFunction(const char* func, double r, double g, double b) {
    iSetColor(r, g, b); // Set color
    for (double x = -10; x <= 10; x += step) {
        double y1 = evaluateFunction(x, func);
        double y2 = evaluateFunction(x + step, func);

        double screenX1 = windowWidth / 2 + (x + offsetX) * scaleX;
        double screenY1 = windowHeight / 2 + (y1 + offsetY) * scaleY;
        double screenX2 = windowWidth / 2 + (x + step + offsetX) * scaleX;
        double screenY2 = windowHeight / 2 + (y2 + offsetY) * scaleY;

        // Avoid plotting undefined values for tan(x)
        if (!std::isfinite(y1) || !std::isfinite(y2)) continue;

        // Avoid plotting extremely large values
        if (fabs(y1) > 10 || fabs(y2) > 10) continue;

        iLine(screenX1, screenY1, screenX2, screenY2);
    }
}

// Plot an ellipse: x^2/a^2 + y^2/b^2 = 1
void plotEllipse(double a, double b) {
    iSetColor(255, 0, 255); // Purple
    double centerX = windowWidth / 2 + offsetX * scaleX;
    double centerY = windowHeight / 2 + offsetY * scaleY;
    iEllipse(centerX, centerY, a * scaleX, b * scaleY, 100); // Using iEllipse for smoother ellipse
}


// Plot a parabola: y = ax^2 + bx + c
void plotParabola(double a, double b, double c) {
    iSetColor(0, 255, 255); // Cyan
    for (double x = -10; x <= 10; x += step) {
        double y = a * x * x + b * x + c;
        double screenX = windowWidth / 2 + (x + offsetX) * scaleX;
        double screenY = windowHeight / 2 + (y + offsetY) * scaleY;
        iPoint(screenX, screenY);
    }
}

// Plot a hyperbola: x^2/a^2 - y^2/b^2 = 1
void plotHyperbola(double a, double b) {
    iSetColor(255, 165, 0); // Orange
    for (double x = -10; x <= -a; x += step) {
        double y = sqrt(((x * x) / (a * a) - 1) * b * b);
        double screenX = windowWidth / 2 + (x + offsetX) * scaleX;
        double screenY1 = windowHeight / 2 + (y + offsetY) * scaleY;
        double screenY2 = windowHeight / 2 + (-y + offsetY) * scaleY;
        iPoint(screenX, screenY1);
        iPoint(screenX, screenY2);
    }
    for (double x = a; x <= 10; x += step) {
        double y = sqrt(((x * x) / (a * a) - 1) * b * b);
        double screenX = windowWidth / 2 + (x + offsetX) * scaleX;
        double screenY1 = windowHeight / 2 + (y + offsetY) * scaleY;
        double screenY2 = windowHeight / 2 + (-y + offsetY) * scaleY;
        iPoint(screenX, screenY1);
        iPoint(screenX, screenY2);
    }
}

// Plot a straight line: y = mx + c
void plotLine(double m, double c) {
    iSetColor(0, 255, 0); // Green
    for (double x = -10; x <= 10; x += step) {
        double y = m * x + c;
        double screenX = windowWidth / 2 + (x + offsetX) * scaleX;
        double screenY = windowHeight / 2 + (y + offsetY) * scaleY;
        iPoint(screenX, screenY);
    }
}




void iDraw() {
    iClear();       // Clear the screen
    drawAxes();     // Draw axes and grid

    // Plot functions based on toggle states
    if (showSin) plotFunction("sin(x)", 0, 0, 255);         // Blue: y = sin(x)
    if (showCos) plotFunction("cos(x)", 255, 0, 0);         // Red: y = cos(x)
    if (showTan) plotFunction("tan(x)", 0, 255, 0);         // Green: y = tan(x)
    if (showExp) plotFunction("exp(x)", 255, 128, 0);       // Orange: y = exp(x)
    if (show_xSquare) plotFunction("x^2", 128, 0, 255);     // Purple: y = x^2

    // Plot shapes based on user input
    if (strcmp(activeShape, "Ellipse") == 0 && inputIndex >= 2) {
        plotEllipse(coefficients[0], coefficients[1]); // Coefficients: a, b
        showTextBox = false; // Hide text box after plotting
    } else if (strcmp(activeShape, "Parabola") == 0 && inputIndex >= 3) {
        plotParabola(coefficients[0], coefficients[1], coefficients[2]); // Coefficients: a, b, c
        showTextBox = false; // Hide text box after plotting
    } else if (strcmp(activeShape, "Hyperbola") == 0 && inputIndex >= 2) {
        plotHyperbola(coefficients[0], coefficients[1]); // Coefficients: a, b
        showTextBox = false; // Hide text box after plotting
    } else if (strcmp(activeShape, "StraightLine") == 0 && inputIndex >= 2) {
        plotLine(coefficients[0], coefficients[1]); // Coefficients: m, c
        showTextBox = false; // Hide text box after plotting
    }

    // Display text box for user input
    if (showTextBox) {
        iSetColor(200, 200, 200); // Light gray
        iFilledRectangle(300, 500, 200, 30); // Draw text box
        iSetColor(0, 0, 0); // Black text
        iText(310, 510, userInput); // Show current input
    }

    // Display instructions and current settings
    iSetColor(255, 255, 255); // White text
    iText(10, 10, "Press 1 to toggle Sin, 2 to toggle Cos, 3 to toggle Tan, 4 to toggle Exp, 5 to toggle x^2");
    iText(10, 30, "Press 6 to plot Ellipse, 7 to plot Parabola, 8 to plot Hyperbola, 9 to plot Line");
    iText(10, 50, "Use '[' and ']' keys to zoom in and out");
    iText(10, 70, "Press 'g' to toggle grid, arrow keys to pan");
    iText(10, 90, "Press 'q' to exit");

    // Display current input for shapes
    if (showTextBox) {
        if (strcmp(activeShape, "Ellipse") == 0) {
            iText(10, 110, "Enter coefficients for Ellipse (a, b):");
            char aText[100];
            char bText[100];
            sprintf(aText, "a = %s", inputIndex > 0 ? userInput : "...");
            sprintf(bText, "b = %s", inputIndex > 1 ? userInput : "...");
            iText(10, 130, aText);
            iText(10, 150, bText);
        } else if (strcmp(activeShape, "Parabola") == 0) {
            iText(10, 110, "Enter coefficients for Parabola (a, b, c):");
            char aText[100], bText[100], cText[100];
            sprintf(aText, "a = %s", inputIndex > 0 ? userInput : "...");
            sprintf(bText, "b = %s", inputIndex > 1 ? userInput : "...");
            sprintf(cText, "c = %s", inputIndex > 2 ? userInput : "...");
            iText(10, 130, aText);
            iText(10, 150, bText);
            iText(10, 170, cText);
        } else if (strcmp(activeShape, "Hyperbola") == 0) {
            iText(10, 110, "Enter coefficients for Hyperbola (a, b):");
            char aText[100], bText[100];
            sprintf(aText, "a = %s", inputIndex > 0 ? userInput : "...");
            sprintf(bText, "b = %s", inputIndex > 1 ? userInput : "...");
            iText(10, 130, aText);
            iText(10, 150, bText);
        } else if (strcmp(activeShape, "StraightLine") == 0) {
            iText(10, 110, "Enter coefficients for Straight Line (m, c):");
            char mText[100], cText[100];
            sprintf(mText, "m = %s", inputIndex > 0 ? userInput : "...");
            sprintf(cText, "c = %s", inputIndex > 1 ? userInput : "...");
            iText(10, 130, mText);
            iText(10, 150, cText);
        }
    }
}

void iKeyboard(unsigned char key) {
    if (showTextBox) {
        if (isdigit(key) || key == '.' || key == '-' || key == '\r') { // Append only valid characters to userInput
            if (key == '\r') { // Enter key
                coefficients[inputIndex] = atof(userInput); // Convert input to double
                inputIndex++;
                userInput[0] = '\0'; // Clear input
            } else if (key == '\b' && strlen(userInput) > 0) { // Backspace
                userInput[strlen(userInput) - 1] = '\0';
            } else if (key != '\b' && strlen(userInput) < 99) { // Append character
                int len = strlen(userInput);
                userInput[len] = key;
                userInput[len + 1] = '\0';
            }
        }
    } else {
        switch (key) {
            case '1': showSin = !showSin; break;           // Toggle sin(x)
            case '2': showCos = !showCos; break;           // Toggle cos(x)
            case '3': showTan = !showTan; break;           // Toggle tan(x)
            case '4': showExp = !showExp; break;           // Toggle e^x
            case '5': show_xSquare = !show_xSquare; break; // Toggle x^2
            case '6': showTextBox = true; strcpy(activeShape, "Ellipse"); inputIndex = 0; break;
            case '7': showTextBox = true; strcpy(activeShape, "Parabola"); inputIndex = 0; break;
            case '8': showTextBox = true; strcpy(activeShape, "Hyperbola"); inputIndex = 0; break;
            case '9': showTextBox = true; strcpy(activeShape, "StraightLine"); inputIndex = 0; break;
            case '[': 
                if (scaleX < maxScale && scaleY < maxScale) {
                    scaleX *= 1.1; scaleY *= 1.1;
                    step /= 1.1; // Increase detail when zooming in
                }
                break;
            case ']':
                if (scaleX > minScale && scaleY > minScale) {
                    scaleX /= 1.1; scaleY /= 1.1;
                    step *= 1.1; // Decrease detail when zooming out
                }
                break;
            case 'g':
                showGrid = !showGrid; // Toggle grid visibility
                break;
            case 'q':
                exit(0); // Exit
                break;
        }
    }
}

void iPassiveMouseMove(int mx, int my) {
    // Intentionally left empty
}

void iMouse(int button, int state, int mx, int my) {
    static int coordDisplayTime = 0;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isPanning = true;
        lastMouseX = mx;
        lastMouseY = my;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isPanning = false;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        double graphX = (mx - windowWidth / 2 - offsetX * scaleX) / scaleX;
        double graphY = (my - windowHeight / 2 - offsetY * scaleY) / scaleY;
        char coordText[100];
        sprintf(coordText, "Coordinates: (%.2f, %.2f)", graphX, graphY);
        iSetColor(255, 255, 255);
        iText(mx, my, coordText);
        coordDisplayTime = glutGet(GLUT_ELAPSED_TIME); // Record the display time
    }
}


void iMouseMove(int mx, int my) {
    if (isPanning) {
        offsetX += (mx - lastMouseX) / (scaleX * 10);
        offsetY += (my - lastMouseY) / (scaleY * 10);
        lastMouseX = mx;
        lastMouseY = my;
    }
}

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

int main() {
    iInitialize(windowWidth, windowHeight, "Graph Plotter with Shapes");
    return 0;
}
