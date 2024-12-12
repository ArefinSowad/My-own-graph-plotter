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
#define STEP_INITIAL 0.001
#define MIN_SCALE 5
#define MAX_SCALE 100


// Function declarations (prototypes)
void removeWhitespaces(char *str);
float parseNumber(char **ptr);
void readPolynomial(const char* equation, float *a4, float *a3, float *a2, float *a1, float *a0);
int readCircle(const char *equation, float *h, float *k, float *r);
int readEllipse(const char *equation, float *h, float *k, float *a, float *b);
int readHyperbola(const char *equation, float *h, float *k, float *a, float *b);

// Global Variables
double scaleX = INITIAL_SCALE_X, scaleY = INITIAL_SCALE_Y;
double step = STEP_INITIAL;
double offsetX = 1, offsetY = 1;

bool showSin = true;
bool showCos = true;
bool showTan = false;
bool showExp = false;
bool show_xSquare = false;
bool showGridFlag = true;
bool showTextBox = false;
bool isEnteringEquation = false;
bool isPanning = false;

char userInput[MAX_INPUT_LENGTH] = "";
char equationInput[MAX_INPUT_LENGTH] = "";
char currentFunction[10] = "";
char activeShape[MAX_SHAPE_NAME] = "";

int inputIndex = 0;
int lastMouseX, lastMouseY;

double coefficients[MAX_COEFFICIENTS] = {0};

// Complete structure definitions
// all typedefs for the structures
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
    for (char *j = str; *j != '\0'; j++) {
        if (*j != ' ' && *j != '\t' && *j != '\n') {
            *i = *j;
            i++;
        }
    }
    *i = '\0';
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
    
    while (**ptr >= '0' && **ptr <= '9') {
        num = num * 10 + (**ptr - '0');
        (*ptr)++;
    }
    
    if (**ptr == '.') {
        (*ptr)++;
        float factor = 0.1;
        while (**ptr >= '0' && **ptr <= '9') {
            num += (**ptr - '0') * factor;
            factor *= 0.1;
            (*ptr)++;
        }
    }
    
    return sign * num;
}

void readPolynomial(const char* equation, float *a4, float *a3, float *a2, float *a1, float *a0) {
    *a4 = 0; *a3 = 0; *a2 = 0; *a1 = 0; *a0 = 0;
    
    char cleanEq[200];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);
    
    char *term = strtok(cleanEq, "+-");
    while (term != NULL) {
        float coeff = 1.0;
        int power = 0;
        
        // Parse coefficient
        char *xPos = strchr(term, 'x');
        if (xPos != NULL) {
            if (xPos != term) {
                char coeffStr[20] = {0};
                strncpy(coeffStr, term, xPos - term);
                coeff = atof(coeffStr);
            }
            
            // Parse power
            if (*(xPos + 1) == '^') {
                power = atoi(xPos + 2);
            } else {
                power = 1;
            }
        } else {
            coeff = atof(term);
            power = 0;
        }
        
        // Assign to appropriate coefficient
        switch(power) {
            case 4: *a4 = coeff; break;
            case 3: *a3 = coeff; break;
            case 2: *a2 = coeff; break;
            case 1: *a1 = coeff; break;
            case 0: *a0 = coeff; break;
        }
        
        term = strtok(NULL, "+-");
    }
}

int readCircle(const char *equation, float *h, float *k, float *r) {
    *h = 0; *k = 0; *r = 0;
    
    char cleanEquation[200];
    strcpy(cleanEquation, equation);
    removeWhitespaces(cleanEquation);
    
    char *ptr = cleanEquation;
    
    if (*ptr++ != '(') return 0;
    if (*ptr++ != 'x') return 0;
    
    if (*ptr == '-' || *ptr == '+') {
        float sign = (*ptr++ == '-') ? 1 : -1;
        if (!(*ptr >= '0' && *ptr <= '9')) return 0;
        *h = sign * parseNumber(&ptr);
    }
    
    if (*ptr++ != ')') return 0;
    if (*ptr++ != '^') return 0;
    if (*ptr++ != '2') return 0;
    if (*ptr++ != '+') return 0;
    if (*ptr++ != '(') return 0;
    if (*ptr++ != 'y') return 0;
    
    if (*ptr == '-' || *ptr == '+') {
        float sign = (*ptr++ == '-') ? 1 : -1;
        if (!(*ptr >= '0' && *ptr <= '9')) return 0;
        *k = sign * parseNumber(&ptr);
    }
    
    if (*ptr++ != ')') return 0;
    if (*ptr++ != '^') return 0;
    if (*ptr++ != '2') return 0;
    if (*ptr++ != '=') return 0;
    
    *r = sqrt(parseNumber(&ptr));
    
    return 1;
}

