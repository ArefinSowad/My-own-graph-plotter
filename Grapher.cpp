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
int readTrigFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType);
int readInverseTrigFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType);
int readExponentialFunction(const char* equation, float *A, float *B, float *C, float *D);
int readLogFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType);
int readCircle(const char *equation, float *h, float *k, float *r);
int readEllipse(const char *equation, float *h, float *k, float *a, float *b);
int readParabola(const char *equation, float *a, float *b, float *c);
int readHyperbola(const char *equation, float *h, float *k, float *a, float *b);

// Global Variables
double scaleX = INITIAL_SCALE_X, scaleY = INITIAL_SCALE_Y;
double step = STEP_INITIAL;
double offsetX = 0, offsetY = 0;

bool showSin = false;
bool showCos = false;
bool showTan = false;
bool showASin = false;
bool showACos = false;
bool showATan = false;
bool showExp = false;
bool showLog = false;
bool showLn = false;
bool showPoly = false;
bool showCircle = false;
bool showEllipse = false;
bool showParabola = false;
bool showHyperbola = false;

bool showGridFlag = true;
bool isEnteringEquation = false;
bool isPanning = false;

char userInput[MAX_INPUT_LENGTH] = "";
char equationInput[MAX_INPUT_LENGTH] = "";
char currentFunction[20] = "";
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
} Circle;

typedef struct {
    float h, k, a, b;  // center (h,k) and semi-major/minor axes a,b
} Ellipses;   // Ellipse to Ellipses because of compaitibily issue

typedef struct {
    float a, b, c;  // coefficients for y = ax^2 + bx + c
} Parabola;

typedef struct {
    float h, k, a, b;  // center (h,k) and semi-major/minor axes a,b
} Hyperbola;

// Structure Instances
TrigFunction customSin = {1.0, 1.0, 0.0, 0.0};
TrigFunction customCos = {1.0, 1.0, 0.0, 0.0};
TrigFunction customTan = {1.0, 1.0, 0.0, 0.0};
TrigFunction customASin = {1.0, 1.0, 0.0, 0.0};
TrigFunction customACos = {1.0, 1.0, 0.0, 0.0};
TrigFunction customATan = {1.0, 1.0, 0.0, 0.0};
Polynomial customPoly = {0, 0, 0, 0, 0, false};
Circle customCircle = {0, 0, 0};
Ellipses customEllipse = {0, 0, 0, 0};
Parabola customParabola = {0, 0, 0};
Hyperbola customHyperbola = {0, 0, 0, 0};

typedef struct {
    float A, B, C, D; // For y = A * exp(B * x + C) + D
} ExponentialFunction;

typedef struct {
    float A, B, C, D; // For y = A * log(B * x + C) + D
} LogFunction;

ExponentialFunction customExp = {1.0, 1.0, 0.0, 0.0};
LogFunction customLog = {1.0, 1.0, 0.0, 0.0};
LogFunction customLn = {1.0, 1.0, 0.0, 0.0};

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

int readTrigFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType) {
    *A = 1.0f; *B = 1.0f; *C = 0.0f; *D = 0.0f;

    char cleanEq[MAX_INPUT_LENGTH];
    strncpy(cleanEq, equation, MAX_INPUT_LENGTH - 1);
    removeWhitespaces(cleanEq);

    char *funcPtr = strstr(cleanEq, funcType);
    if (!funcPtr) return 0;

    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    if (ptr != funcPtr) {
        char coeffStr[MAX_INPUT_LENGTH];
        strncpy(coeffStr, funcPtr - ptr <= 0 ? ptr : ptr, funcPtr - ptr);
        coeffStr[funcPtr - ptr] = '\0';
        *A = atof(coeffStr);
    }

    ptr = funcPtr + strlen(funcType);

    if (*ptr == '(') {
        ptr++;
    } else {
        return 0;
    }

    char innerFunc[MAX_INPUT_LENGTH];
    int idx = 0;

    while (*ptr != ')' && *ptr != '\0') {
        innerFunc[idx++] = *ptr++;
    }
    innerFunc[idx] = '\0';

    // Parse B and C from inner function Bx + C
    char *xPtr = strchr(innerFunc, 'x');
    if (!xPtr) return 0;

    if (xPtr != innerFunc) {
        char coeffStr[MAX_INPUT_LENGTH];
        strncpy(coeffStr, innerFunc, xPtr - innerFunc);
        coeffStr[xPtr - innerFunc] = '\0';
        *B = atof(coeffStr);
    }

    ptr = xPtr + 1;
    if (*ptr == '+' || *ptr == '-') {
        *C = atof(ptr);
    }

    ptr = strchr(ptr, ')');
    if (ptr && *(ptr + 1) == '+') {
        *D = atof(ptr + 2);
    } else if (ptr && *(ptr + 1) == '-') {
        *D = -atof(ptr + 2);
    }

    return 1;
}

int readInverseTrigFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType) {
    return readTrigFunction(equation, A, B, C, D, funcType);
}

int readExponentialFunction(const char* equation, float *A, float *B, float *C, float *D) {
    *A = 1.0f; *B = 1.0f; *C = 0.0f; *D = 0.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strncpy(cleanEq, equation, MAX_INPUT_LENGTH - 1);
    removeWhitespaces(cleanEq);

    char *expPtr = strstr(cleanEq, "exp");
    if (!expPtr) return 0;

    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    if (ptr != expPtr) {
        char coeffStr[MAX_INPUT_LENGTH];
        strncpy(coeffStr, ptr, expPtr - ptr);
        coeffStr[expPtr - ptr] = '\0';
        *A = atof(coeffStr);
    }

    ptr = expPtr + 3;

    if (*ptr == '(') {
        ptr++;
    } else {
        return 0;
    }

    char innerFunc[MAX_INPUT_LENGTH];
    int idx = 0;

    while (*ptr != ')' && *ptr != '\0') {
        innerFunc[idx++] = *ptr++;
    }
    innerFunc[idx] = '\0';

    // Parse B and C from inner function Bx + C
    char *xPtr = strchr(innerFunc, 'x');
    if (!xPtr) return 0;

    if (xPtr != innerFunc) {
        char coeffStr[MAX_INPUT_LENGTH];
        strncpy(coeffStr, innerFunc, xPtr - innerFunc);
        coeffStr[xPtr - innerFunc] = '\0';
        *B = atof(coeffStr);
    }

    ptr = xPtr + 1;
    if (*ptr == '+' || *ptr == '-') {
        *C = atof(ptr);
    }

    ptr = strchr(ptr, ')');
    if (ptr && *(ptr + 1) == '+') {
        *D = atof(ptr + 2);
    } else if (ptr && *(ptr + 1) == '-') {
        *D = -atof(ptr + 2);
    }

    return 1;
}

int readLogFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType) {
    return readExponentialFunction(equation, A, B, C, D);
}

int readCircle(const char *equation, float *h, float *k, float *r) {
    char cleanEq[MAX_INPUT_LENGTH];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);

    // Expected format: (x - h)^2 + (y - k)^2 = r^2
    char *ptr = cleanEq;

    // Find substrings for (x - h)^2 and (y - k)^2
    char *xPart = strstr(ptr, "(x");
    char *yPart = strstr(ptr, "(y");
    char *equalSign = strchr(ptr, '=');

    if (!xPart || !yPart || !equalSign) return 0;

    // Extract h
    ptr = xPart + 2;
    if (*ptr == '-') {
        ptr++;
        *h = atof(ptr);
    } else if (*ptr == '+') {
        ptr++;
        *h = -atof(ptr);
    } else {
        *h = 0;
    }

    // Extract k
    ptr = yPart + 2;
    if (*ptr == '-') {
        ptr++;
        *k = atof(ptr);
    } else if (*ptr == '+') {
        ptr++;
        *k = -atof(ptr);
    } else {
        *k = 0;
    }

    // Extract r
    ptr = equalSign + 1;
    *r = sqrt(atof(ptr));

    return 1; // Parsing successful
}

int readEllipse(const char *equation, float *h, float *k, float *a, float *b) {
    // For simplicity, assume ellipse is centered at (h, k) with equation:
    // ((x - h)^2) / a^2 + ((y - k)^2) / b^2 = 1
    *h = 0; *k = 0; *a = 5; *b = 3; // Default values
    return 1;
}

int readParabola(const char *equation, float *a, float *b, float *c) {
    // Expected format y = ax^2 + bx + c
    *a = 1.0f; *b = 0.0f; *c = 0.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);

    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    // For simplicity, assign default values
    *a = 1.0f; *b = 0.0f; *c = 0.0f;
    return 1;
}

