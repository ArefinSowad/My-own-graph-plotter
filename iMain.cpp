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
#define MINOR_GRID_SPACING 5
#define MAJOR_GRID_SPACING 10

#define MAIN_AXIS_THICKNESS 2.0     // Thickness for main axes
#define AXIS_LABEL_SPACING 5.0      // Space between axis labels (5 units)
#define LABEL_OFFSET 15             // Pixel offset for labels from axis
#define TICK_SIZE 5                 // Size of tick marks in pixels

// Constants for input handling
#define MAX_INPUT_LENGTH 100
#define MAX_COEFFICIENTS 10
#define MAX_SHAPE_NAME 20

// Constants for scaling
#define INITIAL_SCALE_X 25
#define INITIAL_SCALE_Y 25
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

bool isSettingColor = false;
bool showGridFlag = true;
bool isEnteringEquation = false;
bool isPanning = false;

char userInput[MAX_INPUT_LENGTH] = "";
char equationInput[MAX_INPUT_LENGTH] = "";
char currentFunction[20] = "";
char activeShape[MAX_SHAPE_NAME] = "";
char selectedFunction[20] = "";
char inputBuffer[100] = "";

int inputStep = 0; // 0: function name, 1: R, 2: G, 3: B
int inputIndex = 0;
int lastMouseX, lastMouseY;

double coefficients[MAX_COEFFICIENTS] = {0};

// Structure definitions

typedef struct {
    double r; // Red component (0-255)
    double g; // Green component (0-255)
    double b; // Blue component (0-255)
} Color;

typedef struct {
    float A; // Amplitude
    float B; // Frequency
    float C; // Phase shift
    float D; // Vertical shift
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

typedef struct {
    float A, B, C, D; // For y = A * exp(B * x + C) + D
} ExponentialFunction;

typedef struct {
    float A, B, C, D; // For y = A * log(B * x + C) + D
} LogFunction;

// After Color structure definition
typedef struct {
    char name[32];
    Color colors[14];  // One color for each function type
} ColorPreset;

#define NUM_PRESETS 5
ColorPreset colorPresets[NUM_PRESETS] = {
    {
        "Classic", {
            {0, 0, 255},      // Sin - Blue
            {255, 0, 0},      // Cos - Red
            {0, 255, 0},      // Tan - Green
            {255, 128, 128},  // ASin - Light Red
            {128, 128, 255},  // ACos - Light Blue
            {128, 255, 128},  // ATan - Light Green
            {255, 165, 0},    // Exp - Orange
            {255, 20, 147},   // Log - Deep Pink
            {75, 0, 130},     // Ln - Indigo
            {255, 128, 0},    // Poly - Orange
            {255, 0, 255},    // Circle - Magenta
            {0, 255, 255},    // Ellipse - Cyan
            {128, 0, 128},    // Parabola - Purple
            {255, 255, 0}     // Hyperbola - Yellow
        }
    },
    {
        "Neon", {
            {0, 255, 247},    // Sin - Neon Blue
            {255, 0, 247},    // Cos - Neon Pink
            {247, 255, 0},    // Tan - Neon Yellow
            {0, 255, 128},    // ASin - Neon Green
            {255, 128, 0},    // ACos - Neon Orange
            {128, 0, 255},    // ATan - Neon Purple
            {255, 0, 128},    // Exp - Neon Red
            {0, 255, 0},      // Log - Neon Lime
            {0, 128, 255},    // Ln - Neon Sky
            {255, 0, 0},      // Poly - Bright Red
            {0, 255, 255},    // Circle - Cyan
            {255, 255, 0},    // Ellipse - Yellow
            {255, 0, 255},    // Parabola - Magenta
            {0, 255, 128}     // Hyperbola - Spring Green
        }
    },
    {
        "Pastel", {
            {173, 216, 230},  // Sin - Light Blue
            {255, 182, 193},  // Cos - Light Pink
            {144, 238, 144},  // Tan - Light Green
            {221, 160, 221},  // ASin - Plum
            {176, 224, 230},  // ACos - Powder Blue
            {255, 218, 185},  // ATan - Peach
            {255, 192, 203},  // Exp - Pink
            {230, 230, 250},  // Log - Lavender
            {216, 191, 216},  // Ln - Thistle
            {240, 230, 140},  // Poly - Khaki
            {255, 228, 225},  // Circle - Misty Rose
            {176, 196, 222},  // Ellipse - Light Steel Blue
            {255, 240, 245},  // Parabola - Lavender Blush
            {245, 255, 250}   // Hyperbola - Mint Cream
        }
    },
    {
        "Monochrome", {
            {255, 255, 255},  // All white with different opacity
            {230, 230, 230},
            {200, 200, 200},
            {180, 180, 180},
            {160, 160, 160},
            {140, 140, 140},
            {120, 120, 120},
            {100, 100, 100},
            {80, 80, 80},
            {60, 60, 60},
            {40, 40, 40},
            {20, 20, 20},
            {10, 10, 10},
            {5, 5, 5}
        }
    },
    {
        "Dark", {
            {139, 0, 0},      // Dark Red
            {0, 100, 0},      // Dark Green
            {0, 0, 139},      // Dark Blue
            {139, 0, 139},    // Dark Magenta
            {0, 139, 139},    // Dark Cyan
            {139, 69, 19},    // Saddle Brown
            {72, 61, 139},    // Dark Slate Blue
            {85, 107, 47},    // Dark Olive Green
            {139, 69, 0},     // Dark Orange
            {128, 0, 0},      // Maroon
            {0, 0, 128},      // Navy
            {128, 0, 128},    // Purple
            {128, 128, 0},    // Olive
            {70, 130, 180}    // Steel Blue
        }
    }
};

// Add global variable for current preset
int currentPreset = 0;
bool showPresetMenu = false;


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
ExponentialFunction customExp = {1.0, 1.0, 0.0, 0.0};
LogFunction customLog = {1.0, 1.0, 0.0, 0.0};
LogFunction customLn = {1.0, 1.0, 0.0, 0.0};

// Initialize default colors for each function
Color colorSin = {0, 0, 255};          // Blue
Color colorCos = {255, 0, 0};          // Red
Color colorTan = {0, 255, 0};          // Green
Color colorASin = {255, 128, 128};     // Light Red
Color colorACos = {128, 128, 255};     // Light Blue
Color colorATan = {128, 255, 128};     // Light Green
Color colorExp = {255, 165, 0};        // Orange
Color colorLog = {255, 20, 147};       // Deep Pink
Color colorLn = {75, 0, 130};          // Indigo
Color colorPoly = {255, 128, 0};       // Orange
Color colorCircle = {255, 0, 255};     // Magenta
Color colorEllipse = {0, 255, 255};    // Cyan
Color colorParabola = {128, 0, 128};   // Purple
Color colorHyperbola = {255, 255, 0};  // Yellow

// Helper Functions

// Add function to apply color preset
void applyColorPreset(int presetIndex) {
    if (presetIndex < 0 || presetIndex >= NUM_PRESETS) return;
    
    colorSin = colorPresets[presetIndex].colors[0];
    colorCos = colorPresets[presetIndex].colors[1];
    colorTan = colorPresets[presetIndex].colors[2];
    colorASin = colorPresets[presetIndex].colors[3];
    colorACos = colorPresets[presetIndex].colors[4];
    colorATan = colorPresets[presetIndex].colors[5];
    colorExp = colorPresets[presetIndex].colors[6];
    colorLog = colorPresets[presetIndex].colors[7];
    colorLn = colorPresets[presetIndex].colors[8];
    colorPoly = colorPresets[presetIndex].colors[9];
    colorCircle = colorPresets[presetIndex].colors[10];
    colorEllipse = colorPresets[presetIndex].colors[11];
    colorParabola = colorPresets[presetIndex].colors[12];
    colorHyperbola = colorPresets[presetIndex].colors[13];
}


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

    // Handle coefficient A (amplitude)
    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    if (ptr != funcPtr) {
        if (ptr[0] == '-' && ptr + 1 == funcPtr) {
            *A = -1.0f;
        } else {
            char coeffStr[MAX_INPUT_LENGTH];
            strncpy(coeffStr, ptr, funcPtr - ptr);
            coeffStr[funcPtr - ptr] = '\0';
            if (strlen(coeffStr) > 0) {
                *A = atof(coeffStr);
            }
        }
    }

    // Find opening and closing parentheses
    ptr = funcPtr + strlen(funcType);
    if (*ptr == '(') ptr++;
    char *closePtr = strchr(ptr, ')');
    if (!closePtr) return 0;

    // Parse inner function (Bx + C)
    char innerFunc[MAX_INPUT_LENGTH];
    strncpy(innerFunc, ptr, closePtr - ptr);
    innerFunc[closePtr - ptr] = '\0';

    char *xPtr = strchr(innerFunc, 'x');
    if (xPtr) {
        // Handle B (coefficient of x)
        if (xPtr == innerFunc) {
            *B = 1.0f;
        } else if (xPtr == innerFunc + 1 && innerFunc[0] == '-') {
            *B = -1.0f;
        } else {
            char bStr[MAX_INPUT_LENGTH];
            strncpy(bStr, innerFunc, xPtr - innerFunc);
            bStr[xPtr - innerFunc] = '\0';
            if (strlen(bStr) > 0) {
                *B = atof(bStr);
            }
        }

        // Handle C (phase shift)
        ptr = xPtr + 1;
        if (*ptr == '+' || *ptr == '-') {
            *C = atof(ptr);
        }
    }

    // Handle D (vertical shift)
    ptr = closePtr + 1;
    while (*ptr == ' ') ptr++; // Skip spaces

    if (*ptr) {
        if (*ptr == '+') {
            *D = atof(ptr + 1);
        } else if (*ptr == '-') {
            *D = -atof(ptr + 1);
        }
    }

    return 1;
}


int readInverseTrigFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType) {
    *A = 1.0f; *B = 1.0f; *C = 0.0f; *D = 0.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strncpy(cleanEq, equation, MAX_INPUT_LENGTH - 1);
    removeWhitespaces(cleanEq);

    // Check for different inverse trig notations
    char *funcPtr = NULL;
    if (strstr(cleanEq, "arcsin")) {
        funcPtr = strstr(cleanEq, "arcsin");
        strcpy(funcType, "arcsin");
    } else if (strstr(cleanEq, "asin")) {
        funcPtr = strstr(cleanEq, "asin");
        strcpy(funcType, "asin");
    } else if (strstr(cleanEq, "arccos")) {
        funcPtr = strstr(cleanEq, "arccos");
        strcpy(funcType, "arccos");
    } else if (strstr(cleanEq, "acos")) {
        funcPtr = strstr(cleanEq, "acos");
        strcpy(funcType, "acos");
    } else if (strstr(cleanEq, "arctan")) {
        funcPtr = strstr(cleanEq, "arctan");
        strcpy(funcType, "arctan");
    } else if (strstr(cleanEq, "atan")) {
        funcPtr = strstr(cleanEq, "atan");
        strcpy(funcType, "atan");
    }

    if (!funcPtr) return 0;

    // Handle coefficient A (amplitude)
    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    if (ptr != funcPtr) {
        if (ptr[0] == '-' && ptr + 1 == funcPtr) {
            *A = -1.0f;
        } else {
            char coeffStr[MAX_INPUT_LENGTH];
            strncpy(coeffStr, ptr, funcPtr - ptr);
            coeffStr[funcPtr - ptr] = '\0';
            if (strlen(coeffStr) > 0) {
                *A = atof(coeffStr);
            }
        }
    }

    // Find opening and closing parentheses
    ptr = funcPtr + strlen(funcType);
    if (*ptr == '(') ptr++;
    char *closePtr = strchr(ptr, ')');
    if (!closePtr) return 0;

    // Parse inner function (Bx + C)
    char innerFunc[MAX_INPUT_LENGTH];
    strncpy(innerFunc, ptr, closePtr - ptr);
    innerFunc[closePtr - ptr] = '\0';

    char *xPtr = strchr(innerFunc, 'x');
    if (xPtr) {
        // Handle B (coefficient of x)
        if (xPtr == innerFunc) {
            *B = 1.0f;
        } else if (xPtr == innerFunc + 1 && innerFunc[0] == '-') {
            *B = -1.0f;
        } else {
            char bStr[MAX_INPUT_LENGTH];
            strncpy(bStr, innerFunc, xPtr - innerFunc);
            bStr[xPtr - innerFunc] = '\0';
            if (strlen(bStr) > 0) {
                *B = atof(bStr);
            }
        }

        // Handle C (horizontal shift)
        ptr = xPtr + 1;
        if (*ptr == '+' || *ptr == '-') {
            *C = atof(ptr);
        }
    }

    // Handle D (vertical shift)
    ptr = closePtr + 1;
    while (*ptr == ' ') ptr++; // Skip spaces

    if (*ptr) {
        if (*ptr == '+') {
            *D = atof(ptr + 1);
        } else if (*ptr == '-') {
            *D = -atof(ptr + 1);
        }
    }

    return 1;
}


int readExponentialFunction(const char* equation, float *A, float *B, float *C, float *D) {
    *A = 1.0f; *B = 1.0f; *C = 0.0f; *D = 0.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strncpy(cleanEq, equation, MAX_INPUT_LENGTH - 1);
    removeWhitespaces(cleanEq);

    char *expPtr = strstr(cleanEq, "exp");
    if (!expPtr) return 0;

    // Handle coefficient A
    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    if (ptr != expPtr) {
        if (ptr[0] == '-' && ptr + 1 == expPtr) {
            *A = -1.0f;
        } else {
            char coeffStr[MAX_INPUT_LENGTH];
            strncpy(coeffStr, ptr, expPtr - ptr);
            coeffStr[expPtr - ptr] = '\0';
            if (strlen(coeffStr) > 0) {
                *A = atof(coeffStr);
            }
        }
    }

    // Find opening and closing parentheses
    ptr = expPtr + 3; // Move past "exp"
    if (*ptr == '(') ptr++;
    char *closePtr = strchr(ptr, ')');
    if (!closePtr) return 0;

    // Parse inner function (Bx + C)
    char innerFunc[MAX_INPUT_LENGTH];
    strncpy(innerFunc, ptr, closePtr - ptr);
    innerFunc[closePtr - ptr] = '\0';

    char *xPtr = strchr(innerFunc, 'x');
    if (xPtr) {
        // Handle B
        if (xPtr == innerFunc) {
            *B = 1.0f;
        } else if (xPtr == innerFunc + 1 && innerFunc[0] == '-') {
            *B = -1.0f;
        } else {
            char bStr[MAX_INPUT_LENGTH];
            strncpy(bStr, innerFunc, xPtr - innerFunc);
            bStr[xPtr - innerFunc] = '\0';
            if (strlen(bStr) > 0) {
                *B = atof(bStr);
            }
        }

        // Handle C
        ptr = xPtr + 1;
        if (*ptr == '+' || *ptr == '-') {
            *C = atof(ptr);
        }
    }

    // Handle D (vertical shift)
    ptr = closePtr + 1;
    while (*ptr == ' ') ptr++;

    if (*ptr) {
        if (*ptr == '+') {
            *D = atof(ptr + 1);
        } else if (*ptr == '-') {
            *D = -atof(ptr + 1);
        }
    }

    return 1;
}


