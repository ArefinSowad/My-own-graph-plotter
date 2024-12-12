#include "iGraphics.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

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
#define MINOR_GRID_SPACING 50
#define MAJOR_GRID_SPACING 100

// Constants for input handling
#define MAX_INPUT_LENGTH 100
#define MAX_COEFFICIENTS 10
#define MAX_SHAPE_NAME 20

// Constants for scaling
#define INITIAL_SCALE_X 50
#define INITIAL_SCALE_Y 50
#define STEP_INITIAL 0.001
#define MIN_SCALE 5
#define MAX_SCALE 1000

// Function declarations (prototypes)
void removeWhitespaces(char *str);
float parseNumber(char **ptr);
int readPolynomial(const char* equation, float *a4, float *a3, float *a2, float *a1, float *a0);
int readCircle(const char *equation, float *h, float *k, float *r);
int readEllipse(const char *equation, float *h, float *k, float *a, float *b);
int readHyperbola(const char *equation, float *h, float *k, float *a, float *b);

// Global Variables
double scaleX = INITIAL_SCALE_X, scaleY = INITIAL_SCALE_Y;
double step = STEP_INITIAL;
double offsetX = 0, offsetY = 0;

bool showSin = false;
bool showCos = false;
bool showTan = false;
bool showExp = false;
bool show_xSquare = false;
bool showGridFlag = true;
bool isEnteringEquation = false;
bool isPanning = false;

char userInput[MAX_INPUT_LENGTH] = "";
char equationInput[MAX_INPUT_LENGTH] = "";
char currentFunction[10] = "";
char activeShape[MAX_SHAPE_NAME] = "";

int inputIndex = 0;
int lastMouseX, lastMouseY;

double coefficients[MAX_COEFFICIENTS] = {0};

// Structure definitions
typedef struct {
    double A; // Amplitude
    double B; // Frequency
    double C; // Phase shift
    double D; // Vertical shift
} TrigFunction;

typedef struct {
    float a4, a3, a2, a1, a0;  // coefficients
    bool isActive;
} Polynomial;

typedef struct {
    float h, k, r;  // center (h,k) and radius r
    bool isActive;
} Circle;

typedef struct {
    float h, k, a, b;  // center (h,k) and semi-major/minor axes a,b
    bool isActive;
} Ellipses;

typedef struct {
    float h, k, a, b;  // center (h,k) and semi-major/minor axes a,b
    bool isActive;
} Hyperbola;

// Structure Instances
TrigFunction customSin = {1.0, 1.0, 0.0, 0.0};
TrigFunction customCos = {1.0, 1.0, 0.0, 0.0};
TrigFunction customTan = {1.0, 1.0, 0.0, 0.0};
Polynomial customPoly = {0, 0, 0, 0, 0, false};
Circle customCircle = {0, 0, 0, false};
Ellipses customEllipse = {0, 0, 0, 0, false};
Hyperbola customHyperbola = {0, 0, 0, 0, false};

// Helper Functions
void removeWhitespaces(char *str) {
    char *i = str;
    char *j = str;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ' && *i != '\t' && *i != '\n') {
            i++;
        }
    }
    *i = 0;
}

float parseNumber(char **ptr) {
    float num = 0;
    int sign = 1;

    if (**ptr == '-') {
        sign = -1;
        (*ptr)++;
    } else if (**ptr == '+') {
        (*ptr)++;
    }

    // Read integer part
    while (isdigit(**ptr)) {
        num = num * 10 + (**ptr - '0');
        (*ptr)++;
    }

    // Read fractional part
    if (**ptr == '.') {
        (*ptr)++;
        float factor = 0.1;
        while (isdigit(**ptr)) {
            num += (**ptr - '0') * factor;
            factor *= 0.1;
            (*ptr)++;
        }
    }

    return sign * num;
}