int readEllipse(const char *equation, float *h, float *k, float *a, float *b) {
    *h = 0; *k = 0; *a = 0; *b = 0;
    
    char cleanEquation[200];
    strcpy(cleanEquation, equation);
    removeWhitespaces(cleanEquation);
    
    char *ptr = cleanEquation;
    
    if (*ptr++ != '(') return 0;
    if (*ptr++ != 'x') return 0;
    
    if (*ptr == '-' || *ptr == '+') {
        float sign = (*ptr++ == '-') ? 1 : -1;
        if (!(*ptr >= '0' && *ptr <= '9')) return 0;
        *h = sign * parseNumber(&ptr);
    }
    
    if (*ptr++ != ')') return 0;
    if (*ptr++ != '^') return 0;
    if (*ptr++ != '2') return 0;
    if (*ptr++ != '/') return 0;
    
    *a = sqrt(parseNumber(&ptr));
    
    if (*ptr++ != '+') return 0;
    if (*ptr++ != '(') return 0;
    if (*ptr++ != 'y') return 0;
    
    if (*ptr == '-' || *ptr == '+') {
        float sign = (*ptr++ == '-') ? 1 : -1;
        if (!(*ptr >= '0' && *ptr <= '9')) return 0;
        *k = sign * parseNumber(&ptr);
    }
    
    if (*ptr++ != ')') return 0;
    if (*ptr++ != '^') return 0;
    if (*ptr++ != '2') return 0;
    if (*ptr++ != '/') return 0;
    
    *b = sqrt(parseNumber(&ptr));
    
    if (*ptr++ != '=') return 0;
    if (*ptr++ != '1') return 0;
    
    return 1;
}