int readLogFunction(const char* equation, float *A, float *B, float *C, float *D, char *funcType) {
    *A = 1.0f; *B = 1.0f; *C = 0.0f; *D = 0.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strncpy(cleanEq, equation, MAX_INPUT_LENGTH - 1);
    removeWhitespaces(cleanEq);

    char *logPtr = strstr(cleanEq, funcType);
    if (!logPtr) return 0;

    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    // Handle coefficient A (before log/ln)
    if (ptr != logPtr) {
        if (ptr[0] == '-' && ptr + 1 == logPtr) {
            *A = -1.0f;
        } else {
            char coeffStr[MAX_INPUT_LENGTH];
            strncpy(coeffStr, ptr, logPtr - ptr);
            coeffStr[logPtr - ptr] = '\0';
            if (strlen(coeffStr) > 0) {
                *A = atof(coeffStr);
            }
        }
    }

    // Find the opening parenthesis
    ptr = logPtr + strlen(funcType);
    if (*ptr == '(') ptr++;

    // Find the closing parenthesis
    char *closePtr = strchr(ptr, ')');
    if (!closePtr) return 0;

    // Parse the inner part (Bx + C)
    char innerFunc[MAX_INPUT_LENGTH];
    strncpy(innerFunc, ptr, closePtr - ptr);
    innerFunc[closePtr - ptr] = '\0';

    char *xPtr = strchr(innerFunc, 'x');
    if (xPtr) {
        // Handle coefficient B (before x)
        if (xPtr == innerFunc) {
            *B = 1.0f;
        } else if (xPtr == innerFunc + 1 && innerFunc[0] == '-') {
            *B = -1.0f;
        } else {
            char bStr[MAX_INPUT_LENGTH];
            strncpy(bStr, innerFunc, xPtr - innerFunc);
            bStr[xPtr - innerFunc] = '\0';
            if (strlen(bStr) > 0) {
                *B = atof(bStr);
            }
        }

        // Handle C (after x)
        ptr = xPtr + 1;
        if (*ptr == '+' || *ptr == '-') {
            *C = atof(ptr);
        }
    }

    // Handle D (vertical shift) - after the closing parenthesis
    ptr = closePtr + 1;
    while (*ptr == ' ') ptr++; // Skip any spaces

    if (*ptr) {
        if (*ptr == '+') {
            *D = atof(ptr + 1);
        } else if (*ptr == '-') {
            *D = -atof(ptr + 1);
        }
    }

    return 1;
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
    *h = 0.0f; *k = 0.0f; *a = 1.0f; *b = 1.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);

    // Parse x-term
    char *xPart = strstr(cleanEq, "(x");
    if (!xPart) return 0;

    // Parse h (x-center)
    char *ptr = xPart + 2;
    if (*ptr == '-') {
        ptr++;
        *h = atof(ptr);
    } else if (*ptr == '+') {
        ptr++;
        *h = -atof(ptr);
    }

    // Find a^2 denominator
    char *denomPtr = strstr(ptr, "/");
    if (denomPtr) {
        ptr = denomPtr + 1;
        char *endPtr = strchr(ptr, '+');
        if (endPtr) {
            char aStr[MAX_INPUT_LENGTH];
            strncpy(aStr, ptr, endPtr - ptr);
            aStr[endPtr - ptr] = '\0';
            *a = sqrt(atof(aStr));
        }
    }

    // Parse y-term
    char *yPart = strstr(cleanEq, "(y");
    if (!yPart) return 0;

    // Parse k (y-center)
    ptr = yPart + 2;
    if (*ptr == '-') {
        ptr++;
        *k = atof(ptr);
    } else if (*ptr == '+') {
        ptr++;
        *k = -atof(ptr);
    }

    // Find b^2 denominator
    denomPtr = strstr(ptr, "/");
    if (denomPtr) {
        ptr = denomPtr + 1;
        *b = sqrt(atof(ptr));
    }

    return 1;
}
int readParabola(const char *equation, float *a, float *b, float *c) {
    *a = 0.0f; *b = 0.0f; *c = 0.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);

    char *ptr = cleanEq;
    if (strncmp(ptr, "y=", 2) == 0) ptr += 2;

    // Parse quadratic term (ax^2)
    char *x2Ptr = strstr(ptr, "x^2");
    if (x2Ptr) {
        if (x2Ptr == ptr) {
            *a = 1.0f;
        } else if (x2Ptr == ptr + 1 && ptr[0] == '-') {
            *a = -1.0f;
        } else {
            char aStr[MAX_INPUT_LENGTH];
            strncpy(aStr, ptr, x2Ptr - ptr);
            aStr[x2Ptr - ptr] = '\0';
            *a = atof(aStr);
        }
        ptr = x2Ptr + 3;
    }

    // Parse linear term (bx)
    char *xPtr = strstr(ptr, "x");
    if (xPtr && xPtr[-1] != '^') {  // Make sure it's not part of x^2
        if (xPtr == ptr) {
            *b = 1.0f;
        } else if (xPtr == ptr + 1 && ptr[0] == '-') {
            *b = -1.0f;
        } else if (xPtr > ptr) {
            char bStr[MAX_INPUT_LENGTH];
            strncpy(bStr, ptr, xPtr - ptr);
            bStr[xPtr - ptr] = '\0';
            *b = atof(bStr);
        }
        ptr = xPtr + 1;
    }

    // Parse constant term (c)
    if (*ptr == '+' || *ptr == '-' || isdigit(*ptr)) {
        *c = atof(ptr);
    }

    return 1;
}

int readHyperbola(const char *equation, float *h, float *k, float *a, float *b) {
    *h = 0.0f; *k = 0.0f; *a = 1.0f; *b = 1.0f;
    char cleanEq[MAX_INPUT_LENGTH];
    strcpy(cleanEq, equation);
    removeWhitespaces(cleanEq);

    // Similar to ellipse but with minus sign between terms
    char *xPart = strstr(cleanEq, "(x");
    if (!xPart) return 0;

    // Parse h (x-center)
    char *ptr = xPart + 2;
    if (*ptr == '-') {
        ptr++;
        *h = atof(ptr);
    } else if (*ptr == '+') {
        ptr++;
        *h = -atof(ptr);
    }

    // Find a^2 denominator
    char *denomPtr = strstr(ptr, "/");
    if (denomPtr) {
        ptr = denomPtr + 1;
        char *endPtr = strchr(ptr, '-');  // Note: minus instead of plus
        if (endPtr) {
            char aStr[MAX_INPUT_LENGTH];
            strncpy(aStr, ptr, endPtr - ptr);
            aStr[endPtr - ptr] = '\0';
            *a = sqrt(atof(aStr));
        }
    }

    // Parse y-term
    char *yPart = strstr(cleanEq, "(y");
    if (!yPart) return 0;

    // Parse k (y-center)
    ptr = yPart + 2;
    if (*ptr == '-') {
        ptr++;
        *k = atof(ptr);
    } else if (*ptr == '+') {
        ptr++;
        *k = -atof(ptr);
    }

    // Find b^2 denominator
    denomPtr = strstr(ptr, "/");
    if (denomPtr) {
        ptr = denomPtr + 1;
        *b = sqrt(atof(ptr));
    }

    return 1;
}