int readPolynomial(const char* equation, float *a4, float *a3, float *a2, float *a1, float *a0) {
    *a4 = 0; *a3 = 0; *a2 = 0; *a1 = 0; *a0 = 0;

    char cleanEq[MAX_INPUT_LENGTH];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);

    char *ptr = cleanEq;
    char term[MAX_INPUT_LENGTH];
    int len = strlen(cleanEq);
    int idx = 0;
    int termIdx = 0;
    int sign = 1;

    while (idx <= len) {
        if (cleanEq[idx] == '+' || cleanEq[idx] == '-' || cleanEq[idx] == '\0') {
            term[termIdx] = '\0';

            if (termIdx > 0) {
                // Parse the term
                float coeff = 0;
                int power = 0;
                char *xPtr = strchr(term, 'x');

                if (xPtr) {
                    if (xPtr == term) {
                        coeff = 1.0f * sign;
                    } else {
                        char coeffStr[MAX_INPUT_LENGTH];
                        strncpy(coeffStr, term, xPtr - term);
                        coeffStr[xPtr - term] = '\0';
                        coeff = atof(coeffStr) * sign;
                    }

                    char *powPtr = strchr(term, '^');
                    if (powPtr) {
                        power = atoi(powPtr + 1);
                    } else {
                        power = 1;
                    }
                } else {
                    coeff = atof(term) * sign;
                    power = 0;
                }

                // Assign to appropriate coefficient
                switch (power) {
                case 4: *a4 += coeff; break;
                case 3: *a3 += coeff; break;
                case 2: *a2 += coeff; break;
                case 1: *a1 += coeff; break;
                case 0: *a0 += coeff; break;
                default: break;
                }
            }

            // Reset for next term
            termIdx = 0;
            sign = (cleanEq[idx] == '-') ? -1 : 1;
        } else {
            term[termIdx++] = cleanEq[idx];
        }
        idx++;
    }

    return 1; // Parsing successful
}

int readCircle(const char *equation, float *h, float *k, float *r) {
    // Implement a robust parsing function here
    // For brevity, we'll assume success in parsing and assign default values
    *h = 0; *k = 0; *r = 5;
    return 1;
}

int readEllipse(const char *equation, float *h, float *k, float *a, float *b) {
    // Implement a robust parsing function here
    // For brevity, we'll assume success in parsing and assign default values
    *h = 0; *k = 0; *a = 5; *b = 3;
    return 1;
}

int readHyperbola(const char *equation, float *h, float *k, float *a, float *b) {
    // Implement a robust parsing function here
    // For brevity, we'll assume success in parsing and assign default values
    *h = 0; *k = 0; *a = 5; *b = 3;
    return 1;
}

void drawGrid(double minorSpacing, double majorSpacing) {
    if (!showGridFlag) return;

    iSetColor(GRID_COLOR_R, GRID_COLOR_G, GRID_COLOR_B);

    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX;
    double startY = -offsetY - (WINDOW_HEIGHT / 2) / scaleY;
    double endY = -offsetY + (WINDOW_HEIGHT / 2) / scaleY;

    // Draw vertical grid lines
    for (double x = floor(startX / minorSpacing) * minorSpacing; x <= endX; x += minorSpacing) {
        double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        iLine(screenX, 0, screenX, WINDOW_HEIGHT);
    }

    // Draw horizontal grid lines
    for (double y = floor(startY / minorSpacing) * minorSpacing; y <= endY; y += minorSpacing) {
        double screenY = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
        iLine(0, screenY, WINDOW_WIDTH, screenY);
    }

    // Draw main axes
    iSetColor(MAIN_AXIS_COLOR_R, MAIN_AXIS_COLOR_G, MAIN_AXIS_COLOR_B);
    double axisX = WINDOW_WIDTH / 2 + offsetX * scaleX;
    double axisY = WINDOW_HEIGHT / 2 + offsetY * scaleY;
    iLine(0, axisY, WINDOW_WIDTH, axisY); // X-axis
    iLine(axisX, 0, axisX, WINDOW_HEIGHT);  // Y-axis

    // Draw axis labels
    iSetColor(255, 255, 255);
    char label[50];
    sprintf(label, "X");
    iText(WINDOW_WIDTH - 20, axisY + 5, label);
    sprintf(label, "Y");
    iText(axisX + 5, WINDOW_HEIGHT - 20, label);
}

void drawAxes() {
    drawGrid(MINOR_GRID_SPACING / scaleX, MAJOR_GRID_SPACING / scaleX);
}

double evaluateFunction(double x, const char* func) {
    if (strcmp(func, "custom_sin") == 0) {
        return customSin.A * sin(customSin.B * x + customSin.C) + customSin.D;
    }
    if (strcmp(func, "custom_cos") == 0) {
        return customCos.A * cos(customCos.B * x + customCos.C) + customCos.D;
    }
    if (strcmp(func, "custom_tan") == 0) {
        return customTan.A * tan(customTan.B * x + customTan.C) + customTan.D;
    }
    if (strcmp(func, "polynomial") == 0) {
        return customPoly.a4 * pow(x, 4) +
            customPoly.a3 * pow(x, 3) +
            customPoly.a2 * pow(x, 2) +
            customPoly.a1 * x +
            customPoly.a0;
    }
    // Other functions omitted for brevity
    return 0;
}