int readHyperbola(const char *equation, float *h, float *k, float *a, float *b) {
    *h = 0; *k = 0; *a = 0; *b = 0;
    
    char cleanEquation[200];
    strcpy(cleanEquation, equation);
    removeWhitespaces(cleanEquation);
    
    char *ptr = cleanEquation;
    
    if (*ptr++ != '(') return 0;
    if (*ptr++ != 'x') return 0;
    
    if (*ptr == '-' || *ptr == '+') {
        float sign = (*ptr++ == '-') ? 1 : -1;
        if (!(*ptr >= '0' && *ptr <= '9')) return 0;
        *h = sign * parseNumber(&ptr);
    }
    
    if (*ptr++ != ')') return 0;
    if (*ptr++ != '^') return 0;
    if (*ptr++ != '2') return 0;
    if (*ptr++ != '/') return 0;
    
    *a = sqrt(parseNumber(&ptr));
    
    if (*ptr++ != '-') return 0;
    if (*ptr++ != '(') return 0;
    if (*ptr++ != 'y') return 0;
    
    if (*ptr == '-' || *ptr == '+') {
        float sign = (*ptr++ == '-') ? 1 : -1;
        if (!(*ptr >= '0' && *ptr <= '9')) return 0;
        *k = sign * parseNumber(&ptr);
    }
    
    if (*ptr++ != ')') return 0;
    if (*ptr++ != '^') return 0;
    if (*ptr++ != '2') return 0;
    if (*ptr++ != '/') return 0;
    
    *b = sqrt(parseNumber(&ptr));
    
    if (*ptr++ != '=') return 0;
    if (*ptr++ != '1') return 0;
    
    return 1;
}

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
    if (strcmp(func, "circle") == 0) {
        double y2 = customCircle.r * customCircle.r - 
                pow(x - customCircle.h, 2);
        if (y2 < 0) return NAN;
        return sqrt(y2) + customCircle.k;
    }
    if (strcmp(func, "circle_neg") == 0) {
        double y2 = customCircle.r * customCircle.r - 
                pow(x - customCircle.h, 2);
        if (y2 < 0) return NAN;
        return -sqrt(y2) + customCircle.k;
    }
    if (strcmp(func, "ellipse") == 0) {
        double y2 = customEllipse.b * customEllipse.b * 
                (1 - pow(x - customEllipse.h, 2) / 
                (customEllipse.a * customEllipse.a));
        if (y2 < 0) return NAN;
        return sqrt(y2) + customEllipse.k;
    }
    if (strcmp(func, "ellipse_neg") == 0) {
        double y2 = customEllipse.b * customEllipse.b * 
                (1 - pow(x - customEllipse.h, 2) / 
                (customEllipse.a * customEllipse.a));
        if (y2 < 0) return NAN;
        return -sqrt(y2) + customEllipse.k;
    }
    if (strcmp(func, "hyperbola") == 0) {
        double y2 = customHyperbola.b * customHyperbola.b * 
                (pow(x - customHyperbola.h, 2) / 
                (customHyperbola.a * customHyperbola.a) - 1);
        if (y2 < 0) return NAN;
        return sqrt(y2) + customHyperbola.k;
    }
    if (strcmp(func, "hyperbola_neg") == 0) {
        double y2 = customHyperbola.b * customHyperbola.b * 
                (pow(x - customHyperbola.h, 2) / 
                (customHyperbola.a * customHyperbola.a) - 1);
        if (y2 < 0) return NAN;
        return -sqrt(y2) + customHyperbola.k;
    }
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
    }
    
    if (targetFunc == NULL) return false;

    // Try parsing with full format first: A*trig(B*x+C)+D
    int result = sscanf(functionStart, "%f*%*[^(](%f*x%f)%f", &A, &B, &C, &D);
    
    if (result < 2) {
        // Try simpler format: trig(B*x)
        if (sscanf(functionStart, "%*[^(](%f*x)", &B) < 1) {
            // If that fails, try simplest format: trig(x)
            if (strstr(functionStart, "(x)")) {
                B = 1.0f;
                result = 1;
            }
        } else {
            result = 1;
        }
    }
    
    // Set default values based on what was parsed
    if (result >= 1) {
        targetFunc->A = A;
        targetFunc->B = B;
        targetFunc->C = C;
        targetFunc->D = D;
        return true;
    }
    
    return false;
}


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