void drawGrid(double minorSpacing, double majorSpacing) {
    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX;
    double startY = -offsetY - (WINDOW_HEIGHT / 2) / scaleY;
    double endY = -offsetY + (WINDOW_HEIGHT / 2) / scaleY;
    
    // Get axis positions
    double axisX = WINDOW_WIDTH / 2 + offsetX * scaleX;
    double axisY = WINDOW_HEIGHT / 2 + offsetY * scaleY;

    // Draw minor grid lines if grid is enabled
    if (showGridFlag) {
        iSetColor(GRID_COLOR_R, GRID_COLOR_G, GRID_COLOR_B);
        
        // Draw vertical grid lines every 5 units
        for (double x = floor(startX / AXIS_LABEL_SPACING) * AXIS_LABEL_SPACING; x <= endX; x += AXIS_LABEL_SPACING) {
            if (fabs(x) > 1e-10) {  // Skip the main axis line
                double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
                iLine(screenX, 0, screenX, WINDOW_HEIGHT);
            }
        }

        // Draw horizontal grid lines every 5 units
        for (double y = floor(startY / AXIS_LABEL_SPACING) * AXIS_LABEL_SPACING; y <= endY; y += AXIS_LABEL_SPACING) {
            if (fabs(y) > 1e-10) {  // Skip the main axis line
                double screenY = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
                iLine(0, screenY, WINDOW_WIDTH, screenY);
            }
        }
    }

    // Always draw main axes with increased thickness
    iSetColor(MAIN_AXIS_COLOR_R, MAIN_AXIS_COLOR_G, MAIN_AXIS_COLOR_B);
    
    // Draw thick X-axis
    for (int i = 0; i < MAIN_AXIS_THICKNESS; i++) {
        iLine(0, axisY + i - MAIN_AXIS_THICKNESS/2, WINDOW_WIDTH, axisY + i - MAIN_AXIS_THICKNESS/2);
    }

    // Draw thick Y-axis
    for (int i = 0; i < MAIN_AXIS_THICKNESS; i++) {
        iLine(axisX + i - MAIN_AXIS_THICKNESS/2, 0, axisX + i - MAIN_AXIS_THICKNESS/2, WINDOW_HEIGHT);
    }

    // Always draw coordinate labels and tick marks
    iSetColor(255, 255, 255);  // White color for labels

    // X-axis labels and ticks
    for (double x = floor(startX / AXIS_LABEL_SPACING) * AXIS_LABEL_SPACING; x <= endX; x += AXIS_LABEL_SPACING) {
        if (fabs(x) > 1e-10) {  // Skip 0 to avoid overlapping labels
            double screenX = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
            
            // Draw tick marks
            iLine(screenX, axisY - TICK_SIZE/2, screenX, axisY + TICK_SIZE/2);
            
            // Draw coordinate labels
            char label[20];
            sprintf(label, "%.0f", x);
            iText(screenX - strlen(label) * 3, axisY - LABEL_OFFSET, label);
        }
    }

    // Y-axis labels and ticks
    for (double y = floor(startY / AXIS_LABEL_SPACING) * AXIS_LABEL_SPACING; y <= endY; y += AXIS_LABEL_SPACING) {
        if (fabs(y) > 1e-10) {  // Skip 0 to avoid overlapping labels
            double screenY = WINDOW_HEIGHT / 2 + (y + offsetY) * scaleY;
            
            // Draw tick marks
            iLine(axisX - TICK_SIZE/2, screenY, axisX + TICK_SIZE/2, screenY);
            
            // Draw coordinate labels
            char label[20];
            sprintf(label, "%.0f", y);
            iText(axisX + LABEL_OFFSET, screenY - 5, label);
        }
    }

    // Draw origin label (0)
    iText(axisX + LABEL_OFFSET, axisY - LABEL_OFFSET, "0");
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
        double input = customASin.B * x + customASin.C;
        if (input < -1 || input > 1) return INFINITY;
        return customASin.A * asin(input) + customASin.D;
    }
    if (strcmp(func, "custom_acos") == 0) {
        double input = customACos.B * x + customACos.C;
        if (input < -1 || input > 1) return INFINITY;
        return customACos.A * acos(input) + customACos.D;
    }
    if (strcmp(func, "custom_atan") == 0) {
        double input = customATan.B * x + customATan.C;
        return customATan.A * atan(input) + customATan.D;
    }
    if (strcmp(func, "exponential") == 0) {
        return customExp.A * exp(customExp.B * x + customExp.C) + customExp.D;
    }
    if (strcmp(func, "logarithm") == 0) {
        double input = customLog.B * x + customLog.C;
        if (input <= 0) return INFINITY;
        return customLog.A * log10(input) + customLog.D;
    }
    if (strcmp(func, "natural_log") == 0) {
        double input = customLn.B * x + customLn.C;
        if (input <= 0) return INFINITY;
        return customLn.A * log(input) + customLn.D;
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

// Plot Circle with dynamic color
void plotCircle(double r, double g, double b) {
    iSetColor(r, g, b);
    float h = customCircle.h;
    float k = customCircle.k;
    float radius = customCircle.r * scaleX;

    // Draw the circle
    for(int i = 0; i < 360; i++) {
        double theta1 = i * (3.14159265 / 180.0);
        double theta2 = (i+1) * (3.14159265 / 180.0);
        double x1 = WINDOW_WIDTH / 2 + (h + radius * cos(theta1)) * scaleX;
        double y1 = WINDOW_HEIGHT / 2 + (k + radius * sin(theta1)) * scaleY;
        double x2 = WINDOW_WIDTH / 2 + (h + radius * cos(theta2)) * scaleX;
        double y2 = WINDOW_HEIGHT / 2 + (k + radius * sin(theta2)) * scaleY;
        iLine(x1, y1, x2, y2);
    }
}

// Plot Ellipse with dynamic color
void plotEllipse(double r, double g, double b) {
    iSetColor(r, g, b);
    float h = customEllipse.h;
    float k = customEllipse.k;
    float a = customEllipse.a * scaleX;
    float b_axis = customEllipse.b * scaleY;

    // Draw the ellipse
    for(int i = 0; i < 360; i++) {
        double theta1 = i * (3.14159265 / 180.0);
        double theta2 = (i+1) * (3.14159265 / 180.0);
        double x1 = WINDOW_WIDTH / 2 + (h + a * cos(theta1)) * scaleX;
        double y1 = WINDOW_HEIGHT / 2 + (k + b_axis * sin(theta1)) * scaleY;
        double x2 = WINDOW_WIDTH / 2 + (h + a * cos(theta2)) * scaleX;
        double y2 = WINDOW_HEIGHT / 2 + (k + b_axis * sin(theta2)) * scaleY;
        iLine(x1, y1, x2, y2);
    }
}

// Plot Parabola with dynamic color
void plotParabola(double r, double g, double b) {
    iSetColor(r, g, b);
    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX;

    for (double x = startX; x <= endX; x += step) {
        double y1 = customParabola.a * x * x + customParabola.b * x + customParabola.c;
        double y2 = customParabola.a * (x + step) * (x + step) + customParabola.b * (x + step) + customParabola.c;

        if (!isfinite(y1) || !isfinite(y2)) continue;

        double screenX1 = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
        double screenY1 = WINDOW_HEIGHT / 2 + (y1 + offsetY) * scaleY;
        double screenX2 = WINDOW_WIDTH / 2 + (x + step + offsetX) * scaleX;
        double screenY2 = WINDOW_HEIGHT / 2 + (y2 + offsetY) * scaleY;

        if ((screenX1 < 0 || screenX1 > WINDOW_WIDTH) && (screenX2 < 0 || screenX2 > WINDOW_WIDTH)) continue;
        if ((screenY1 < 0 || screenY1 > WINDOW_HEIGHT) && (screenY2 < 0 || screenY2 > WINDOW_HEIGHT)) continue;

        iLine(screenX1, screenY1, screenX2, screenY2);
    }
}

// Plot Hyperbola with dynamic color
void plotHyperbola(double r, double g, double b) {
    iSetColor(r, g, b);
    float h = customHyperbola.h;
    float k = customHyperbola.k;
    float a = customHyperbola.a * scaleX;
    float b_axis = customHyperbola.b * scaleY;

    double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX - h - 10;
    double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX - h + 10;

    for (double x = startX; x <= endX; x += step) {
        double denominator = (x / a) * (x / a) - 1;
        if (denominator <= 0) continue;

        double y1 = k + b_axis * sqrt(denominator);
        double y2 = k + b_axis * sqrt(((x + step) / a) * ((x + step) / a) - 1);

        double y3 = k - b_axis * sqrt(denominator);
        double y4 = k - b_axis * sqrt(((x + step) / a) * ((x + step) / a) - 1);

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
    if (showSin) plotFunction("custom_sin", colorSin.r, colorSin.g, colorSin.b);
    if (showCos) plotFunction("custom_cos", colorCos.r, colorCos.g, colorCos.b);
    if (showTan) plotFunction("custom_tan", colorTan.r, colorTan.g, colorTan.b);
    if (showASin) plotFunction("custom_asin", colorASin.r, colorASin.g, colorASin.b);
    if (showACos) plotFunction("custom_acos", colorACos.r, colorACos.g, colorACos.b);
    if (showATan) plotFunction("custom_atan", colorATan.r, colorATan.g, colorATan.b);
    if (showExp) plotFunction("exponential", colorExp.r, colorExp.g, colorExp.b);
    if (showLog) plotFunction("logarithm", colorLog.r, colorLog.g, colorLog.b);
    if (showLn) plotFunction("natural_log", colorLn.r, colorLn.g, colorLn.b);
    if (showPoly) plotFunction("polynomial", colorPoly.r, colorPoly.g, colorPoly.b);
    
    if (showCircle) {
        iSetColor(colorCircle.r, colorCircle.g, colorCircle.b);
        float h = customCircle.h;
        float k = customCircle.k;
        float r = customCircle.r;
        iCircle(WINDOW_WIDTH / 2 + (h + offsetX) * scaleX, WINDOW_HEIGHT / 2 + (k + offsetY) * scaleY, r * scaleX);
    }
    
    if (showEllipse) {
        iSetColor(colorEllipse.r, colorEllipse.g, colorEllipse.b);
        float h = customEllipse.h;
        float k = customEllipse.k;
        float a = customEllipse.a;
        float b = customEllipse.b;
        iEllipse(WINDOW_WIDTH / 2 + (h + offsetX) * scaleX, WINDOW_HEIGHT / 2 + (k + offsetY) * scaleY, a * scaleX, b * scaleY);
    }
    
    if (showParabola) {
        iSetColor(colorParabola.r, colorParabola.g, colorParabola.b);
        double startX = -offsetX - (WINDOW_WIDTH / 2) / scaleX;
        double endX = -offsetX + (WINDOW_WIDTH / 2) / scaleX;

        for (double x = startX; x <= endX; x += step) {
            double y1 = customParabola.a * x * x + customParabola.b * x + customParabola.c;
            double y2 = customParabola.a * (x + step) * (x + step) + customParabola.b * (x + step) + customParabola.c;

            if (!isfinite(y1) || !isfinite(y2)) continue;

            double screenX1 = WINDOW_WIDTH / 2 + (x + offsetX) * scaleX;
            double screenY1 = WINDOW_HEIGHT / 2 + (y1 + offsetY) * scaleY;
            double screenX2 = WINDOW_WIDTH / 2 + (x + step + offsetX) * scaleX;
            double screenY2 = WINDOW_HEIGHT / 2 + (y2 + offsetY) * scaleY;

            if ((screenX1 < 0 || screenX1 > WINDOW_WIDTH) && (screenX2 < 0 || screenX2 > WINDOW_WIDTH)) continue;
            if ((screenY1 < 0 || screenY1 > WINDOW_HEIGHT) && (screenY2 < 0 || screenY2 > WINDOW_HEIGHT)) continue;

            iLine(screenX1, screenY1, screenX2, screenY2);
        }
    }
    
    if (showHyperbola) {
        iSetColor(colorHyperbola.r, colorHyperbola.g, colorHyperbola.b);
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
}

void drawUI() {
    iSetColor(255, 255, 255);
    iText(10, 580, "Function Keys:");
    iText(10, 560, "'1' - Sine:          y = A*sin(B*x + C) + D");
    iText(10, 540, "'2' - Cosine:        y = A*cos(B*x + C) + D");
    iText(10, 520, "'3' - Tangent:       y = A*tan(B*x + C) + D");
    iText(10, 500, "'4' - Polynomial:    y = ax^4 + bx^3 + cx^2 + dx + e");
    iText(10, 480, "'5' - Circle:        (x-h)^2 + (y-k)^2 = r^2");
    iText(10, 460, "'6' - Ellipse:       ((x-h)^2)/a^2 + ((y-k)^2)/b^2 = 1");
    iText(10, 440, "'7' - Parabola:      y = ax^2 + bx + c");
    iText(10, 420, "'8' - Hyperbola:     ((x-h)^2)/a^2 - ((y-k)^2)/b^2 = 1");
    iText(10, 400, "'9' - Exponential:   y = A*exp(B*x + C) + D");
    iText(10, 380, "'0' - Logarithm:     y = A*log(B*x + C) + D");
    iText(10, 360, "'-' - Natural Log:   y = A*ln(B*x + C) + D");
    iText(10, 340, "'=' - Inverse Trig:  y = A*arcsin(B*x + C) + D");

    iText(10, 320, "'[' and ']' - Zoom in/out");
    iText(10, 300, "Arrow keys - Pan graph");
    iText(10, 280, "'g' - Toggle grid");
    iText(10, 260, "'q' - Exit");

    char zoomText[50];
    sprintf(zoomText, "Zoom: %.2f", scaleX);
    iText(10, 240, zoomText);

    // Add color setting instructions
    if (isSettingColor) {
        iSetColor(255, 255, 255); // White text
        if (inputStep == 0) {
            iText(200, 500, "Enter function name (sin, cos, tan, asin, acos, atan, exp, log, ln, poly, circle, ellipse, parabola, hyperbola):");
        }
        else if (inputStep == 1) {
            iText(200, 480, "Enter Red value (0-255):");
        }
        else if (inputStep == 2) {
            iText(200, 460, "Enter Green value (0-255):");
        }
        else if (inputStep == 3) {
            iText(200, 440, "Enter Blue value (0-255):");
        }
        // Display the input buffer
        iSetColor(200, 200, 200); // Light gray background for input
        iFilledRectangle(200, 420, 400, 30);
        iSetColor(0, 0, 0); // Black text
        iText(210, 430, inputBuffer);
    }

    if (isEnteringEquation) {
        iSetColor(200, 200, 200);
        iFilledRectangle(200, 500, 400, 30);
        iSetColor(0, 0, 0);
        iText(210, 510, equationInput);

        iSetColor(255, 255, 255);
        char instructions[200];
        sprintf(instructions, "Enter equation for %s function and press Enter.", currentFunction);
        iText(200, 540, instructions);

        // Display Format and Example
        iSetColor(150, 150, 150); // Light gray for format text
        char format[100] = "";
        char example[100] = "";
        if (strcmp(currentFunction, "inverse_trig_menu") == 0) {
            iSetColor(255, 255, 255);
            iText(200, 560, "Choose inverse trig function:");
            iText(200, 540, "1 - arcsin");
            iText(200, 520, "2 - arccos");
            iText(200, 500, "3 - arctan");
            iText(200, 480, "Press ESC to cancel");
        }
        if (strcmp(currentFunction, "sin") == 0) {
            strcpy(format, "Format: y = A*sin(B*x + C) + D");
            strcpy(example, "Example: y = 2*sin(1.5*x + 0.5) + 1");
        }
        else if (strcmp(currentFunction, "cos") == 0) {
            strcpy(format, "Format: y = A*cos(B*x + C) + D");
            strcpy(example, "Example: y = 3*cos(2*x - 1) + 0");
        }
        else if (strcmp(currentFunction, "tan") == 0) {
            strcpy(format, "Format: y = A*tan(B*x + C) + D");
            strcpy(example, "Example: y = 1*tan(1*x + 0) + 0");
        }
        else if (strcmp(currentFunction, "poly") == 0) {
            strcpy(format, "Format: y = ax^4 + bx^3 + cx^2 + dx + e");
            strcpy(example, "Example: y = 2*x^4 - 3*x^3 + x^2 + 5*x - 6");
        }
        else if (strcmp(currentFunction, "circle") == 0) {
            strcpy(format, "Format: (x - h)^2 + (y - k)^2 = r^2");
            strcpy(example, "Example: (x - 2)^2 + (y + 1)^2 = 25");
        }
        else if (strcmp(currentFunction, "log") == 0) {
            strcpy(format, "Format: y = [A]log([B]x + [C]) + [D]");
            strcpy(example, "Example: y = log(x) or y = 2log(3x + 1) + 2");
        }
        else if (strcmp(currentFunction, "ln") == 0) {
            strcpy(format, "Format: y = [A]ln([B]x + [C]) + [D]");
            strcpy(example, "Example: y = ln(x) or y = 2ln(3x + 1) + 2");
        }
        else if (strcmp(currentFunction, "parabola") == 0) {
            strcpy(format, "Format: y = [a]x^2 + [b]x + [c]");
            strcpy(example, "Example: y = x^2 or y = -2x^2 + 3x + 1");
        }
        else if (strcmp(currentFunction, "ellipse") == 0) {
            strcpy(format, "Format: ((x-[h])^2)/[a]^2 + ((y-[k])^2)/[b]^2 = 1");
            strcpy(example, "Example: ((x)^2)/4 + ((y)^2)/9 = 1");
        }
        else if (strcmp(currentFunction, "hyperbola") == 0) {
            strcpy(format, "Format: ((x-[h])^2)/[a]^2 - ((y-[k])^2)/[b]^2 = 1");
            strcpy(example, "Example: ((x)^2)/4 - ((y)^2)/9 = 1");
        }
        else if (strcmp(currentFunction, "exp") == 0) {
            strcpy(format, "Format: y = A*exp(B*x + C) + D");
            strcpy(example, "Example: y = 2*exp(1.5*x - 1) + 0");
        }


        else if (strcmp(currentFunction, "arcsin") == 0) {
            strcpy(format, "Format: y = [A]arcsin([B]x + [C]) + [D]");
            strcpy(example, "Example: y = arcsin(x) or y = 2arcsin(0.5x) + 1");
        }
        else if (strcmp(currentFunction, "arccos") == 0) {
            strcpy(format, "Format: y = [A]arccos([B]x + [C]) + [D]");
            strcpy(example, "Example: y = arccos(x) or y = 2arccos(0.5x) + 1");
        }
        else if (strcmp(currentFunction, "arctan") == 0) {
            strcpy(format, "Format: y = [A]arctan([B]x + [C]) + [D]");
            strcpy(example, "Example: y = arctan(x) or y = 2arctan(2x) - 1");
        }

        // Display format and example
        iText(200, 520, format);
        iText(200, 500, example);

        // Add explanation of parameters
        iSetColor(200, 200, 200);
        if (strstr(currentFunction, "sin") || strstr(currentFunction, "cos") || 
            strstr(currentFunction, "tan") || strcmp(currentFunction, "exp") == 0 || 
            strcmp(currentFunction, "log") == 0 || strcmp(currentFunction, "ln") == 0) {
            iText(200, 480, "A = amplitude, B = frequency/scale, C = phase/shift, D = vertical shift");
        }
        else if (strcmp(currentFunction, "circle") == 0) {
            iText(200, 480, "h,k = center coordinates, r = radius");
        }
        else if (strcmp(currentFunction, "ellipse") == 0 || strcmp(currentFunction, "hyperbola") == 0) {
            iText(200, 480, "h,k = center coordinates, a,b = semi-major/minor axes");
        }
        else if (strcmp(currentFunction, "parabola") == 0) {
            iText(200, 480, "a = vertical stretch, b = horizontal shift, c = vertical shift");
        }
        else if (strcmp(currentFunction, "poly") == 0) {
            iText(200, 480, "Enter coefficients for each power of x (use 0 for missing terms)");
        }
    }

    // Add color preset menu
    if (showPresetMenu) {
        iSetColor(0, 0, 0);
        iFilledRectangle(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 200, 190, 190);
        iSetColor(255, 255, 255);
        iText(WINDOW_WIDTH - 190, WINDOW_HEIGHT - 30, "Color Presets (0-4):");
        for (int i = 0; i < NUM_PRESETS; i++) {
            char presetText[50];
            sprintf(presetText, "%d: %s", i, colorPresets[i].name);
            iText(WINDOW_WIDTH - 190, WINDOW_HEIGHT - 60 - (i * 20), presetText);
        }
        iText(WINDOW_WIDTH - 190, WINDOW_HEIGHT - 180, "Press ESC to close");
    }

    // Add preset menu hint
    iSetColor(255, 255, 255);
    iText(10, 220, "Press 'p' for color presets");

    // Show current equations
    int yPos = 220;
    if (showSin) {
        char eqText[100];
        sprintf(eqText, "Sin: y = %.2f*sin(%.2f*x + %.2f) + %.2f", customSin.A, customSin.B, customSin.C, customSin.D);
        sprintf(eqText, "Sin Color: R=%.0f G=%.0f B=%.0f", colorSin.r, colorSin.g, colorSin.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showCos) {
        char eqText[100];
        sprintf(eqText, "Cos: y = %.2f*cos(%.2f*x + %.2f) + %.2f", customCos.A, customCos.B, customCos.C, customCos.D);
        sprintf(eqText, "Cos Color: R=%.0f G=%.0f B=%.0f", colorCos.r, colorCos.g, colorCos.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showTan) {
        char eqText[100];
        sprintf(eqText, "Tan: y = %.2f*tan(%.2f*x + %.2f) + %.2f", customTan.A, customTan.B, customTan.C, customTan.D);
        sprintf(eqText, "Tan Color: R=%.0f G=%.0f B=%.0f", colorTan.r, colorTan.g, colorTan.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showASin) {
        char eqText[100];
        sprintf(eqText, "Arcsin: y = %.2f*arcsin(%.2f*x + %.2f) + %.2f", customASin.A, customASin.B, customASin.C, customASin.D);
        sprintf(eqText, "ASin Color: R=%.0f G=%.0f B=%.0f", colorASin.r, colorASin.g, colorASin.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showACos) {
        char eqText[100];
        sprintf(eqText, "Arccos: y = %.2f*arccos(%.2f*x + %.2f) + %.2f", customACos.A, customACos.B, customACos.C, customACos.D);
        sprintf(eqText, "ACos Color: R=%.0f G=%.0f B=%.0f", colorACos.r, colorACos.g, colorACos.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showATan) {
        char eqText[100];
        sprintf(eqText, "Arctan: y = %.2f*arctan(%.2f*x + %.2f) + %.2f", customATan.A, customATan.B, customATan.C, customATan.D);
        sprintf(eqText, "ATan Color: R=%.0f G=%.0f B=%.0f", colorATan.r, colorATan.g, colorATan.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showExp) {
        char eqText[100];
        sprintf(eqText, "Exp: y = %.2f*exp(%.2f*x + %.2f) + %.2f", customExp.A, customExp.B, customExp.C, customExp.D);
        sprintf(eqText, "Exponential Color: R=%.0f G=%.0f B=%.0f", colorExp.r, colorExp.g, colorExp.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showLog) {
        char eqText[100];
        sprintf(eqText, "Log: y = %.2f*log(%.2f*x + %.2f) + %.2f", customLog.A, customLog.B, customLog.C, customLog.D);
        sprintf(eqText, "Logarithm Color: R=%.0f G=%.0f B=%.0f", colorLog.r, colorLog.g, colorLog.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showLn) {
        char eqText[100];
        sprintf(eqText, "Ln: y = %.2f*ln(%.2f*x + %.2f) + %.2f", customLn.A, customLn.B, customLn.C, customLn.D);
        sprintf(eqText, "Natural Log Color: R=%.0f G=%.0f B=%.0f", colorLn.r, colorLn.g, colorLn.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showPoly) {
        char eqText[200];
        sprintf(eqText, "Poly: y = %.2fx^4 + %.2fx^3 + %.2fx^2 + %.2fx + %.2f",
            customPoly.a4, customPoly.a3, customPoly.a2, customPoly.a1, customPoly.a0);
        sprintf(eqText, "Polynomial Color: R=%.0f G=%.0f B=%.0f", colorPoly.r, colorPoly.g, colorPoly.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showCircle) {
        char eqText[100];
        sprintf(eqText, "Circle: (x - %.2f)^2 + (y - %.2f)^2 = %.2f^2", customCircle.h, customCircle.k, customCircle.r);
        sprintf(eqText, "Circle Color: R=%.0f G=%.0f B=%.0f", colorCircle.r, colorCircle.g, colorCircle.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showEllipse) {
        char eqText[100];
        sprintf(eqText, "Ellipses: ((x - %.2f)^2)/%.2f^2 + ((y - %.2f)^2)/%.2f^2 = 1", customEllipse.h, customEllipse.a, customEllipse.k, customEllipse.b);
        sprintf(eqText, "Ellipse Color: R=%.0f G=%.0f B=%.0f", colorEllipse.r, colorEllipse.g, colorEllipse.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showParabola) {
        char eqText[100];
        sprintf(eqText, "Parabola: y = %.2fx^2 + %.2fx + %.2f", customParabola.a, customParabola.b, customParabola.c);
        sprintf(eqText, "Parabola Color: R=%.0f G=%.0f B=%.0f", colorParabola.r, colorParabola.g, colorParabola.b);
        iText(10, yPos, eqText);
        yPos -= 20;
    }
    if (showHyperbola) {
        char eqText[100];
        sprintf(eqText, "Hyperbola: ((x - %.2f)^2)/%.2f^2 - ((y - %.2f)^2)/%.2f^2 = 1", customHyperbola.h, customHyperbola.a, customHyperbola.k, customHyperbola.b);
        sprintf(eqText, "Hyperbola Color: R=%.0f G=%.0f B=%.0f", colorHyperbola.r, colorHyperbola.g, colorHyperbola.b);
        iText(10, yPos, eqText);
    }
}

void testFunctionParsing() {
    float A, B, C, D;
    float a4, a3, a2, a1, a0;

    // Test trig functions
    printf("\nTesting Trig Functions:\n");
    readTrigFunction("y = sin(x)", &A, &B, &C, &D, "sin");
    printf("sin(x): A=%.1f, B=%.1f, C=%.1f, D=%.1f\n", A, B, C, D);
    
    readTrigFunction("y = sin(x) - 5", &A, &B, &C, &D, "sin");
    printf("sin(x) - 5: A=%.1f, B=%.1f, C=%.1f, D=%.1f\n", A, B, C, D);

    // Test exponential
    printf("\nTesting Exponential:\n");
    readExponentialFunction("y = exp(x)", &A, &B, &C, &D);
    printf("exp(x): A=%.1f, B=%.1f, C=%.1f, D=%.1f\n", A, B, C, D);
    
    readExponentialFunction("y = exp(x) - 2", &A, &B, &C, &D);
    printf("exp(x) - 2: A=%.1f, B=%.1f, C=%.1f, D=%.1f\n", A, B, C, D);

    // Test logarithmic
    printf("\nTesting Logarithmic:\n");
    readLogFunction("y = ln(x)", &A, &B, &C, &D, "ln");
    printf("ln(x): A=%.1f, B=%.1f, C=%.1f, D=%.1f\n", A, B, C, D);
    
    readLogFunction("y = ln(x) - 3", &A, &B, &C, &D, "ln");
    printf("ln(x) - 3: A=%.1f, B=%.1f, C=%.1f, D=%.1f\n", A, B, C, D);

    // Test polynomial
    printf("\nTesting Polynomial:\n");
    readPolynomial("y = x^2", &a4, &a3, &a2, &a1, &a0);
    printf("x^2: a4=%.1f, a3=%.1f, a2=%.1f, a1=%.1f, a0=%.1f\n", 
           a4, a3, a2, a1, a0);
    
    readPolynomial("y = x^2 - 2x + 1", &a4, &a3, &a2, &a1, &a0);
    printf("x^2 - 2x + 1: a4=%.1f, a3=%.1f, a2=%.1f, a1=%.1f, a0=%.1f\n", 
           a4, a3, a2, a1, a0);
}

void iKeyboard(unsigned char key) {
    // Add at the beginning of the iKeyboard function
    bool isWaitingForInput = isSettingColor || isEnteringEquation || showPresetMenu;

    // Handle preset menu first
    if (showPresetMenu) {
        if (key >= '0' && key <= '4') {
            applyColorPreset(key - '0');
            showPresetMenu = false;
        } else if (key == 27) { // ESC
            showPresetMenu = false;
        }
        return;
    }

    // Handle color setting input
    if (isSettingColor) {
        if (key == '\r') { // Enter key
            if (inputStep == 0) {
                strcpy(selectedFunction, inputBuffer);
                inputStep++;
                inputBuffer[0] = '\0';
            }
            else {
                double value = atof(inputBuffer);
                if (value < 0) value = 0;
                if (value > 255) value = 255;

                Color* targetColor = NULL;
                if (strcmp(selectedFunction, "sin") == 0) targetColor = &colorSin;
                else if (strcmp(selectedFunction, "cos") == 0) targetColor = &colorCos;
                else if (strcmp(selectedFunction, "tan") == 0) targetColor = &colorTan;
                else if (strcmp(selectedFunction, "asin") == 0) targetColor = &colorASin;
                else if (strcmp(selectedFunction, "acos") == 0) targetColor = &colorACos;
                else if (strcmp(selectedFunction, "atan") == 0) targetColor = &colorATan;
                else if (strcmp(selectedFunction, "exp") == 0) targetColor = &colorExp;
                else if (strcmp(selectedFunction, "log") == 0) targetColor = &colorLog;
                else if (strcmp(selectedFunction, "ln") == 0) targetColor = &colorLn;
                else if (strcmp(selectedFunction, "poly") == 0) targetColor = &colorPoly;
                else if (strcmp(selectedFunction, "circle") == 0) targetColor = &colorCircle;
                else if (strcmp(selectedFunction, "ellipse") == 0) targetColor = &colorEllipse;
                else if (strcmp(selectedFunction, "parabola") == 0) targetColor = &colorParabola;
                else if (strcmp(selectedFunction, "hyperbola") == 0) targetColor = &colorHyperbola;

                if (targetColor != NULL) {
                    if (inputStep == 1) targetColor->r = value;
                    else if (inputStep == 2) targetColor->g = value;
                    else if (inputStep == 3) targetColor->b = value;
                }

                inputBuffer[0] = '\0';
                if (inputStep == 3) {
                    isSettingColor = false;
                    inputStep = 0;
                    strcpy(selectedFunction, "");
                } else {
                    inputStep++;
                }
            }
        }
        else if (key == 27) { // ESC key
            isSettingColor = false;
            inputStep = 0;
            inputBuffer[0] = '\0';
            strcpy(selectedFunction, "");
        }
        else if (isprint(key) && strlen(inputBuffer) < 99) {
            int len = strlen(inputBuffer);
            inputBuffer[len] = key;
            inputBuffer[len + 1] = '\0';
        }
        return;  // Early return to prevent other handlers
    }

    // Handle equation input
    if (isEnteringEquation) {
        if (strcmp(currentFunction, "inverse_trig_menu") == 0) {
            switch (key) {
                case '1':
                    strcpy(currentFunction, "arcsin");
                    showASin = false;
                    isEnteringEquation = true;
                    break;
                case '2':
                    strcpy(currentFunction, "arccos");
                    showACos = false;
                    isEnteringEquation = true;
                    break;
                case '3':
                    strcpy(currentFunction, "arctan");
                    showATan = false;
                    isEnteringEquation = true;
                    break;
                case 27: // ESC key
                    isEnteringEquation = false;
                    break;
            }
            return;
        }

        if (key == '\r') { // Enter key
            bool success = false;
            char funcType[20] = "";

            if (strcmp(currentFunction, "sin") == 0) {
                success = readTrigFunction(equationInput, &customSin.A, &customSin.B, 
                                        &customSin.C, &customSin.D, "sin");
                if (success) showSin = true;
            }
            else if (strcmp(currentFunction, "cos") == 0) {
                success = readTrigFunction(equationInput, &customCos.A, &customCos.B, 
                                        &customCos.C, &customCos.D, "cos");
                if (success) showCos = true;
            }
            else if (strcmp(currentFunction, "tan") == 0) {
                success = readTrigFunction(equationInput, &customTan.A, &customTan.B, 
                                        &customTan.C, &customTan.D, "tan");
                if (success) showTan = true;
            }
            else if (strcmp(currentFunction, "arcsin") == 0) {
                success = readInverseTrigFunction(equationInput, &customASin.A, &customASin.B, 
                                                &customASin.C, &customASin.D, funcType);
                if (success) showASin = true;
            }
            else if (strcmp(currentFunction, "arccos") == 0) {
                success = readInverseTrigFunction(equationInput, &customACos.A, &customACos.B, 
                                                &customACos.C, &customACos.D, funcType);
                if (success) showACos = true;
            }
            else if (strcmp(currentFunction, "arctan") == 0) {
                success = readInverseTrigFunction(equationInput, &customATan.A, &customATan.B, 
                                                &customATan.C, &customATan.D, funcType);
                if (success) showATan = true;
            }
            else if (strcmp(currentFunction, "exp") == 0) {
                success = readExponentialFunction(equationInput, &customExp.A, &customExp.B, &customExp.C, &customExp.D);
                if (success) showExp = true;
            }
            else if (strcmp(currentFunction, "log") == 0) {
                success = readLogFunction(equationInput, &customLog.A, &customLog.B, &customLog.C, &customLog.D, "log");
                if (success) showLog = true;
            }
            else if (strcmp(currentFunction, "ln") == 0) {
                success = readLogFunction(equationInput, &customLn.A, &customLn.B, &customLn.C, &customLn.D, "ln");
                if (success) showLn = true;
            }
            else if (strcmp(currentFunction, "poly") == 0) {
                success = readPolynomial(equationInput, &customPoly.a4, &customPoly.a3, &customPoly.a2, &customPoly.a1, &customPoly.a0);
                if (success) showPoly = true;
            }
            else if (strcmp(currentFunction, "circle") == 0) {
                success = readCircle(equationInput, &customCircle.h, &customCircle.k, &customCircle.r);
                if (success) showCircle = true;
            }
            else if (strcmp(currentFunction, "ellipse") == 0) {
                success = readEllipse(equationInput, &customEllipse.h, &customEllipse.k, &customEllipse.a, &customEllipse.b);
                if (success) showEllipse = true;
            }
            else if (strcmp(currentFunction, "parabola") == 0) {
                success = readParabola(equationInput, &customParabola.a, &customParabola.b, &customParabola.c);
                if (success) showParabola = true;
            }
            else if (strcmp(currentFunction, "hyperbola") == 0) {
                success = readHyperbola(equationInput, &customHyperbola.h, &customHyperbola.k, &customHyperbola.a, &customHyperbola.b);
                if (success) showHyperbola = true;
            }

            if (success) {
                isEnteringEquation = false;
                equationInput[0] = '\0';
            }
            else {
                strcpy(equationInput, "Invalid equation! Press ESC and try again.");
            }
        }
        else if (key == 27) { // ESC key
            isEnteringEquation = false;
            equationInput[0] = '\0';
        }
        else if (key == '\b') { // Backspace
            int len = strlen(equationInput);
            if (len > 0) {
                equationInput[len - 1] = '\0';
            }
        }
        else if (isprint(key) && strlen(equationInput) < MAX_INPUT_LENGTH - 1) {
            int len = strlen(equationInput);
            equationInput[len] = key;
            equationInput[len + 1] = '\0';
        }
        return;  // Early return to prevent other handlers
    }

    // Only handle general commands if not waiting for any input
    if (!isWaitingForInput) {
        switch (key) {
            case '1': 
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '-':
            case '=':
                // ...existing equation selection code...
                if (key == '1') {
                    strcpy(currentFunction, "sin"); 
                    showSin = false; 
                    isEnteringEquation = true;
                } else if (key == '2') {
                    strcpy(currentFunction, "cos"); 
                    showCos = false; 
                    isEnteringEquation = true;
                } else if (key == '3') {
                    strcpy(currentFunction, "tan"); 
                    showTan = false; 
                    isEnteringEquation = true;
                } else if (key == '4') {
                    strcpy(currentFunction, "poly"); 
                    showPoly = false; 
                    isEnteringEquation = true;
                } else if (key == '5') {
                    strcpy(currentFunction, "circle"); 
                    showCircle = false; 
                    isEnteringEquation = true;
                } else if (key == '6') {
                    strcpy(currentFunction, "ellipse"); 
                    showEllipse = false; 
                    isEnteringEquation = true;
                } else if (key == '7') {
                    strcpy(currentFunction, "parabola"); 
                    showParabola = false; 
                    isEnteringEquation = true;
                } else if (key == '8') {
                    strcpy(currentFunction, "hyperbola"); 
                    showHyperbola = false; 
                    isEnteringEquation = true;
                } else if (key == '9') {
                    strcpy(currentFunction, "exp"); 
                    showExp = false; 
                    isEnteringEquation = true;
                } else if (key == '0') {
                    strcpy(currentFunction, "log"); 
                    showLog = false; 
                    isEnteringEquation = true;
                } else if (key == '-') {
                    strcpy(currentFunction, "ln"); 
                    showLn = false; 
                    isEnteringEquation = true;
                } else if (key == '=') {
                    // Show inverse trig menu
                    strcpy(currentFunction, "inverse_trig_menu");
                    isEnteringEquation = true;  // Set this to true so the menu can be processed
                    equationInput[0] = '\0';    // Clear any existing input
                }
                break;
            case 'c': 
                isSettingColor = true;
                inputStep = 0;
                inputBuffer[0] = '\0';
                selectedFunction[0] = '\0';
                break;
            case 'p':
                showPresetMenu = true;
                break;
            case 'g':
                showGridFlag = !showGridFlag;
                break;
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
            case 'q':
                exit(0);
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