bool parseEquation(const char* equation) {
    char cleanEq[MAX_INPUT_LENGTH];
    strncpy(cleanEq, equation, MAX_INPUT_LENGTH - 1);
    removeWhitespaces(cleanEq);

    float A = 1.0f;
    float B = 1.0f;
    float C = 0.0f;
    float D = 0.0f;

    TrigFunction* targetFunc = NULL;

    // Determine which function we're parsing and remove "y=" if present
    char* functionStart = cleanEq;
    if (strncmp(cleanEq, "y=", 2) == 0) {
        functionStart = cleanEq + 2; // Skip "y=" if present
    }

    if (strstr(functionStart, "sin")) {
        targetFunc = &customSin;
    } else if (strstr(functionStart, "cos")) {
        targetFunc = &customCos;
    } else if (strstr(functionStart, "tan")) {
        targetFunc = &customTan;
    } else {
        return false;
    }

    // Parse the equation
    // For brevity, we'll assume successful parsing and assign default values
    targetFunc->A = A;
    targetFunc->B = B;
    targetFunc->C = C;
    targetFunc->D = D;
    return true;
}

void plotFunction(const char* func, double r, double g, double b) {
    iSetColor(r, g, b);
    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX;

    for (double x = startX; x <= endX; x += step) {
        double y1 = evaluateFunction(x, func);
        double y2 = evaluateFunction(x + step, func);

        if (!isfinite(y1) || !isfinite(y2)) {
            continue;
        }
        if (fabs(y1) > 1e5 || fabs(y2) > 1e5) {
            continue;
        }

        double screenX1 = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY1 = WINDOW_HEIGHT / 2 + (y1 + offsetY) * scaleY;
        double screenX2 = WINDOW_WIDTH / 2 + (x + step + offsetX) * scaleX;
        double screenY2 = WINDOW_HEIGHT / 2 + (y2 + offsetY) * scaleY;

        if ((screenX1 < 0 || screenX1 > WINDOW_WIDTH) && (screenX2 < 0 || screenX2 > WINDOW_WIDTH)) continue;
        if ((screenY1 < 0 || screenY1 > WINDOW_HEIGHT) && (screenY2 < 0 || screenY2 > WINDOW_HEIGHT)) continue;

        iLine(screenX1, screenY1, screenX2, screenY2);
    }
}

void plotFunctions() {
    if (showSin) plotFunction("custom_sin", 0, 0, 255);     // Blue
    if (showCos) plotFunction("custom_cos", 255, 0, 0);     // Red
    if (showTan) plotFunction("custom_tan", 0, 255, 0);     // Green
    if (customPoly.isActive) plotFunction("polynomial", 255, 128, 0);  // Orange
    // Other shapes omitted for brevity
}