int readHyperbola(const char *equation, float *h, float *k, float *a, float *b) {
    // For simplicity, assume hyperbola is centered at (h, k) with equation:
    // ((x - h)^2) / a^2 - ((y - k)^2) / b^2 = 1
    *h = 0; *k = 0; *a = 5; *b = 3; // Default values
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
    if (strcmp(func, "custom_asin") == 0) {
        return customASin.A * asin(customASin.B * x + customASin.C) + customASin.D;
    }
    if (strcmp(func, "custom_acos") == 0) {
        return customACos.A * acos(customACos.B * x + customACos.C) + customACos.D;
    }
    if (strcmp(func, "custom_atan") == 0) {
        return customATan.A * atan(customATan.B * x + customATan.C) + customATan.D;
    }
    if (strcmp(func, "exponential") == 0) {
        return customExp.A * exp(customExp.B * x + customExp.C) + customExp.D;
    }
    if (strcmp(func, "logarithm") == 0) {
        return customLog.A * log10(customLog.B * x + customLog.C) + customLog.D;
    }
    if (strcmp(func, "natural_log") == 0) {
        return customLn.A * log(customLn.B * x + customLn.C) + customLn.D;
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

void plotCircle() {
    iSetColor(255, 0, 255); // Magenta
    float h = customCircle.h;
    float k = customCircle.k;
    float r = customCircle.r;

    iCircle(WINDOW_WIDTH / 2 + (h + offsetX) * scaleX, WINDOW_HEIGHT / 2 + (k + offsetY) * scaleY, r * scaleX);
}

void plotEllipse() {
    iSetColor(0, 255, 255); // Cyan
    float h = customEllipse.h;
    float k = customEllipse.k;
    float a = customEllipse.a;
    float b = customEllipse.b;

    iEllipse(WINDOW_WIDTH / 2 + (h + offsetX) * scaleX, WINDOW_HEIGHT / 2 + (k + offsetY) * scaleY, a * scaleX, b * scaleY);
}

void plotParabola() {
    iSetColor(128, 0, 128); // Purple
    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX;

    for (double x = startX; x <= endX; x += step) {
        double y1 = customParabola.a * x * x + customParabola.b * x + customParabola.c;
        double y2 = customParabola.a * (x + step) * (x + step) + customParabola.b * (x + step) + customParabola.c;

        if (!isfinite(y1) || !isfinite(y2)) {
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

void plotHyperbola() {
    iSetColor(255, 255, 0); // Yellow
    float h = customHyperbola.h;
    float k = customHyperbola.k;
    float a = customHyperbola.a;
    float b = customHyperbola.b;

    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX - h - 10;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX - h + 10;

    for (double x = startX; x <= endX; x += step) {
        double y1 = k + b * sqrt((x - h) * (x - h) / (a * a) - 1);
        double y2 = k + b * sqrt(((x + step) - h) * ((x + step) - h) / (a * a) - 1);

        double y3 = k - b * sqrt((x - h) * (x - h) / (a * a) - 1);
        double y4 = k - b * sqrt(((x + step) - h) * ((x + step) - h) / (a * a) - 1);

        if (isfinite(y1) && isfinite(y2)) {
            double screenX1 = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
            double screenY1 = WINDOW_HEIGHT / 2 + (y1 + offsetY) * scaleY;
            double screenX2 = WINDOW_WIDTH / 2 + (x + step + offsetX) * scaleX;
            double screenY2 = WINDOW_HEIGHT / 2 + (y2 + offsetY) * scaleY;
            iLine(screenX1, screenY1, screenX2, screenY2);
        }

        if (isfinite(y3) && isfinite(y4)) {
            double screenX1 = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
            double screenY1 = WINDOW_HEIGHT / 2 + (y3 + offsetY) * scaleY;
            double screenX2 = WINDOW_WIDTH / 2 + (x + step + offsetX) * scaleX;
            double screenY2 = WINDOW_HEIGHT / 2 + (y4 + offsetY) * scaleY;
            iLine(screenX1, screenY1, screenX2, screenY2);
        }
    }
}

void plotFunctions() {
    if (showSin) plotFunction("custom_sin", 0, 0, 255);     // Blue
    if (showCos) plotFunction("custom_cos", 255, 0, 0);     // Red
    if (showTan) plotFunction("custom_tan", 0, 255, 0);     // Green
    if (showASin) plotFunction("custom_asin", 255, 128, 128); // Light Red
    if (showACos) plotFunction("custom_acos", 128, 128, 255); // Light Blue
    if (showATan) plotFunction("custom_atan", 128, 255, 128); // Light Green
    if (showExp) plotFunction("exponential", 255, 165, 0);    // Orange
    if (showLog) plotFunction("logarithm", 255, 20, 147);     // Deep Pink
    if (showLn) plotFunction("natural_log", 75, 0, 130);      // Indigo
    if (showPoly) plotFunction("polynomial", 255, 128, 0);  // Orange

    if (showCircle) plotCircle();
    if (showEllipse) plotEllipse();
    if (showParabola) plotParabola();
    if (showHyperbola) plotHyperbola();
}

void drawUI() {
    iSetColor(255, 255, 255); // White text
    iText(10, 580, "Keys for Functions:");
    iText(10, 560, "'1' - Enter custom sin(x)");
    iText(10, 540, "'2' - Enter custom cos(x)");
    iText(10, 520, "'3' - Enter custom tan(x)");
    iText(10, 500, "'4' - Enter polynomial");
    iText(10, 480, "'5' - Enter circle");
    iText(10, 460, "'6' - Enter ellipse");
    iText(10, 440, "'7' - Enter parabola");
    iText(10, 420, "'8' - Enter hyperbola");
    iText(10, 400, "'9' - Enter exponential");
    iText(10, 380, "'0' - Enter logarithm");
    iText(10, 360, "'-' - Enter natural log");
    iText(10, 340, "'=' - Enter inverse trig");

    iText(10, 320, "'[' and ']' - Zoom in/out");
    iText(10, 300, "Arrow keys - Pan graph");
    iText(10, 280, "'g' - Toggle grid");
    iText(10, 260, "'q' - Exit");

    char zoomText[50];
    sprintf(zoomText, "Zoom: %.2f", scaleX);
    iText(10, 240, zoomText);

    if (isEnteringEquation) {
        iSetColor(200, 200, 200);
        iFilledRectangle(200, 500, 400, 30);
        iSetColor(0, 0, 0);
        iText(210, 510, equationInput);

        iSetColor(255, 255, 255);
        char instructions[200];
        sprintf(instructions, "Enter equation for %s function and press Enter.", currentFunction);
        iText(200, 540, instructions);
    }

    // Show current equations
    int yPos = 220;
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
    if (showASin) {
        char eqText[100];
        sprintf(eqText, "Arcsin: y = %.2f*arcsin(%.2f*x + %.2f) + %.2f", customASin.A, customASin.B, customASin.C, customASin.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showACos) {
        char eqText[100];
        sprintf(eqText, "Arccos: y = %.2f*arccos(%.2f*x + %.2f) + %.2f", customACos.A, customACos.B, customACos.C, customACos.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showATan) {
        char eqText[100];
        sprintf(eqText, "Arctan: y = %.2f*arctan(%.2f*x + %.2f) + %.2f", customATan.A, customATan.B, customATan.C, customATan.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showExp) {
        char eqText[100];
        sprintf(eqText, "Exp: y = %.2f*exp(%.2f*x + %.2f) + %.2f", customExp.A, customExp.B, customExp.C, customExp.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showLog) {
        char eqText[100];
        sprintf(eqText, "Log: y = %.2f*log(%.2f*x + %.2f) + %.2f", customLog.A, customLog.B, customLog.C, customLog.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showLn) {
        char eqText[100];
        sprintf(eqText, "Ln: y = %.2f*ln(%.2f*x + %.2f) + %.2f", customLn.A, customLn.B, customLn.C, customLn.D);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showPoly) {
        char eqText[200];
        sprintf(eqText, "Poly: y = %.2fx^4 + %.2fx^3 + %.2fx^2 + %.2fx + %.2f",
            customPoly.a4, customPoly.a3, customPoly.a2, customPoly.a1, customPoly.a0);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showCircle) {
        char eqText[100];
        sprintf(eqText, "Circle: (x - %.2f)^2 + (y - %.2f)^2 = %.2f^2", customCircle.h, customCircle.k, customCircle.r);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showEllipse) {
        char eqText[100];
        sprintf(eqText, "Ellipses: ((x - %.2f)^2)/%.2f^2 + ((y - %.2f)^2)/%.2f^2 = 1", customEllipse.h, customEllipse.a, customEllipse.k, customEllipse.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showParabola) {
        char eqText[100];
        sprintf(eqText, "Parabola: y = %.2fx^2 + %.2fx + %.2f", customParabola.a, customParabola.b, customParabola.c);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showHyperbola) {
        char eqText[100];
        sprintf(eqText, "Hyperbola: ((x - %.2f)^2)/%.2f^2 - ((y - %.2f)^2)/%.2f^2 = 1", customHyperbola.h, customHyperbola.a, customHyperbola.k, customHyperbola.b);
        iText(10, yPos, eqText);
    }
}

void iKeyboard(unsigned char key) {
    if (!isEnteringEquation && (key >= '0' && key <= '9' || key == '-' || key == '=')) {
        isEnteringEquation = true;
        equationInput[0] = '\0';

        switch (key) {
        case '1': strcpy(currentFunction, "sin"); showSin = false; break;
        case '2': strcpy(currentFunction, "cos"); showCos = false; break;
        case '3': strcpy(currentFunction, "tan"); showTan = false; break;
        case '4': strcpy(currentFunction, "poly"); showPoly = false; break;
        case '5': strcpy(currentFunction, "circle"); showCircle = false; break;
        case '6': strcpy(currentFunction, "ellipse"); showEllipse = false; break;
        case '7': strcpy(currentFunction, "parabola"); showParabola = false; break;
        case '8': strcpy(currentFunction, "hyperbola"); showHyperbola = false; break;
        case '9': strcpy(currentFunction, "exp"); showExp = false; break;
        case '0': strcpy(currentFunction, "log"); showLog = false; break;
        case '-': strcpy(currentFunction, "ln"); showLn = false; break;
        case '=': strcpy(currentFunction, "inverse_trig"); break;
        default: break;
        }
        return;
    }

    if (isEnteringEquation) {
        if (key == '\r') { // Enter key
            bool success = false;
            if (strcmp(currentFunction, "poly") == 0) {
                success = readPolynomial(equationInput, &customPoly.a4, &customPoly.a3, &customPoly.a2, &customPoly.a1, &customPoly.a0);
                if (success) showPoly = true;
            } else if (strcmp(currentFunction, "sin") == 0) {
                success = readTrigFunction(equationInput, &customSin.A, &customSin.B, &customSin.C, &customSin.D, "sin");
                if (success) showSin = true;
            } else if (strcmp(currentFunction, "cos") == 0) {
                success = readTrigFunction(equationInput, &customCos.A, &customCos.B, &customCos.C, &customCos.D, "cos");
                if (success) showCos = true;
            } else if (strcmp(currentFunction, "tan") == 0) {
                success = readTrigFunction(equationInput, &customTan.A, &customTan.B, &customTan.C, &customTan.D, "tan");
                if (success) showTan = true;
            } else if (strcmp(currentFunction, "circle") == 0) {
                success = readCircle(equationInput, &customCircle.h, &customCircle.k, &customCircle.r);
                if (success) showCircle = true;
            } else if (strcmp(currentFunction, "ellipse") == 0) {
                success = readEllipse(equationInput, &customEllipse.h, &customEllipse.k, &customEllipse.a, &customEllipse.b);
                if (success) showEllipse = true;
            } else if (strcmp(currentFunction, "parabola") == 0) {
                success = readParabola(equationInput, &customParabola.a, &customParabola.b, &customParabola.c);
                if (success) showParabola = true;
            } else if (strcmp(currentFunction, "hyperbola") == 0) {
                success = readHyperbola(equationInput, &customHyperbola.h, &customHyperbola.k, &customHyperbola.a, &customHyperbola.b);
                if (success) showHyperbola = true;
            } else if (strcmp(currentFunction, "exp") == 0) {
                success = readExponentialFunction(equationInput, &customExp.A, &customExp.B, &customExp.C, &customExp.D);
                if (success) showExp = true;
            } else if (strcmp(currentFunction, "log") == 0) {
                success = readLogFunction(equationInput, &customLog.A, &customLog.B, &customLog.C, &customLog.D, "log");
                if (success) showLog = true;
            } else if (strcmp(currentFunction, "ln") == 0) {
                success = readLogFunction(equationInput, &customLn.A, &customLn.B, &customLn.C, &customLn.D, "ln");
                if (success) showLn = true;
            } else if (strcmp(currentFunction, "inverse_trig") == 0) {
                // Prompt user to specify which inverse trig function
                // For simplicity, let's assume it's arcsin
                success = readInverseTrigFunction(equationInput, &customASin.A, &customASin.B, &customASin.C, &customASin.D, "arcsin");
                if (success) showASin = true;
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
    iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Graph Plotter with Enhanced Functionality");
    return 0;
}