void plotFunctions() {
    if (showSin) plotFunction("custom_sin", 0, 0, 255);     // Blue
    if (showCos) plotFunction("custom_cos", 255, 0, 0);     // Red
    if (showTan) plotFunction("custom_tan", 0, 255, 0);     // Green
    if (customPoly.isActive) plotFunction("polynomial", 255, 128, 0);  // Orange
    if (customCircle.isActive) {
        plotFunction("circle", 128, 0, 128);      // Purple
        plotFunction("circle_neg", 128, 0, 128);  // Purple
    }
    if (customEllipse.isActive) {
        plotFunction("ellipse", 0, 128, 128);     // Teal
        plotFunction("ellipse_neg", 0, 128, 128); // Teal
    }
    if (customHyperbola.isActive) {
        plotFunction("hyperbola", 128, 128, 0);     // Olive
        plotFunction("hyperbola_neg", 128, 128, 0); // Olive
    }
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

void drawUI() {
    iSetColor(255, 255, 255); // White text
    iText(10, 10, "Keys for Functions:");
    iText(10, 30, "'1' - Enter custom sin(x)");
    iText(10, 50, "'2' - Enter custom cos(x)");
    iText(10, 70, "'3' - Enter custom tan(x)");
    iText(10, 90, "'4' - Enter polynomial");
    iText(10, 110, "'[' and ']' - Zoom in/out");
    iText(10, 130, "Arrow keys - Pan graph");
    iText(10, 150, "'g' - Toggle grid");
    iText(10, 170, "'q' - Exit");

    char zoomText[50];
    sprintf(zoomText, "Zoom: %.2f", scaleX);
    iText(10, 190, zoomText);

    if (isEnteringEquation) {
        iSetColor(200, 200, 200);
        iFilledRectangle(200, 500, 400, 30);
        iSetColor(0, 0, 0);
        iText(210, 510, equationInput);
        
        iSetColor(255, 255, 255);
        if (strcmp(currentFunction, "poly") == 0) {
            iText(200, 540, "Format: ax^4 + bx^3 + cx^2 + dx + e");
            iText(200, 560, "Examples:");
            iText(200, 580, "2x^4 - 3.5x^3 + 4x^2 - 1.5x + 6");
            iText(200, 600, "x^2 + 1");
        } else {
            char instructions[100];
            sprintf(instructions, "Format: [A*]%s([B*]x[+C])[+D]", currentFunction);
            iText(200, 540, instructions);
            iText(200, 560, "Examples:");
            iText(200, 580, "2*sin(3*x+1.5)+1");
            iText(200, 600, "sin(x)    [defaults: A=1, B=1, C=0, D=0]");
        }
        iText(200, 620, "Press Enter to confirm, ESC to cancel");
    }

    // Show current equations
    int yPos = 210;
    if (showSin) {
        char eqText[100];
        if (customSin.A == 1 && customSin.C == 0 && customSin.D == 0) {
            if (customSin.B == 1) sprintf(eqText, "Sin: sin(x)");
            else sprintf(eqText, "Sin: sin(%.2f*x)", customSin.B);
        } else {
            sprintf(eqText, "Sin: %.2f*sin(%.2f*x+%.2f)+%.2f", 
                    customSin.A, customSin.B, customSin.C, customSin.D);
        }
        iText(10, yPos, eqText);
        yPos += 20;
    }
    if (showCos) {
        char eqText[100];
        if (customCos.A == 1 && customCos.C == 0 && customCos.D == 0) {
            if (customCos.B == 1) sprintf(eqText, "Cos: cos(x)");
            else sprintf(eqText, "Cos: cos(%.2f*x)", customCos.B);
        } else {
            sprintf(eqText, "Cos: %.2f*cos(%.2f*x+%.2f)+%.2f", 
                    customCos.A, customCos.B, customCos.C, customCos.D);
        }
        iText(10, yPos, eqText);
        yPos += 20;
    }
    if (showTan) {
        char eqText[100];
        if (customTan.A == 1 && customTan.C == 0 && customTan.D == 0) {
            if (customTan.B == 1) sprintf(eqText, "Tan: tan(x)");
            else sprintf(eqText, "Tan: tan(%.2f*x)", customTan.B);
        } else {
            sprintf(eqText, "Tan: %.2f*tan(%.2f*x+%.2f)+%.2f", 
                    customTan.A, customTan.B, customTan.C, customTan.D);
        }
        iText(10, yPos, eqText);
        yPos += 20;
    }
    if (customPoly.isActive) {
        char eqText[200] = "Poly: ";
        char term[50];
        bool first = true;
        
        if (customPoly.a4 != 0) {
            sprintf(term, "%gx^4", customPoly.a4);
            strcat(eqText, term);
            first = false;
        }
        if (customPoly.a3 != 0) {
            sprintf(term, "%s%gx^3", first ? "" : (customPoly.a3 > 0 ? " + " : " "), customPoly.a3);
            strcat(eqText, term);
            first = false;
        }
        if (customPoly.a2 != 0) {
            sprintf(term, "%s%gx^2", first ? "" : (customPoly.a2 > 0 ? " + " : " "), customPoly.a2);
            strcat(eqText, term);
            first = false;
        }
        if (customPoly.a1 != 0) {
            sprintf(term, "%s%gx", first ? "" : (customPoly.a1 > 0 ? " + " : " "), customPoly.a1);
            strcat(eqText, term);
            first = false;
        }
        if (customPoly.a0 != 0) {
            sprintf(term, "%s%g", first ? "" : (customPoly.a0 > 0 ? " + " : " "), customPoly.a0);
            strcat(eqText, term);
        }
        if (first) strcat(eqText, "0");
        
        iText(10, yPos, eqText);
    }
}

void handleUserInput(unsigned char key) {
    if (showTextBox) {
        if (isdigit(key) || key == '.' || key == '-' || key == '\r') {
            if (key == '\r') { // Enter key
                coefficients[inputIndex] = atof(userInput);
                inputIndex++;
                userInput[0] = '\0'; // Clear input

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
            } else if (key != '\b' && strlen(userInput) < MAX_INPUT_LENGTH - 1) {
                int len = strlen(userInput);
                userInput[len] = key;
                userInput[len + 1] = '\0';
            }
        }
    } else {
        switch (key) {
            case '4': showExp = !showExp; break;           
            case '5': show_xSquare = !show_xSquare; break; 
            case '6': showTextBox = true; strcpy(activeShape, "Ellipse"); inputIndex = 0; break;
            case '7': showTextBox = true; strcpy(activeShape, "Parabola"); inputIndex = 0; break;
            case '8': showTextBox = true; strcpy(activeShape, "Hyperbola"); inputIndex = 0; break;
            case '9': showTextBox = true; strcpy(activeShape, "StraightLine"); inputIndex = 0; break;
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
        }
    }
}

// Function to toggle function visibility and initiate shape plotting
void toggleFunction(unsigned char key) {
    switch (key) {
        case 'e': // Press 'e' to enter equation mode
            isEnteringEquation = true;
            equationInput[0] = '\0';
            break;
        case '2': showCos = !showCos; break;
        case '3': showTan = !showTan; break;
        case '4': showExp = !showExp; break;
        case '5': show_xSquare = !show_xSquare; break;
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

void iKeyboard(unsigned char key) {
    if (!isEnteringEquation && (key >= '1' && key <= '7')) {
        showTextBox = false;
        isEnteringEquation = true;
        equationInput[0] = '\0';
        
        switch(key) {
            case '1': strcpy(currentFunction, "sin"); showSin = false; break;
            case '2': strcpy(currentFunction, "cos"); showCos = false; break;
            case '3': strcpy(currentFunction, "tan"); showTan = false; break;
            case '4': strcpy(currentFunction, "poly"); customPoly.isActive = false; break;
            case '5': strcpy(currentFunction, "circle"); customCircle.isActive = false; break;
            case '6': strcpy(currentFunction, "ellipse"); customEllipse.isActive = false; break;
            case '7': strcpy(currentFunction, "hyperbola"); customHyperbola.isActive = false; break;
        }
        return;
    }
    
    if (isEnteringEquation) {
        if (key == '\r') { // Enter key
            bool success = false;
            if (strcmp(currentFunction, "poly") == 0) {
                readPolynomial(equationInput, &customPoly.a4, &customPoly.a3, 
                             &customPoly.a2, &customPoly.a1, &customPoly.a0);
                customPoly.isActive = true;
                success = true;
            } else if (strcmp(currentFunction, "circle") == 0) {
                success = readCircle(equationInput, &customCircle.h, &customCircle.k, &customCircle.r);
                customCircle.isActive = success;
            } else if (strcmp(currentFunction, "ellipse") == 0) {
                success = readEllipse(equationInput, &customEllipse.h, &customEllipse.k, 
                                    &customEllipse.a, &customEllipse.b);
                customEllipse.isActive = success;
            } else if (strcmp(currentFunction, "hyperbola") == 0) {
                success = readHyperbola(equationInput, &customHyperbola.h, &customHyperbola.k, 
                                      &customHyperbola.a, &customHyperbola.b);
                customHyperbola.isActive = success;
            } else {
                success = parseEquation(equationInput);
                if (success) {
                    if (strcmp(currentFunction, "sin") == 0) showSin = true;
                    else if (strcmp(currentFunction, "cos") == 0) showCos = true;
                    else if (strcmp(currentFunction, "tan") == 0) showTan = true;
                }
            }
            if (success) isEnteringEquation = false;
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
        }
    }
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
/*
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
*/
void iDraw() {
    iClear();       // Clear the screen
    drawAxes();     // Draw axes and grid
    plotFunctions(); // Plot mathematical functions
    plotActiveShape(); // Plot user-defined shape
    drawUI();       // Draw user interface elements
}

// Main function
int main() {
//    testEvaluateFunction(); // Run unit tests
    iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Graph Plotter with Shapes");
    return 0;
}