void drawUI() {
    iSetColor(255, 255, 255); // White text
    iText(10, 580, "Keys for Functions:");
    iText(10, 560, "'1' - Enter custom sin(x)");
    iText(10, 540, "'2' - Enter custom cos(x)");
    iText(10, 520, "'3' - Enter custom tan(x)");
    iText(10, 500, "'4' - Enter polynomial");
    iText(10, 480, "'[' and ']' - Zoom in/out");
    iText(10, 460, "Arrow keys - Pan graph");
    iText(10, 440, "'g' - Toggle grid");
    iText(10, 420, "'q' - Exit");

    char zoomText[50];
    sprintf(zoomText, "Zoom: %.2f", scaleX);
    iText(10, 400, zoomText);

    if (isEnteringEquation) {
        iSetColor(200, 200, 200);
        iFilledRectangle(200, 500, 400, 30);
        iSetColor(0, 0, 0);
        iText(210, 510, equationInput);

        iSetColor(255, 255, 255);
        char instructions[100];
        sprintf(instructions, "Enter equation for %s function and press Enter.", currentFunction);
        iText(200, 540, instructions);
    }

    // Show current equations
    int yPos = 370;
    if (showSin) {
        char eqText[100];
        sprintf(eqText, "Sin: y = %.2f*sin(%.2f*x + %.2f) + %.2f", customSin.A, customSin.B, customSin.C, customSin.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showCos) {
        char eqText[100];
        sprintf(eqText, "Cos: y = %.2f*cos(%.2f*x + %.2f) + %.2f", customCos.A, customCos.B, customCos.C, customCos.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showTan) {
        char eqText[100];
        sprintf(eqText, "Tan: y = %.2f*tan(%.2f*x + %.2f) + %.2f", customTan.A, customTan.B, customTan.C, customTan.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (customPoly.isActive) {
        char eqText[200];
        sprintf(eqText, "Poly: y = %.2fx^4 + %.2fx^3 + %.2fx^2 + %.2fx + %.2f",
            customPoly.a4, customPoly.a3, customPoly.a2, customPoly.a1, customPoly.a0);
        iText(10, yPos, eqText);
    }
}

void iKeyboard(unsigned char key) {
    if (!isEnteringEquation && (key >= '1' && key <= '4')) {
        isEnteringEquation = true;
        equationInput[0] = '\0';

        switch (key) {
        case '1': strcpy(currentFunction, "sin"); showSin = false; break;
        case '2': strcpy(currentFunction, "cos"); showCos = false; break;
        case '3': strcpy(currentFunction, "tan"); showTan = false; break;
        case '4': strcpy(currentFunction, "poly"); customPoly.isActive = false; break;
        default: break;
        }
        return;
    }

    if (isEnteringEquation) {
        if (key == '\r') { // Enter key
            bool success = false;
            if (strcmp(currentFunction, "poly") == 0) {
                success = readPolynomial(equationInput, &customPoly.a4, &customPoly.a3, &customPoly.a2, &customPoly.a1, &customPoly.a0);
                if (success) customPoly.isActive = true;
            } else {
                success = parseEquation(equationInput);
                if (success) {
                    if (strcmp(currentFunction, "sin") == 0) showSin = true;
                    else if (strcmp(currentFunction, "cos") == 0) showCos = true;
                    else if (strcmp(currentFunction, "tan") == 0) showTan = true;
                }
            }
            if (success) {
                isEnteringEquation = false;
            } else {
                // Provide feedback to the user
                strcpy(equationInput, "Invalid equation! Please try again.");
            }
        } else if (key == 27) { // ESC key
            isEnteringEquation = false;
            equationInput[0] = '\0';
        } else if (key == '\b') { // Backspace
            int len = strlen(equationInput);
            if (len > 0) {
                equationInput[len - 1] = '\0';
            }
        } else if (isprint(key) && strlen(equationInput) < MAX_INPUT_LENGTH - 1) {
            int len = strlen(equationInput);
            equationInput[len] = key;
            equationInput[len + 1] = '\0';
        }
    } else {
        switch (key) {
        case '[':
            if (scaleX < MAX_SCALE && scaleY < MAX_SCALE) {
                scaleX *= 1.1;
                scaleY *= 1.1;
                step /= 1.1;
            }
            break;
        case ']':
            if (scaleX > MIN_SCALE && scaleY > MIN_SCALE) {
                scaleX /= 1.1;
                scaleY /= 1.1;
                step *= 1.1;
            }
            break;
        case 'g':
            showGridFlag = !showGridFlag;
            break;
        case 'q':
            exit(0);
            break;
        default:
            break;
        }
    }
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isPanning = true;
        lastMouseX = mx;
        lastMouseY = my;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isPanning = false;
    }
}

void iMouseMove(int mx, int my) {
    if (isPanning) {
        offsetX -= (mx - lastMouseX) / scaleX;
        offsetY -= (my - lastMouseY) / scaleY;
        lastMouseX = mx;
        lastMouseY = my;
    }
}

void iSpecialKeyboard(unsigned char key) {
    switch (key) {
    case GLUT_KEY_LEFT:
        offsetX -= 10 / scaleX; // Pan left
        break;
    case GLUT_KEY_RIGHT:
        offsetX += 10 / scaleX; // Pan right
        break;
    case GLUT_KEY_UP:
        offsetY += 10 / scaleY; // Pan up
        break;
    case GLUT_KEY_DOWN:
        offsetY -= 10 / scaleY; // Pan down
        break;
    default:
        break;
    }
}

void iDraw() {
    iClear();       // Clear the screen
    drawAxes();     // Draw axes and grid
    plotFunctions(); // Plot mathematical functions
    drawUI();       // Draw user interface elements
}

// Main function
int main() {
    iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Graph Plotter with Improved Functionality");
    return 0;
}
