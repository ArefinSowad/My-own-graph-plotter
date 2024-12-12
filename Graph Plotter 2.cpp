#include "iGraphics.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

#define screenWidth 1200
#define screenHeight 600
#define MAX_TEXTBOXES 100
#define MAX_TEXT_LENGTH 100
/*
	function iDraw() is called again and again by the system.

	*/
char token;
int r = 0;
int g = 0;
int b = 0;
int r1 = 255;
int g1 = 255;
int b1 = 255;
int mouseX = 0;
int mouseY = 0;
int windowX = 0;
int windowY = 0;
int windowWidth = 350;
int windowHeight = 600;
int angleX = 5;
int angleY = 565;
int angleX1 = 320;
int angleWidth = 32;
int angleHeight = 32;
int emptyText[MAX_TEXTBOXES] = {0};
int drawState[MAX_TEXTBOXES] = {0};
bool showMyself = false;
bool showAbout = false;
bool homepage = true;
bool lightMode = false;
bool showCurve[MAX_TEXTBOXES];
bool showFuncList = false;
bool toggleTextCursor = false;
bool onClickFunction = false;
bool showFloatingWindow = false;
bool rightAngle = true;
bool leftAngle = false;
bool showKeyboard = false;
bool typeState = false;
bool highlightedTextBox = false;
int indexNumber[MAX_TEXTBOXES + 5] = {6};
int textBoxWidth = 350;
int textBoxHeight = 70;
int textBoxCount = 1;
int functionX = 853;
int functionY = 125;
int buttonWidth = 68;
int buttonHeight = 35;
int buttonX = 214;
int buttonY = 5;
int numberpadX = 533;
int numberpadY = 5;
int functionWidth = 137;
int functionHeight = 35;
int floatingFunctionX = 1000;
int floatingFunctionY = 70;
int floatingFunctionWidth = 195;
int floatingFunctionHeight = 160;
int keyboardIconWidth = 98;
int keyboardIconHeight = 60;
int keyboardWidth = 1200;
int keyboardHeight = 165;
int keyboardX = 7;
int keyboardY = 7;
int startIndex_X = 111;
int selectedTextBox = 0;
int counter = 0;
double graphColor[MAX_TEXTBOXES][3];
int colorFlag[MAX_TEXTBOXES];
int funcType[MAX_TEXTBOXES];
int funcSpec[MAX_TEXTBOXES][4][7];
int cursorIndex2[MAX_TEXTBOXES];
int cursorIndex[MAX_TEXTBOXES];
char func[MAX_TEXTBOXES][MAX_TEXT_LENGTH];
char func1[MAX_TEXTBOXES][MAX_TEXT_LENGTH];
int textBoxPositions[MAX_TEXTBOXES];

double offsetX = 0.0;
double offsetY = 0.0;
double offsetX1[MAX_TEXTBOXES] = {0.0};
double offsetY1 = 0.0;

void designHome()
{
	iShowBMP2(0, 0, "homePage.bmp", 0.0);
	iShowBMP2(175, 350 + 40, "Title.bmp", 0.0);
	iShowBMP2(875, 150 - 40, "button.bmp", 0.0);
	iShowBMP2(875, 50 - 40, "button.bmp", 0.0);
	iShowBMP2(875, 250 - 40, "button.bmp", 0.0);
	iShowBMP2(890 + 5, 205 + 15, "PlotGraph.bmp", 0.0);
	iShowBMP2(890 + 5 + 30, 105 + 10, "About.bmp", 0.0);
	iShowBMP2(890 + 5, 5 + 15 + 10, "Instruct.bmp", 0.0);
	iShowBMP2(1000, 400, "BUET.bmp", 0.0);
}

void myself()
{
	if (homepage == true && showMyself == true)
	{
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1200, 600);
		iShowBMP2(80, 0, "Myself.bmp", 0.0);
		iShowBMP2(1150, 550, "circleWhite.bmp", 0.0);
	}
}

void drawIcons()
{
	iSetColor(r, g, b);
	iFilledRectangle(0, 0, 1200, 600);

	iSetColor(255, 87, 51);
	iFilledCircle(1167, 569 - 2, 20);
	iShowBMP2(1150, 550, "moon.bmp", 0.0);

	iSetColor(255, 87, 51);
	iFilledCircle(1167, 537 - 10 - 2, 20);
	iShowBMP2(1150, 510, "book.bmp", 0.0);

	iSetColor(255, 87, 51);
	iFilledCircle(1166, 505 - 20 - 2, 16);
	iShowBMP2(1150, 480 - 13.7, "homeButton.bmp", 0.0);
}
void instructions()
{
	if (showAbout == true)
	{

		iShowBMP2(200, 100, "InstructionsFull.bmp", 0.0);
		iShowBMP2(390, 480, "Instructions.bmp", 0.0);
		iShowBMP2(970, 520, "circleWhite.bmp", 0.0);
	}
}
void instructions2()
{
	if (showAbout == true)
	{
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1200, 600);
		iShowBMP2(200, 100, "InstructionsFull.bmp", 0.0);
		iShowBMP2(390, 480, "Instructions.bmp", 0.0);
		iShowBMP2(970, 520, "circleWhite.bmp", 0.0);
	}
}
void initializeArray()
{
	for (int i = 1; i < MAX_TEXTBOXES; i++)
	{
		funcType[i] = 0;
	}
}
void initializeCursorIndex()
{
	for (int i = 0; i < MAX_TEXTBOXES + 5; i++)
	{
		cursorIndex[i] = 105 + indexNumber[selectedTextBox];
		cursorIndex2[i] = 0;
	}
}
int countDigits(int num)
{
	int count = 0;
	// Handle the case where num is 0 separately
	if (num == 0)
	{
		return 1;
	}
	// For negative numbers, make them positive temporarily
	if (num < 0)
	{
		num = -num;
	}
	while (num > 0)
	{
		num /= 10;
		count++;
	}
	return count;
}
void initializeIndex()
{
	for (int i = 1; i < MAX_TEXTBOXES + 5; i++)
	{
		indexNumber[i] = 6;
	}
}
void drawSquares(double lineWidth)
{
	// Set line color to darker gray
	iSetColor(80, 80, 80);

	// Draw horizontal lines for centimeter squares
	for (int i = -100000; i <= 100000; i += 20)
	{
		iLine(0, i - offsetY, 100000, i - offsetY, lineWidth);
	}

	// Draw vertical lines for centimeter squares
	for (int i = -100000; i <= 100000; i += 20)
	{
		iLine(i - offsetX, 0, i - offsetX, 100000, lineWidth);
	}
}
void darkLine(double lineWidth)
{
	for (int i = -100000; i <= 100000; i += 100)
	{
		iSetColor(180, 180, 180); // Darker color
		iLine(0, i - offsetY, iScreenWidth, i - offsetY, lineWidth);
	}
	for (int i = -100000; i <= 100000; i += 100)
	{
		iSetColor(180, 180, 180); // Darker color
		iLine(i - offsetX, 0, i - offsetX, iScreenHeight, lineWidth);
	}
}
void drawLabel()
{
	for (int i = screenWidth / 2, j = 0; i <= 100000; i += 100, j += 1)
	{
		char label[5];
		if (j != 0)
		{
			int textX, textY, textWidth, textHeight;

			if (offsetX > -496 && offsetY < -303)
			{
				// Adjust position when conditions are met
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = i - 5 - offsetX;
				textHeight = 13;
				textY = screenHeight - textHeight;
			}
			else if (offsetX > -494 && offsetY > 280)
			{
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = i - 5 - offsetX;
				textHeight = 13;
				textY = 0;
			}
			else
			{
				textX = i - 5 - offsetX;
				textY = screenHeight / 2 - 17 - offsetY; // Fixing the text at the top of the screen
				textWidth = 17;
				textHeight = 13;
			}

			iSetColor(r, g, b);
			iFilledRectangle(textX, textY, textWidth, textHeight);

			iSetColor(r1, g1, b1);
			sprintf(label, "%d", j);
			iText(textX, textY, label, GLUT_BITMAP_HELVETICA_12);
		}
	}

	for (int i = screenWidth / 2, j = 0; i >= -100000; i -= 100, j -= 1)
	{
		char label[5];
		if (j != 0)
		{
			int textX, textY, textWidth, textHeight;

			if (offsetX < 574 && offsetY < -303)
			{
				// Adjust position when conditions are met
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = i - 12 - offsetX;
				textHeight = 12;
				textY = screenHeight - textHeight;
			}
			else if (offsetX < 574 && offsetY > 280)
			{
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = i - 12 - offsetX;
				textHeight = 12;
				textY = 0;
			}
			else
			{
				textX = i - 12 - offsetX;
				textY = screenHeight / 2 - 15 - offsetY;
				textWidth = 17;
				textHeight = 13;
			}

			iSetColor(r, g, b);
			iFilledRectangle(textX, textY - 2, textWidth, textHeight);

			iSetColor(r1, g1, b1);
			sprintf(label, "%d", j);
			iText(textX, textY, label, GLUT_BITMAP_HELVETICA_12);
		}
		else
		{
			iSetColor(r1, g1, b1);
			sprintf(label, "%d", j);
			iText(i - 12 - offsetX, screenHeight / 2 - 15 - offsetY, label, GLUT_BITMAP_HELVETICA_12);
		}
	}

	for (int i = screenHeight / 2, j = 0; i <= 100000; i += 100, j += 1)
	{
		char label[5];
		int textX, textY, textWidth, textHeight;

		if (j != 0)
		{
			if (offsetX < -573 && offsetY > -280)
			{
				// Adjust position when conditions are met
				textY = i - 5 - offsetY;
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = screenWidth - textWidth;
				textHeight = 12;
			}
			else if (offsetX > 600 && offsetY > -280)
			{
				textY = i - 5 - offsetY;
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = 0;
				textHeight = 12;
			}
			else
			{
				textX = screenWidth / 2 - 23 + 25 - offsetX;
				textY = i - 5 - offsetY;
				textWidth = 15 + 5 * (countDigits(j) - 1);
				textHeight = 12;
			}

			iSetColor(r, g, b);
			iFilledRectangle(textX, textY - 2, textWidth - 4, textHeight);
		}
		iSetColor(r1, g1, b1);
		sprintf(label, "%d", j);
		iText(textX, textY, label, GLUT_BITMAP_HELVETICA_12);
	}

	for (int i = screenHeight / 2, j = 0; i >= -100000; i -= 100, j -= 1)
	{
		char label[5];
		if (j != 0)
		{
			int textX, textY, textWidth, textHeight;

			if (offsetX < -573 && offsetY < 280)
			{
				// Adjust position when conditions are met

				textY = i - 5 - offsetY;
				textWidth = 20 + 5 * (countDigits(j) - 1);
				textX = screenWidth - textWidth;
				textHeight = 12;
			}
			else if (offsetX > 600 && offsetY < 280)
			{
				textY = i - 5 - offsetY;
				textWidth = 15 + 5 * (countDigits(j) - 1);
				textX = 0;
				textHeight = 12;
			}
			else
			{
				textX = screenWidth / 2 - 27 - offsetX + 28;
				textY = i - 5 - offsetY;
				textWidth = 14 + 7 * (countDigits(j) - 1);
				textHeight = 16;
			}

			iSetColor(r, g, b);
			iFilledRectangle(textX, textY, textWidth + 5, textHeight - 5);

			iSetColor(r1, g1, b1);
			sprintf(label, "%d", j);
			iText(textX, textY, label, GLUT_BITMAP_HELVETICA_12);
		}
	}
}
void showLine()
{
	iSetColor(r1, g1, b1);
	iLine(screenWidth / 2 - offsetX, -100000 - offsetY, screenWidth / 2 - offsetX, 100000 - offsetY, 3.0);
	iLine(-100000 - offsetX, screenHeight / 2 - offsetY, 100000 - offsetX, screenHeight / 2 - offsetY, 3.0);
}
void angle()
{

	if (lightMode == false)
	{
		if (rightAngle)
		{
			iShowBMP2(angleX, angleY, "angleWhiteright.bmp", 0);
		}
		if (leftAngle)
		{
			iShowBMP2(angleX1, angleY, "angleWhiteleft.bmp", 0);
		}
	}
	else
	{
		if (rightAngle)
		{
			iSetColor(0, 0, 0);
			iText(angleX, angleY, ">>", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (leftAngle)
		{
			iSetColor(0, 0, 0);
			iText(angleX1, angleY, "<<", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
}
void block()
{
	if (showFloatingWindow)
	{
		iSetColor(r, g, b);
		iFilledRectangle(windowX, 561, windowWidth, windowHeight);
	}
}
void add()
{
	if (lightMode == false)
	{
		if (showFloatingWindow)
		{
			iShowBMP2(angleX, angleY, "plusWhite.bmp", 0);
		}
	}
	else
	{
		if (showFloatingWindow)
		{
			iSetColor(0, 0, 0);
			iText(angleX, angleY, "+", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
}

void floatingFunction2()
{
	if (showFuncList == true && showFloatingWindow == true && selectedTextBox != 0)
	{
		iSetColor(100, 100, 100);
		iFilledRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 177 + 6, 150, 180);
		iSetColor(r1, g1, b1);
		iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 177 + 6, 150, 180);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 15 + 6 - 3, "Linear Function", GLUT_BITMAP_HELVETICA_18);
		iLine(197 + 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 20 + 6 - 3, 347 - 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 20 + 6 - 3);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 40 + 6 - 3, "Trig Function", GLUT_BITMAP_HELVETICA_18);
		iLine(197 + 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 45 + 6 - 3, 347 - 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 45 + 6 - 3);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 67 + 6, "Inv Trig Func", GLUT_BITMAP_HELVETICA_18);
		iLine(197 + 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 75 + 6, 347 - 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 75 + 6);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 95 + 6, "Exponential ", GLUT_BITMAP_HELVETICA_18);
		iLine(197 + 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 100 + 6, 347 - 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 100 + 6);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 120 + 6, "Logarithmic ", GLUT_BITMAP_HELVETICA_18);
		iLine(197 + 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 125 + 6, 347 - 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 125 + 6);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 145 + 6, "Natural Log", GLUT_BITMAP_HELVETICA_18);
		iLine(197 + 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 6, 347 - 3, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 6);
		iText(205, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 165 + 6 - 3, "Polynomial", GLUT_BITMAP_HELVETICA_18);
		// iShowBMP2(320, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 165 + 6-3-7,"dropDown.bmp",0.0);
	}
}
void selectFunction2()
{

	if (showFuncList == true && showFloatingWindow == true && selectedTextBox != 0)
	{
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 15 + 6 - 3 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 15 + 25 + 6)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 20 + 5 - 1.5, 150, 25);
			funcType[selectedTextBox] = 1;
			showFuncList = false;
		}
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 40 + 6 - 3 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 40 + 25 + 6 - 3)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 45 + 5 - 1.5, 150, 25);
			funcType[selectedTextBox] = 2;
			for (int i = 0; i < 7; i++)
			{
				funcSpec[selectedTextBox][1][i] = 0;
			}
			showFuncList = false;
		}
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 67 + 6 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 67 + 25 + 6 - 3)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 75 + 5 + 0.4, 150, 25);
			funcType[selectedTextBox] = 3;
			for (int i = 0; i < 7; i++)
			{
				funcSpec[selectedTextBox][2][i] = 0;
			}
			showFuncList = false;
		}
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 95 + 6 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 95 + 25 + 6)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 100 + 5 + 0.4, 150, 25);
			funcType[selectedTextBox] = 4;
			showFuncList = false;
		}
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 120 + 6 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 120 + 25 + 6)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 125 + 5 + 0.4, 150, 25);
			funcType[selectedTextBox] = 5;
			showFuncList = false;
		}
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 145 + 6 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 145 + 25 + 6)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4, 150, 25);
			funcType[selectedTextBox] = 6;
			showFuncList = false;
		}
		if (mouseX >= 197 && mouseX <= 347 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 165 + 6 - 3 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 165 + 25 + 6 - 3)
		{
			iSetColor(255, 165, 0);
			iRectangle(197, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 165 + 5 + 0.4 - 10, 150, 25);
			funcType[selectedTextBox] = 7;
			iSetColor(255, 165, 0);
			iFilledCircle(305 - 1.5, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 12, 8);
			iSetColor(0, 0, 0);
			iText(300 - 1.5, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 17, "2", GLUT_BITMAP_HELVETICA_18);
			iSetColor(255, 165, 0);
			iFilledCircle(323 - 1.5, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 12, 8);
			iSetColor(0, 0, 0);
			iText(318 - 1.5, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 17, "3", GLUT_BITMAP_HELVETICA_18);
			iSetColor(255, 165, 0);
			iFilledCircle(341 - 1.5, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 12, 8);
			iSetColor(0, 0, 0);
			iText(336 - 1.5, windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 17, "4", GLUT_BITMAP_HELVETICA_18);
			if (mouseX >= 297 && mouseX <= 313 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 20 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 4)
			{
				funcSpec[selectedTextBox][3][1] = 2;
				showFuncList = false;
			}
			if (mouseX >= 315 && mouseX <= 331 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 20 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 4)
			{
				funcSpec[selectedTextBox][3][1] = 3;
				showFuncList = false;
			}
			if (mouseX >= 333 && mouseX <= 349 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 20 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] - 150 + 5 + 0.4 - 4)
			{
				funcSpec[selectedTextBox][3][1] = 4;
				showFuncList = false;
			}
		}
	}
}
void graphColoring()
{
	for (int i = 1; i < textBoxCount; i++)
	{
		if (colorFlag[i] == 0 && i % 5 == 4)
		{
			graphColor[i][0] = rand() % 20 + 235;
			graphColor[i][1] = rand() % 225 + 30;
			graphColor[i][2] = rand() % 128 + 45;
			colorFlag[i] = 1;
		}
		else if (colorFlag[i] == 0 && i % 5 == 3)
		{
			graphColor[i][0] = rand() % 150 + 45;
			graphColor[i][1] = rand() % 128 + 45;
			graphColor[i][2] = rand() % 20 + 200;
			colorFlag[i] = 1;
		}
		else if (colorFlag[i] == 0 && i % 5 == 2)
		{
			graphColor[i][0] = rand() % 175 + 30;
			graphColor[i][1] = rand() % 20 + 235;
			graphColor[i][2] = rand() % 128 + 45;
			colorFlag[i] = 1;
		}
		else if (colorFlag[i] == 0 && i % 5 == 1)
		{
			graphColor[i][0] = rand() % 10 + 245;
			graphColor[i][1] = rand() % 10;
			graphColor[i][2] = rand() % 256;
			colorFlag[i] = 1;
		}
		else if (colorFlag[i] == 0 && i % 5 == 0)
		{
			graphColor[i][0] = rand() % 256;
			graphColor[i][1] = rand() % 256;
			graphColor[i][2] = rand() % 10 + 235;
			colorFlag[i] = 1;
		}
	}
}
void textBox()
{

	for (int i = 1; i < textBoxCount; ++i)
	{

		int stringWidth = strlen(func[i]) * 12; // Adjust the multiplier as needed
		int visibleWidth = windowWidth - 55;	// Adjust the visible width as needed

		if (cursorIndex[i] >= windowWidth - 54 && selectedTextBox == i && typeState == true)
		{
			int visibleStringLength = 22; // Adjust the number of visible characters as needed
			int startIndex = strlen(func[i]) - visibleStringLength;

			// Draw the last 20 characters of the string with adjusted x-coordinate
			iSetColor(r1, g1, b1);
			iText(55, windowHeight - 10 - textBoxPositions[i] - offsetY1, func[i] + startIndex, GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else
		{
			// Draw the entire string with adjusted x-coordinate
			iSetColor(r1, g1, b1);
			// func1[i] will copy 22 characters from the func[i] string
			char temp[] = "\0";
			strcpy(temp, func1[i]);
			strncpy(func1[i], func[i], 22);
			iText(55, windowHeight - 10 - textBoxPositions[i] - offsetY1, func1[i], GLUT_BITMAP_TIMES_ROMAN_24);
		}
		iSetColor(100, 100, 100);
		iRectangle(1, windowHeight - 40 - textBoxPositions[i] - offsetY1, textBoxWidth, textBoxHeight);
		iSetColor(45, 45, 45);
		iFilledRectangle(1, windowHeight - 39.511 - textBoxPositions[i] - offsetY1, textBoxWidth - 300, textBoxHeight);
		char temp1[5];
		sprintf(temp1, "%d", i); // Convert the integer to a string
		iSetColor(145, 145, 145);

		iShowBMP2(320, windowHeight - textBoxPositions[i] - offsetY1 + 6, "angleDownWhite.bmp", 0.0);
		graphColoring();
		iShowBMP2(320, windowHeight - textBoxPositions[i] - offsetY1 - 18, "circleWhite.bmp", 0.0);

		if (drawState[i] == 2)
		{
			if (i < 10)
			{
				if (showCurve[i] == 0)
				{
					iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]);
					iFilledCircle(25, windowHeight - textBoxPositions[i] - offsetY1 - 5, 14);
				}
				iSetColor(r, g, b);
				iFilledCircle(25, windowHeight - textBoxPositions[i] - offsetY1 - 5, 9);
				// iSetColor(145, 145, 145);
			}
			else if (i >= 10)
			{
				if (showCurve[i] == 1)
				{
					iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]);

					iFilledCircle(27, windowHeight - textBoxPositions[i] - offsetY1 - 5, 16);
				}
				iSetColor(r, g, b);
				iFilledCircle(27, windowHeight - textBoxPositions[i] - offsetY1 - 5, 11);
				//	iSetColor(145, 145, 145);
			}
		}

		if (i < 10 || drawState[i] == 0 || drawState[i] == 2)
		{
			iSetColor(145, 145, 145);
			iText(20, windowHeight - textBoxPositions[i] - offsetY1 - 10.3, temp1, GLUT_BITMAP_HELVETICA_18, 15.0);
		}
		else
		{
			iSetColor(145, 145, 145);
			iText(17, windowHeight - textBoxPositions[i] - offsetY1 - 10.3, temp1, GLUT_BITMAP_HELVETICA_18, 15.0);
		}
		if (drawState[i] == 3)
		{
			iSetColor(45, 45, 45);
			iFilledRectangle(1, windowHeight - 39 - textBoxPositions[i] - offsetY1, textBoxWidth - 300, textBoxHeight - 5);
			iShowBMP2(14, windowHeight - textBoxPositions[i] - offsetY1 - 17, "triangle.bmp", 0);
		}

		// 	iSetColor(r1, g1, b1);
		// 	iText(55 - offsetX1[i], windowHeight - 10 - textBoxPositions[i] - offsetY1, func[i], GLUT_BITMAP_TIMES_ROMAN_24);
	}
}
void drawFloatingWindow()
{
	if (showFloatingWindow)
	{
		rightAngle = false;
		leftAngle = true;
		iSetColor(r, g, b);
		iFilledRectangle(windowX, windowY, windowWidth, windowHeight);
		textBox();
	}
}
void keyboard()
{
	if (showFloatingWindow == true)
	{
		if (showKeyboard == false)
		{
			iShowBMP2(keyboardX, keyboardY, "keyboardBlack.bmp", 0);
		}
		else
		{
			iShowBMP2(0, 0 + keyboardHeight, "keyBoardBlackDown.bmp", 0);
			iShowBMP(0, 0, "keyboardBlackFull.bmp");
		}
	}
}
void highlightTextBox()
{
	if (showFloatingWindow == true)
	{
		if (mouseX >= 0 && mouseX <= textBoxWidth && mouseY > 0 && mouseY < 560 && showKeyboard == false)
		{
			for (int i = 1; i <= textBoxCount; i++)
			{
				// Check if the mouse is over the current textbox
				if (mouseX >= 0 && mouseX <= textBoxWidth && mouseY >= windowHeight - 40 - offsetY1 - textBoxPositions[i] && mouseY <= windowHeight - textBoxPositions[i] - offsetY1 + 25 && showFuncList == false)
				{

					typeState = true;
					selectedTextBox = i;
					highlightedTextBox = true;
					// Draw a rectangle around the specific textbox
					iSetColor(255, 165, 0);
					iRectangle(1, windowHeight - 40 - textBoxPositions[selectedTextBox] - offsetY1, textBoxWidth - 2, textBoxHeight);
				}
			}
		}
		else if (mouseX >= 0 && mouseX <= textBoxWidth && !(mouseY >= 0 && mouseY <= keyboardHeight + keyboardIconHeight + 10) && showKeyboard == true)
		{
			for (int i = 1; i <= textBoxCount; i++)
			{
				// Check if the mouse is over the current textbox
				if (mouseX >= 0 && mouseX <= textBoxWidth && mouseY >= windowHeight - 40 - offsetY1 - textBoxPositions[i] && mouseY <= windowHeight - textBoxPositions[i] - offsetY1 + 25 && !(mouseY >= 0 && mouseY <= keyboardHeight + keyboardIconHeight))
				{

					typeState = true;
					selectedTextBox = i;
					highlightedTextBox = true;
					// Draw a rectangle around the specific textbox
					iSetColor(255, 165, 0);
					iRectangle(1, windowHeight - 40 - textBoxPositions[selectedTextBox] - offsetY1, textBoxWidth - 2, textBoxHeight);
				}
			}
		}
	}
}
void toggleFunction()
{
	if (onClickFunction == true)
	{
		iSetColor(255, 165, 0);
		iRectangle(functionX, functionY, functionWidth, functionHeight);
	}
}
void floatingFunction()
{
	if (showFloatingWindow == true && onClickFunction == true && showKeyboard == true)
	{
		iSetColor(100, 100, 100);
		double xa[] = {992, 1000, 992};
		double ya[] = {148, 143, 137};
		iFilledPolygon(xa, ya, 3);
		iFilledRectangle(1000, 5, floatingFunctionWidth, floatingFunctionHeight);
		iSetColor(r1, g1, b1);
		iPolygon(xa, ya, 3);
		iRectangle(1000, 5, floatingFunctionWidth, floatingFunctionHeight);
		iSetColor(r1, g1, b1);
		iText(1005, 170 - 3 - 20, "Linear Function", GLUT_BITMAP_HELVETICA_18);
		iLine(1000 + 3, 170 - 3 - 3 - 20, 1000 + floatingFunctionWidth - 3, 170 - 3 - 3 - 20);
		iText(1005, 150 - 5 - 20, "Trig Function", GLUT_BITMAP_HELVETICA_18);
		iLine(1000 + 3, 150 - 10 - 20, 1000 + floatingFunctionWidth - 3, 150 - 10 - 20);
		iText(1005, 130 - 7 - 20, "Inv Trig Func", GLUT_BITMAP_HELVETICA_18);
		iLine(1000 + 3, 130 - 13 - 20, 1000 + floatingFunctionWidth - 3, 130 - 13 - 20);
		iText(1005, 110 - 9 - 20, "Exponential ", GLUT_BITMAP_HELVETICA_18);
		iLine(1000 + 3, 110 - 15 - 20, 1000 + floatingFunctionWidth - 3, 110 - 15 - 20);
		iText(1005, 90 - 11 - 20, "Logarithmic ", GLUT_BITMAP_HELVETICA_18);
		iLine(1000 + 3, 90 - 17 - 20, 1000 + floatingFunctionWidth - 3, 90 - 17 - 20);
		iText(1005, 70 - 13 - 20, "Natural Log", GLUT_BITMAP_HELVETICA_18);
		iLine(1000 + 3, 70 - 19 - 20, 1000 + floatingFunctionWidth - 3, 70 - 19 - 20);
		iText(1005, 50 - 15 - 20, "Polynomial", GLUT_BITMAP_HELVETICA_18);
	}
}
void textCursor()
{
	if (showFloatingWindow == true && selectedTextBox != 0)
	{
		if (toggleTextCursor == true && typeState == true)
		{
			if (cursorIndex[selectedTextBox] <= 111 || strlen(func[selectedTextBox]) == 6)
			{
				cursorIndex[selectedTextBox] = 111;
			}

			iShowBMP2(cursorIndex[selectedTextBox], windowHeight - 20 - textBoxPositions[selectedTextBox] - offsetY1, "textCursor.bmp", 0);
		}
	}
}
void change()
{
	if (typeState == true)
	{
		if (toggleTextCursor == true)
		{
			toggleTextCursor = false;
		}
		else
		{
			toggleTextCursor = true;
		}
	}
}
void keyboardMap()
{
	if (showFloatingWindow == true && showKeyboard == true)
	{
		typeState == true;
		counter = 0;
		if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight)
		{

			funcType[selectedTextBox] = 3;
		}
		if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY && mouseY <= buttonY + buttonHeight)
		{

			const char *sqrtStr = "ln(";
			strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
			indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
			cursorIndex[selectedTextBox] += 26;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY && mouseY <= buttonY + buttonHeight)
		{
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = ')';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 8;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}

		if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY + buttonHeight + 5 && mouseY <= buttonY + buttonHeight * 2 + 5)
		{

			if (funcType[selectedTextBox] == 3)
			{
				const char *sqrtStr = "acosec(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 75;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
			else
			{
				const char *sqrtStr = "cosec(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 63;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
		}
		if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY + buttonHeight + 5 && mouseY <= buttonY + buttonHeight * 2 + 5)
		{
			if (funcType[selectedTextBox] == 3)
			{
				const char *sqrtStr = "asec(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 52;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
			else
			{
				const char *sqrtStr = "sec(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 41;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
		}
		if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY + buttonHeight + 5 && mouseY <= buttonY + buttonHeight * 2 + 5)
		{

			if (funcType[selectedTextBox] == 3)
			{
				const char *sqrtStr = "acot(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 50;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
			else
			{
				const char *sqrtStr = "cot(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 39;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
		}

		if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY + buttonHeight * 2 + 9 && mouseY <= buttonY + buttonHeight * 3 + 5)
		{

			if (funcType[selectedTextBox] == 3)
			{
				const char *sqrtStr = "asin(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 48;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
			else
			{
				const char *sqrtStr = "sin(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 38;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
		}
		if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY + buttonHeight * 2 + 9 && mouseY <= buttonY + buttonHeight * 3 + 5)
		{

			if (funcType[selectedTextBox] == 3)
			{
				const char *sqrtStr = "acos(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 52;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
			else
			{
				const char *sqrtStr = "cos(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 41;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
		}
		if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY + buttonHeight * 2 + 9 && mouseY <= buttonY + buttonHeight * 3 + 5)
		{
			if (funcType[selectedTextBox] == 3)
			{
				const char *sqrtStr = "atan(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 50;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
			else
			{
				const char *sqrtStr = "tan(";
				strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
				indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
				cursorIndex[selectedTextBox] += 39;
				func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
				strcpy(func1[selectedTextBox], func[selectedTextBox]);
			}
		}

		if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY + buttonHeight * 3 + 7 && mouseY <= buttonY + buttonHeight * 4 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = 'x';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY + buttonHeight * 3 + 10 && mouseY <= buttonY + buttonHeight * 4)
		{

			const char *sqrtStr = "pow(";
			strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
			indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
			cursorIndex[selectedTextBox] += 50;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY + buttonHeight * 3 + 10 && mouseY <= buttonY + buttonHeight * 4)
		{

			const char *sqrtStr = "exp(";
			strcpy(func[selectedTextBox] + indexNumber[selectedTextBox + 1], sqrtStr);
			indexNumber[selectedTextBox + 1] += strlen(sqrtStr);
			cursorIndex[selectedTextBox] += 45;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}

		if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '0';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '.';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 7;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = ',';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 7;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '+';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '1';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '2';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '3';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '-';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '4';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '5';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = ' \0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '6';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = ' \0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '*';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = ' \0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4 + 5)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '7';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '8';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '9';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
		if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4)
		{

			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '/';
			indexNumber[selectedTextBox + 1]++;
			cursorIndex[selectedTextBox] += 11;
			func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
			strcpy(func1[selectedTextBox], func[selectedTextBox]);
		}
	}
}
void highlightKeyboardButton()
{
	if (counter <= 7)
	{
		if (showFloatingWindow == true && showKeyboard == true)
		{

			if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY && mouseY <= buttonY + buttonHeight)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 1 + 5, buttonY, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY && mouseY <= buttonY + buttonHeight)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 2 + 5, buttonY, buttonWidth, buttonHeight);
			}

			if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY + buttonHeight + 5 && mouseY <= buttonY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX, buttonY + buttonHeight + 5, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY + buttonHeight + 5 && mouseY <= buttonY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 1 + 5, buttonY + buttonHeight + 5, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY + buttonHeight + 5 && mouseY <= buttonY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 2 + 5, buttonY + buttonHeight + 5, buttonWidth, buttonHeight);
			}

			if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY + buttonHeight * 2 + 7 && mouseY <= buttonY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX, buttonY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY + buttonHeight * 2 + 7 && mouseY <= buttonY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 1 + 5, buttonY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY + buttonHeight * 2 + 7 && mouseY <= buttonY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 2 + 5, buttonY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}

			if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY + buttonHeight * 3 + 7 && mouseY <= buttonY + buttonHeight * 4 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX, buttonY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 1 + 5 && mouseX <= buttonX + buttonWidth * 2 && mouseY >= buttonY + buttonHeight * 3 + 7 && mouseY <= buttonY + buttonHeight * 4)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 1 + 5, buttonY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}
			if (mouseX >= buttonX + buttonWidth * 2 + 5 && mouseX <= buttonX + buttonWidth * 3 && mouseY >= buttonY + buttonHeight * 3 + 7 && mouseY <= buttonY + buttonHeight * 4)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX + buttonWidth * 2 + 5, buttonY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}

			if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX, numberpadY, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 1 + 5, numberpadY, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 2 + 5, numberpadY, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY && mouseY <= numberpadY + buttonHeight)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 3 + 5, numberpadY, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX, numberpadY + buttonHeight + 5, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 1 + 5, numberpadY + buttonHeight + 5, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 2 + 5, numberpadY + buttonHeight + 5, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY + buttonHeight + 5 && mouseY <= numberpadY + buttonHeight * 2 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 3 + 8, numberpadY + buttonHeight + 5, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX, numberpadY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 1 + 5, numberpadY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 2 + 5, numberpadY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY + buttonHeight * 2 + 5 && mouseY <= numberpadY + buttonHeight * 3 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 3 + 5 + 4, numberpadY + buttonHeight * 2 + 9, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX && mouseX <= numberpadX + buttonWidth && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4 + 5)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX, numberpadY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 1 + 5 && mouseX <= numberpadX + buttonWidth * 2 && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 1 + 5, numberpadY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 2 + 5 && mouseX <= numberpadX + buttonWidth * 3 && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 2 + 5, numberpadY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}
			if (mouseX >= numberpadX + buttonWidth * 3 + 5 && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= numberpadY + buttonHeight * 3 + 5 && mouseY <= numberpadY + buttonHeight * 4)
			{

				iSetColor(255, 165, 0);
				iRectangle(numberpadX + buttonWidth * 3 + 8, numberpadY + buttonHeight * 3 + 12, buttonWidth, buttonHeight);
			}
		}
	}
	if (showFloatingWindow == true && showKeyboard == true)
	{
		if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight)
		{
			if (funcType[selectedTextBox] == 3)
			{

				iSetColor(255, 165, 0);
				iRectangle(buttonX, buttonY, buttonWidth, buttonHeight);
			}
		}
	}
}
void onKeyboardHighlight()
{
	if (showKeyboard == true && showFloatingWindow == true && selectedTextBox != 0)
	{

		if ((mouseX >= buttonX && mouseX <= numberpadX + buttonWidth * 4 && mouseY >= buttonY && mouseY <= numberpadY + buttonHeight * 4 + 5) || (mouseX >= 1 && mouseX <= 1 + textBoxWidth && mouseY >= windowHeight - 40 - textBoxPositions[selectedTextBox] - offsetY1, textBoxWidth - 2 && mouseY <= windowHeight - 40 - textBoxPositions[selectedTextBox] - offsetY1, textBoxWidth - 2 + textBoxHeight))
			typeState = true;

		highlightedTextBox = true;
		// Draw a rectangle around the specific textbox
		iSetColor(255, 165, 0);
		iRectangle(1, windowHeight - 40 - textBoxPositions[selectedTextBox] - offsetY1, textBoxWidth - 2, textBoxHeight);
	}
}

void selectFunction()
{
	if (onClickFunction == true && showFloatingWindow == true)
	{
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 148 && mouseY <= 165)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 148 - 4, floatingFunctionWidth, 22);
			funcType[selectedTextBox] = 1;
			onClickFunction = false;
		}
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 123 && mouseY <= 140)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 123 - 4, floatingFunctionWidth, 22);
			funcType[selectedTextBox] = 2;
			onClickFunction = false;
		}
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 98 && mouseY <= 115)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 98, floatingFunctionWidth, 23);
			funcType[selectedTextBox] = 3;
			onClickFunction = false;
		}
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 73 && mouseY <= 90)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 73, floatingFunctionWidth, 23);
			funcType[selectedTextBox] = 4;
			onClickFunction = false;
		}
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 48 && mouseY <= 65)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 48 + 4, floatingFunctionWidth, 23);
			funcType[selectedTextBox] = 5;
			onClickFunction = false;
		}
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 23 && mouseY <= 40)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 23 + 5, floatingFunctionWidth, 23);
			funcType[selectedTextBox] = 6;
			onClickFunction = false;
		}
		if (mouseX >= 1000 && mouseX <= 1192 && mouseY >= 0 && mouseY <= 20)
		{
			iSetColor(255, 165, 0);
			iRectangle(floatingFunctionX, 0 + 5, floatingFunctionWidth, 23);
			funcType[selectedTextBox] = 7;
			onClickFunction = false;
		}
	}
}
double evaluateLinearFunction(double a, double b, double x)
{
	return a * x + b;
}
double evaluateSinFunction(double d, double x)
{
	return d * sin(x);
}
double evaluateCosFunction(double d, double x)
{
	return d * cos(x);
}
double evaluateTanFunction(double d, double x)
{
	return d * tan(x);
}
double evaluateAsinFunction(double d, double x)
{
	return d * asin(x);
}
double evaluateAcosFunction(double d, double x)
{
	return d * acos(x);
}
double evaluateAtanFunction(double d, double x)
{
	return d * atan(x);
}
double evaluateAcotFunction(double d, double x)
{
	return d * (atan(1 / x));
}
double evaluateASecFunction(double d, double x)
{
	return d * (acos(1 / x));
}
double evaluateAcosecFunction(double d, double x)
{
	return d * (asin(1 / x));
}
double evaluateExpFunction(double d, double x)
{
	return d * pow(2.71828182845904523536, x);
}
double evaluateLogFunction(double d, double x, double base)
{
	return d * log(x) / log(base);
}
double evaluateLnFunction(double d, double x)
{
	return d * log(x);
}
double evaluatePolynomialFunction(double a, double b, double c)
{
	return a * pow(b, c);
}
void warning()
{
	if (func[selectedTextBox] != '\0' && funcType[selectedTextBox] == 0 && showFloatingWindow == true && selectedTextBox != 0 && typeState == true)
	{
		iSetColor(255, 165, 0);
		iText(60, windowHeight - 30 - textBoxPositions[selectedTextBox] - offsetY1, "No functions selected!", GLUT_BITMAP_HELVETICA_12, 0.0);
	}
}
void readSin(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * sin(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf sin(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf sin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = sin(mx - c) type equation
		b = 0; // To make it y = sin(mx + (-c))
		funcSpec[i][1][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf sin(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf sin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = sin(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf sin(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf sin(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * sin(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = sin(mx) type equation
		funcSpec[i][1][1] = 1;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf sin(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf sin(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * sin(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = sin(mx - c) type equation
		b = 0; // To make it y = sin(mx + (-c))
		funcSpec[i][1][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf sin(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf sin(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * sin(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = sin(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf sin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf sin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = sin(mx - c) type equation
		b = 0; // To make it y = sin(mx + (-c))
		funcSpec[i][1][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf sin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf sin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = sin(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sin(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf sin(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf sin(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * sin(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = sin(mx) type equation
		funcSpec[i][1][1] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readCos(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * cos(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf cos(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf cos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))

	{
		// For y = cos(mx - c) type equation
		b = 0; // To make it y = cos(mx + (-c))
		funcSpec[i][1][2] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf cos(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf cos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = cos(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][2] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf cos(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf cos(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * cos(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = cos(mx) type equation
		funcSpec[i][1][2] = 1;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf cos(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf cos(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * cos(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = cos(mx - c) type equation
		b = 0; // To make it y = cos(mx + (-c))
		funcSpec[i][1][2] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf cos(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf cos(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * cos(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = cos(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][2] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf cos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf cos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = cos(mx - c) type equation
		b = 0; // To make it y = cos(mx + (-c))
		funcSpec[i][1][2] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf cos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf cos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = cos(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][2] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cos(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf cos(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf cos(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * cos(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = cos(mx) type equation
		funcSpec[i][1][2] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readTan(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * tan(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf tan(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf tan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * tan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = tan(mx - c) type equation
		b = 0; // To make it y = tan(mx + (-c))
		funcSpec[i][1][3] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf tan(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf tan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * tan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = tan(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][3] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf tan(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf tan(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * tan(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = tan(mx) type equation
		funcSpec[i][1][3] = 1;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf tan(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf tan(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * tan(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = tan(mx - c) type equation
		b = 0; // To make it y = tan(mx + (-c))
		funcSpec[i][1][3] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf tan(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf tan(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * tan(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = tan(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][3] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf tan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf tan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * tan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = tan(mx - c) type equation
		b = 0; // To make it y = tan(mx + (-c))
		funcSpec[i][1][3] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf tan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf tan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * tan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = tan(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][3] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * tan(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf tan(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf tan(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * tan(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = tan(mx) type equation
		funcSpec[i][1][3] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readCot(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * cot(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf cot(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf cot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = cot(mx - c) type equation
		b = 0; // To make it y = cot(mx + (-c))
		funcSpec[i][1][4] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf cot(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf cot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = cot(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][4] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf cot(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf cot(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * cot(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = cot(mx) type equation
		funcSpec[i][1][4] = 1;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf cot(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf cot(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * cot(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = cot(mx - c) type equation
		b = 0; // To make it y = cot(mx + (-c))
		funcSpec[i][1][4] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf cot(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf cot(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * cot(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = cot(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][4] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf cot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf cot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = cot(mx - c) type equation
		b = 0; // To make it y = cot(mx + (-c))
		funcSpec[i][1][4] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf cot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf cot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = cot(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][4] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cot(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf cot(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf cot(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * cot(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = cot(mx) type equation
		funcSpec[i][1][4] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readCosec(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf cosec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf cosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = cosec(mx - c) type equation
		b = 0; // To make it y = cosec(mx + (-c))
		funcSpec[i][1][5] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf cosec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf cosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = cosec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][5] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf cosec(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf cosec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = cosec(mx) type equation
		funcSpec[i][1][5] = 1;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf cosec(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf cosec(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = cosec(mx - c) type equation
		b = 0; // To make it y = cosec(mx + (-c))
		funcSpec[i][1][5] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf cosec(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf cosec(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = cosec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][5] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf cosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf cosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = cosec(mx - c) type equation
		b = 0; // To make it y = cosec(mx + (-c))
		funcSpec[i][1][5] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf cosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf cosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = cosec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][5] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * cosec(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf cosec(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf cosec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * cosec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = cosec(mx) type equation
		funcSpec[i][1][5] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readSec(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * sec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf sec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf sec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = sec(mx - c) type equation
		b = 0; // To make it y = sec(mx + (-c))
		funcSpec[i][1][6] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf sec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf sec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = sec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][6] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf sec(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf sec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * sec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = sec(mx) type equation
		funcSpec[i][1][6] = 1;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf sec(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf sec(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * sec(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = sec(mx - c) type equation
		b = 0; // To make it y = sec(mx + (-c))
		funcSpec[i][1][6] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf sec(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf sec(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * sec(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = sec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][6] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf sec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf sec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = sec(mx - c) type equation
		b = 0; // To make it y = sec(mx + (-c))
		funcSpec[i][1][6] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf sec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf sec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * sec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = sec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][1][6] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * sec(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf sec(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf sec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * sec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = sec(mx) type equation
		funcSpec[i][1][6] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readASin(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * asin(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf asin(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf asin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = asin(mx - c) type equation
		b = 0; // To make it y = asin(mx + (-c))
		funcSpec[i][2][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf asin(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf asin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = asin(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf asin(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf asin(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * asin(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = asin(mx) type equation
		funcSpec[i][2][1] = 1;

		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf asin(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf asin(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * asin(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = asin(mx - c) type equation
		b = 0; // To make it y = asin(mx + (-c))
		funcSpec[i][2][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf asin(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf asin(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * asin(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = asin(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf asin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf asin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asin(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = asin(mx - c) type equation
		b = 0; // To make it y = asin(mx + (-c))
		funcSpec[i][2][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf asin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf asin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asin(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = asin(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][1] = 1;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asin(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf asin(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf asin(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * asin(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = asin(mx) type equation
		funcSpec[i][2][1] = 1;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readACos(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * acos(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf acos(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf acos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = acos(mx - c) type equation
		b = 0; // To make it y = acos(mx + (-c))
		funcSpec[i][2][2] = 2;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf acos(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf acos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = acos(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][2] = 2;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf acos(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf acos(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * acos(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = acos(mx) type equation
		funcSpec[i][2][2] = 2;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf acos(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf acos(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * acos(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = acos(mx - c) type equation
		b = 0; // To make it y = acos(mx + (-c))
		funcSpec[i][2][2] = 2;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf acos(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf acos(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * acos(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = acos(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][2] = 2;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf acos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf acos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acos(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = acos(mx - c) type equation
		b = 0; // To make it y = acos(mx + (-c))
		funcSpec[i][2][2] = 2;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf acos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf acos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acos(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = acos(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][2] = 2;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acos(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf acos(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf acos(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * acos(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = acos(mx) type equation
		funcSpec[i][2][2] = 2;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readATan(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * atan(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf atan(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf atan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * atan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = atan(mx - c) type equation
		b = 0; // To make it y = atan(mx + (-c))
		funcSpec[i][2][3] = 3;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf atan(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf atan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * atan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = atan(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][3] = 3;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf atan(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf atan(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * atan(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = atan(mx) type equation
		funcSpec[i][2][3] = 3;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf atan(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf atan(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * atan(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = atan(mx - c) type equation
		b = 0; // To make it y = atan(mx + (-c))
		funcSpec[i][2][3] = 3;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf atan(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf atan(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * atan(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = atan(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][3] = 3;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf atan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf atan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * atan(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = atan(mx - c) type equation
		b = 0; // To make it y = atan(mx + (-c))
		funcSpec[i][2][3] = 3;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf atan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf atan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * atan(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = atan(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][3] = 3;
	}
	else if ((sscanf(func[i], "f(x) = %lf * atan(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf atan(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf atan(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * atan(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = atan(mx) type equation
		funcSpec[i][2][3] = 3;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readACot(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * acot(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf acot(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf acot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = acot(mx - c) type equation
		b = 0; // To make it y = acot(mx + (-c))
		funcSpec[i][2][4] = 4;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf acot(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf acot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = acot(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][4] = 4;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf acot(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf acot(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * acot(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = acot(mx) type equation
		funcSpec[i][2][4] = 4;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf acot(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf acot(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * acot(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = acot(mx - c) type equation
		b = 0; // To make it y = acot(mx + (-c))
		funcSpec[i][2][4] = 4;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf acot(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf acot(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * acot(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = acot(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][4] = 4;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf acot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf acot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acot(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = acot(mx - c) type equation
		b = 0; // To make it y = acot(mx + (-c))
		funcSpec[i][2][4] = 4;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf acot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf acot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acot(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = acot(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][4] = 4;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acot(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf acot(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf acot(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * acot(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = acot(mx) type equation
		funcSpec[i][2][4] = 4;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readACosec(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf acosec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf acosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = acosec(mx - c) type equation
		b = 0; // To make it y = acosec(mx + (-c))
		funcSpec[i][2][5] = 5;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf acosec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf acosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = acosec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][5] = 5;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf acosec(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf acosec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = acosec(mx) type equation
		funcSpec[i][2][5] = 5;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf acosec(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf acosec(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = acosec(mx - c) type equation
		b = 0; // To make it y = acosec(mx + (-c))
		funcSpec[i][2][5] = 5;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf acosec(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf acosec(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = acosec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][5] = 5;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf acosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf acosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = acosec(mx - c) type equation
		b = 0; // To make it y = acosec(mx + (-c))
		funcSpec[i][2][5] = 5;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf acosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf acosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = acosec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][5] = 5;
	}
	else if ((sscanf(func[i], "f(x) = %lf * acosec(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf acosec(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf acosec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * acosec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = acosec(mx) type equation
		funcSpec[i][2][5] = 5;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readASec(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * asec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf asec(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf asec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = asec(mx - c) type equation
		b = 0; // To make it y = asec(mx + (-c))
		funcSpec[i][2][6] = 6;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf asec(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf asec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = asec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][6] = 6;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf asec(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf asec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * asec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = asec(mx) type equation
		funcSpec[i][2][6] = 6;
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf asec(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf asec(%lfx - %lf)", &d, &a, &c) == 3 || (sscanf(func[i], "f(x) = %lf * asec(%lfx - %lf)", &d, &a, &c) == 3))
	{
		// For y = asec(mx - c) type equation
		b = 0; // To make it y = asec(mx + (-c))
		funcSpec[i][2][6] = 6;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf asec(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf asec(%lfx + %lf)", &d, &a, &b) == 3 || (sscanf(func[i], "f(x) = %lf * asec(%lfx + %lf)", &d, &a, &b) == 3))
	{
		// For y = asec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][6] = 6;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf asec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf asec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asec(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4))
	{
		// For y = asec(mx - c) type equation
		b = 0; // To make it y = asec(mx + (-c))
		funcSpec[i][2][6] = 6;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf asec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf asec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || (sscanf(func[i], "f(x) = %lf * asec(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4))
	{
		// For y = asec(mx + c) type equation
		// Do nothing as coefficients are already assigned
		funcSpec[i][2][6] = 6;
	}
	else if ((sscanf(func[i], "f(x) = %lf * asec(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf asec(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf asec(%lfx)+ %lf", &d, &a, &e) == 3 || (sscanf(func[i], "f(x) = %lf * asec(%lfx)+ %lf", &d, &a, &e) == 3))
	{
		// For y = asec(mx) type equation
		funcSpec[i][2][6] = 6;
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readExp(int i, double &a, double &b, double &c, double &d, double &e)
{
	if ((sscanf(func[i], "f(x) = %lf * e^(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4) || sscanf(func[i], "f(x) = %lf e^(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf * pow(e, %lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf pow(e, %lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4)
	{
		// For y = e^(mx - c) type equation
		b = 0; // To make it y = e^(mx + (-c))
	}
	else if ((sscanf(func[i], "f(x) = %lf * e^(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4) || sscanf(func[i], "f(x) = %lf e^(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf * pow(e, %lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf pow(e, %lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4)
	{
		// For y = e^(mx + c) type equation
		// Do nothing as coefficients are already assigned
	}
	else if ((sscanf(func[i], "f(x) = %lf * e^(%lfx)+ (%lf)", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf  e^(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf * pow(e, %lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf pow(e, %lfx)+ (%lf)", &d, &a, &e) == 3)
	{
		// For y = e^(mx) type equation
		b = 0;
	}
	else if ((sscanf(func[i], "f(x) = %lf * e^(%lfx - %lf)", &d, &a, &c) == 3) || sscanf(func[i], "f(x) = %lf e^(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf * pow(e, %lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf pow(e, %lfx - %lf)", &d, &a, &c) == 3)
	{
		// For y = e^(mx - c) type equation
		b = 0; // To make it y = e^(mx + (-c))
	}
	else if ((sscanf(func[i], "f(x) = %lf * e^(%lfx + %lf)", &d, &a, &b) == 3) || sscanf(func[i], "f(x) = %lf e^(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf * pow(e, %lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf pow(e, %lfx + %lf)", &d, &a, &b) == 3)
	{
		// For y = e^(mx + c) type equation
		// Do nothing as coefficients are already assigned
	}
	else if ((sscanf(func[i], "f(x) = %lf * e^(%lfx)+ %lf", &d, &a, &e) == 3) || sscanf(func[i], "f(x) = %lf e^(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf * pow(e, %lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf pow(e, %lfx)+ %lf", &d, &a, &e) == 3)
	{
		// For y = e^(mx) type equation
		b = 0;
	}
}
void readLog(int i, double &a, double &b, double &c, double &d, double &e, double &base)
{
	if (sscanf(func[i], "f(x) = %lf * log(%lfx - %lf, %lf)+ (%lf)", &d, &a, &c, &base, &e) == 5 || sscanf(func[i], "f(x) = %lf log(%lfx - %lf, %lf)+ (%lf)", &d, &a, &c, &base, &e) == 5 || sscanf(func[i], "f(x) = %lf log(%lfx - %lf, %lf)+ %lf", &d, &a, &c, &base, &e) == 5 || sscanf(func[i], "f(x) = %lf * log(%lfx - %lf, %lf)+ %lf", &d, &a, &c, &base, &e) == 5)
	{
		// For y = log(mx - c) type equation
		b = 0; // To make it y = log(mx + (-c))
	}
	else if (sscanf(func[i], "f(x) = %lf * log(%lfx + %lf, %lf)+ (%lf)", &d, &a, &b, &base, &e) == 5 || sscanf(func[i], "f(x) = %lf log(%lfx + %lf, %lf)+ (%lf)", &d, &a, &b, &base, &e) == 5 || sscanf(func[i], "f(x) = %lf log(%lfx + %lf, %lf)+ %lf", &d, &a, &b, &base, &e) == 5 || sscanf(func[i], "f(x) = %lf * log(%lfx + %lf, %lf)+ %lf", &d, &a, &b, &base, &e) == 5)
	{
		// For y = log(mx + c) type equation
		// Do nothing as coefficients are already assigned
	}
	else if (sscanf(func[i], "f(x) = %lf * log(%lfx, %lf)+ (%lf)", &d, &a, &base, &e) == 4 || sscanf(func[i], "f(x) = %lf log(%lfx, %lf)+ (%lf)", &d, &a, &base, &e) == 4 || sscanf(func[i], "f(x) = %lf log(%lfx, %lf)+ %lf", &d, &a, &base, &e) == 4 || sscanf(func[i], "f(x) = %lf * log(%lfx, %lf)+ %lf", &d, &a, &base, &e) == 4)
	{
		// For y = log(mx) type equation
		b = 0;
	}
	else if (sscanf(func[i], "f(x) = %lf * log(%lfx - %lf, %lf)", &d, &a, &c, &base) == 4 || sscanf(func[i], "f(x) = %lf log(%lfx - %lf, %lf)", &d, &a, &c, &base) == 4 || sscanf(func[i], "f(x) = %lf log(%lfx - %lf, %lf)", &d, &a, &c, &base) == 4 || sscanf(func[i], "f(x) = %lf * log(%lfx - %lf, %lf)", &d, &a, &c, &base) == 4)
	{
		// For y = log(mx - c) type equation
		b = 0; // To make it y = log(mx + (-c))
	}
	else if (sscanf(func[i], "f(x) = %lf * log(%lfx + %lf, %lf)", &d, &a, &b, &base) == 4 || sscanf(func[i], "f(x) = %lf log(%lfx + %lf, %lf)", &d, &a, &b, &base) == 4 || sscanf(func[i], "f(x) = %lf log(%lfx + %lf, %lf)", &d, &a, &b, &base) == 4 || sscanf(func[i], "f(x) = %lf * log(%lfx + %lf, %lf)", &d, &a, &b, &base) == 4)
	{
		// For y = log(mx + c) type equation
		// Do nothing as coefficients are already assigned
	}
	else if (sscanf(func[i], "f(x) = %lf * log(%lfx, %lf)", &d, &a, &base) == 3 || sscanf(func[i], "f(x) = %lf log(%lfx, %lf)", &d, &a, &base) == 3 || sscanf(func[i], "f(x) = %lf log(%lfx, %lf)", &d, &a, &base) == 3 || sscanf(func[i], "f(x) = %lf * log(%lfx, %lf)", &d, &a, &base) == 3)
	{
		// For y = log(mx) type equation
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readLn(int i, double &a, double &b, double &c, double &d, double &e)
{
	if (sscanf(func[i], "f(x) = %lf * ln(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf ln(%lfx - %lf)+ (%lf)", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf ln(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4 || sscanf(func[i], "f(x) = %lf * ln(%lfx - %lf)+ %lf", &d, &a, &c, &e) == 4)
	{
		// For y = ln(mx - c) type equation
		b = 0; // To make it y = ln(mx + (-c))
	}
	else if (sscanf(func[i], "f(x) = %lf * ln(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf ln(%lfx + %lf)+ (%lf)", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf ln(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4 || sscanf(func[i], "f(x) = %lf * ln(%lfx + %lf)+ %lf", &d, &a, &b, &e) == 4)
	{
		// For y = ln(mx + c) type equation
		// Do nothing as coefficients are already assigned
	}
	else if (sscanf(func[i], "f(x) = %lf * ln(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx)+ (%lf)", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf * ln(%lfx)+ %lf", &d, &a, &e) == 3)
	{
		// For y = ln(mx) type equation
		b = 0;
	}
	else if (sscanf(func[i], "f(x) = %lf * ln(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx - %lf)", &d, &a, &c) == 3 || sscanf(func[i], "f(x) = %lf * ln(%lfx - %lf)", &d, &a, &c) == 3)
	{
		// For y = ln(mx - c) type equation
		b = 0; // To make it y = ln(mx + (-c))
	}
	else if (sscanf(func[i], "f(x) = %lf * ln(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx + %lf)", &d, &a, &b) == 3 || sscanf(func[i], "f(x) = %lf * ln(%lfx + %lf)", &d, &a, &b) == 3)
	{
		// For y = ln(mx + c) type equation
		// Do nothing as coefficients are already assigned
	}
	else if (sscanf(func[i], "f(x) = %lf * ln(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf ln(%lfx)+ %lf", &d, &a, &e) == 3 || sscanf(func[i], "f(x) = %lf * ln(%lfx)+ %lf", &d, &a, &e) == 3)
	{
		// For y = ln(mx) type equation
		b = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readQuadratic(int i, double &a, double &b, double &c)
{
	if (sscanf(func[i], "f(x) = %lfx^2 + %lfx + %lf", &a, &b, &c) == 3 || sscanf(func[i], "f(x) = %lf pow(x,2) + %lfx + %lf", &a, &b, &c) == 3)
	{
	}
	else if (sscanf(func[i], "f(x) = %lfx^2 + %lfx ", &a, &b) == 2 || sscanf(func[i], "f(x) = %lf pow(x,2) + %lfx ", &a, &b) == 2)
	{
		c = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^2 + %lf", &a, &c) == 2 || sscanf(func[i], "f(x) = %lf pow(x,2) + %lf", &a, &c) == 2)
	{

		b = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^2 ", &a) == 1 || sscanf(func[i], "f(x) = %lf pow(x,2) ", &a) == 1)
	{

		b = 0;
		c = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readCubic(int i, double &a, double &b, double &c, double &d)
{
	if (sscanf(func[i], "f(x) = %lfx^3 + %lfx^2 + %lfx + %lf", &a, &b, &c, &d) == 4 || sscanf(func[i], "f(x) = %lf pow(x,3) + %lf pow(x,2) + %lfx + %lf", &a, &b, &c, &d) == 4)
	{
	}
	else if (sscanf(func[i], "f(x) = %lfx^3 + %lfx^2 + %lfx ", &a, &b, &c) == 3 || sscanf(func[i], "f(x) = %lf pow(x,3) + %lf pow(x,2) + %lfx ", &a, &b, &c) == 3)
	{
		d = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^3 + %lfx^2 ", &a, &b) == 2 || sscanf(func[i], "f(x) = %lf pow(x,3) + %lf pow(x,2) ", &a, &b) == 2)
	{
		c = 0;
		d = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^3 + %lf", &a, &d) == 2 || sscanf(func[i], "f(x) = %lf pow(x,3) + %lf", &a, &d) == 2)
	{
		b = 0;
		c = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^3 ", &a) == 1 || sscanf(func[i], "f(x) = %lf pow(x,3) ", &a) == 1)
	{
		b = 0;
		c = 0;
		d = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}
void readTetra(int i, double &a, double &b, double &c, double &d, double &e)
{
	if (sscanf(func[i], "f(x) = %lfx^4 + %lfx^3 + %lfx^2 + %lfx + %lf", &a, &b, &c, &d, &e) == 5 || sscanf(func[i], "f(x) = %lf pow(x,4) + %lf pow(x,3) + %lf pow(x,2) + %lfx + %lf", &a, &b, &c, &d, &e) == 5)
	{
	}
	else if (sscanf(func[i], "f(x) = %lfx^4 + %lfx^3 + %lfx^2 + %lfx ", &a, &b, &c, &d) == 4 || sscanf(func[i], "f(x) = %lf pow(x,4) + %lf pow(x,3) + %lf pow(x,2) + %lfx ", &a, &b, &c, &d) == 4)
	{
		e = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^4 + %lfx^3 + %lfx^2 ", &a, &b, &c) == 3 || sscanf(func[i], "f(x) = %lf pow(x,4) + %lf pow(x,3) + %lf pow(x,2) ", &a, &b, &c) == 3)
	{
		d = 0;
		e = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^4 + %lfx^3 ", &a, &b) == 2 || sscanf(func[i], "f(x) = %lf pow(x,4) + %lf pow(x,3) ", &a, &b) == 2)
	{
		c = 0;
		d = 0;
		e = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^4 + %lf", &a, &e) == 2 || sscanf(func[i], "f(x) = %lf pow(x,4) + %lf", &a, &e) == 2)
	{
		b = 0;
		c = 0;
		d = 0;
	}
	else if (sscanf(func[i], "f(x) = %lfx^4 ", &a) == 1 || sscanf(func[i], "f(x) = %lf pow(x,4) ", &a) == 1)
	{
		b = 0;
		c = 0;
		d = 0;
		e = 0;
	}
	else
	{
		// If it's not in the expected format, skip to the next function
		drawState[i] = 3;
	}
}

void drawFunc()
{

	for (int i = 1; i < textBoxCount; ++i)
	{
		if (strlen(func[i]) <= 7 || funcType[i] == 0 || showCurve[i] == 1)
		{
			continue; // Skip empty or invalid entries
		}

		else
		{

			if (funcType[i] == 1)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;										 // Adjust the scale as needed

				// Initialize the first point
				double x1 = -100000 / scale;
				double a = 0, b = 0, c = 0;

				// Check the structure of the function string
				if (sscanf(func[i], "f(x) = %lfx %lf", &a, &c) == 2)
				{
					// For y = mx - c type equation
					b = 0; // To make it y = mx + (-c)
				}
				else if (sscanf(func[i], "f(x) = %lfx + (%lf)", &a, &b) == 2)
				{

					// For y = mx + c type equation
					// Do nothing as coefficients are already assigned
				}
				else if (sscanf(func[i], "f(x) = %lfx", &a) == 1)
				{
					// For y = mx type equation
					b = 0;
				}
				else
				{
					// If it's not in the expected format, skip to the next function

					drawState[i] = 3;

					continue;
				}

				double y1 = evaluateLinearFunction(a, b, x1) - c;
				int screenY1 = y1 * scale;
				int screenX1 = -100000 + screenWidth / 2;

				for (int j = -100000; j <= 100000; j += 1)
				{
					drawState[i] = 2;

					double x2 = j / scale;
					double y2 = evaluateLinearFunction(a, b, x2) - c;
					int screenY2 = y2 * scale;
					int screenX2 = j + screenWidth / 2;

					iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points

					// Update the previous point
					x1 = x2;
					y1 = y2;
					screenY1 = screenY2;
					screenX1 = screenX2;
				}
			}
			if (funcType[i] == 2)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;										 // Adjust the scale as needed

				// Initialize the first point
				double x1 = -100000 / scale;
				double a = 0, b = 0, c = 0, d = 1, e = 0;

				// Check the structure of the function string
				readSin(i, a, b, c, d, e);
				readCos(i, a, b, c, d, e);
				readTan(i, a, b, c, d, e);
				readCot(i, a, b, c, d, e);
				readCosec(i, a, b, c, d, e);
				readSec(i, a, b, c, d, e);

				if (funcSpec[i][1][1] == 1)
				{

					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateSinFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateSinFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;

						iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points

						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][1][2] == 1)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateCosFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateCosFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;

						iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points

						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][1][3] == 1)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateTanFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					// Inside your loop
					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;

						// Check if tan is undefined

						double y2 = evaluateTanFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][1][4] == 1)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = 1 / evaluateTanFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					// Inside your loop
					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;

						// Check if tan is undefined

						double y2 = 1 / evaluateTanFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][1][5] == 1)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = 1 / evaluateSinFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					// Inside your loop
					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;

						// Check if tan is undefined

						double y2 = 1 / evaluateSinFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][1][6] == 1)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = 1 / evaluateCosFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					// Inside your loop
					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;

						// Check if tan is undefined

						double y2 = 1 / evaluateCosFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
			}
			if (funcType[i] == 3)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;										 // Adjust the scale as needed

				// Initialize the first point
				double x1 = -100000 / scale;
				double a = 0, b = 0, c = 0, d = 1, e = 0;

				// Check the structure of the function string
				readASin(i, a, b, c, d, e);
				readACos(i, a, b, c, d, e);
				readATan(i, a, b, c, d, e);
				readACot(i, a, b, c, d, e);
				readACosec(i, a, b, c, d, e);
				readASec(i, a, b, c, d, e);

				if (funcSpec[i][2][1] == 1)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateAsinFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateAsinFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				if (funcSpec[i][2][2] == 2)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateAcosFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateAcosFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				if (funcSpec[i][2][3] == 3)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateAtanFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateAtanFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				if (funcSpec[i][2][4] == 4)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateAcotFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateAcotFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				if (funcSpec[i][2][5] == 5)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateAcosecFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;
					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateAcosecFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				if (funcSpec[i][2][6] == 6)
				{
					double y1Temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateASecFunction(d, y1Temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;
					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2Temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateASecFunction(d, y2Temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
			}
			if (funcType[i] == 4)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;										 // Adjust the scale as needed

				// Initialize the first point
				double x1 = -100000 / scale;
				double a = 0, b = 0, c = 0, d = 1, e = 0;

				// Check the structure of the function string
				readExp(i, a, b, c, d, e);

				{
					double y1temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateExpFunction(d, y1temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;

						double x2 = j / scale;
						double y2temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateExpFunction(d, y2temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
			}
			if (funcType[i] == 5)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;										 // Adjust the scale as needed

				// Initialize the first point
				double x1 = -100000 / scale;
				double a = 0, b = 0, c = 0, d = 1, e = 0, base = 10;

				// Check the structure of the function string
				readLog(i, a, b, c, d, e, base);

				{
					double y1temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateLogFunction(d, y1temp, base) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;

						double x2 = j / scale;
						double y2temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateLogFunction(d, y2temp, base) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
			}
			if (funcType[i] == 6)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;										 // Adjust the scale as needed

				// Initialize the first point
				double x1 = -100000 / scale;
				double a = 0, b = 0, c = 0, d = 1, e = 0;

				// Check the structure of the function string
				readLn(i, a, b, c, d, e);

				{
					double y1temp = evaluateLinearFunction(a, b, x1) - c;
					double y1 = evaluateLnFunction(d, y1temp) + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;

						double x2 = j / scale;
						double y2temp = evaluateLinearFunction(a, b, x2) - c;
						double y2 = evaluateLnFunction(d, y2temp) + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						if (abs(screenY2 - screenY1) <= 1000)
						{
							iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						}
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
			}
			if (funcType[i] == 7)
			{
				iSetColor(graphColor[i][0], graphColor[i][1], graphColor[i][2]); // Red color for the graph
				long double scale = 100;
				double a = 1, b = 0, c = 0, d = 0, e = 0;
				if (funcSpec[i][3][1] == 2)
				{
					readQuadratic(i, a, b, c);

					double x1 = -100000 / scale;
					double y1 = a * x1 * x1 + b * x1 + c;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2 = a * x2 * x2 + b * x2 + c;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][3][1] == 3)
				{
					readCubic(i, a, b, c, d);
					double x1 = -100000 / scale;
					double y1 = a * x1 * x1 * x1 + b * x1 * x1 + c * x1 + d;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2 = a * x2 * x2 * x2 + b * x2 * x2 + c * x2 + d;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
				else if (funcSpec[i][3][1] == 4)
				{
					readTetra(i, a, b, c, d, e);
					double x1 = -100000 / scale;
					double y1 = a * x1 * x1 * x1 * x1 + b * x1 * x1 * x1 + c * x1 * x1 + d * x1 + e;
					int screenY1 = y1 * scale;
					int screenX1 = -100000 + screenWidth / 2;

					for (int j = -100000; j <= 100000; j += 1)
					{
						drawState[i] = 2;
						double x2 = j / scale;
						double y2 = a * x2 * x2 * x2 * x2 + b * x2 * x2 * x2 + c * x2 * x2 + d * x2 + e;
						int screenY2 = y2 * scale;
						int screenX2 = j + screenWidth / 2;
						iLine(screenX1 - offsetX, screenY1 + screenHeight / 2 - offsetY, screenX2 - offsetX, screenY2 + screenHeight / 2 - offsetY, 2.0); // Draw a line between points
						// Update the previous point
						x1 = x2;
						y1 = y2;
						screenY1 = screenY2;
						screenX1 = screenX2;
					}
				}
			}
		}
	}
}

void iDraw()
{
	// place your drawing codes here
	iClear();

	if (homepage == false)
	{
		if (counter <= 7)
		{
			counter++;
		}
		// drawMillimeterSquares(1.0);
		drawIcons();
		drawSquares(0.0);
		darkLine(2.0);
		showLine();
		drawLabel();
		drawFunc();
		drawFloatingWindow();

		highlightTextBox();
		warning();

		textCursor();
		block();
		angle();
		add();

		keyboard();
		toggleFunction();

		floatingFunction();
		onKeyboardHighlight();
		highlightKeyboardButton();
		selectFunction();
		floatingFunction2();
		selectFunction2();
		instructions();
	}
	else
	{
		designHome();
		instructions2();
		myself();
	}
}

void iMouseMove(int x, int y)
{
	if (homepage == false)
	{
		if (!showFloatingWindow)
		{

			printf("x = %d, y= %d\n", x, y);
			// place your codes here
			offsetX -= (x - mouseX); // Reverse the sign for x-axis
			offsetY += (mouseY - y); // Invert Y-axis
			printf("offsetX %lf offsetY %lf\n", offsetX, offsetY);
			mouseX = x;
			mouseY = y;
		}
		else if (showFloatingWindow == true && showKeyboard == false)
		{
			if (!(x >= 1 && x <= textBoxWidth))
			{
				offsetX -= (x - mouseX); // Reverse the sign for x-axis
				offsetY += (mouseY - y); // Invert Y-axis
				printf("offsetX %lf offsetY %lf\n", offsetX, offsetY);
				mouseX = x;
				mouseY = y;
			}
		}
		else if (showFloatingWindow == true && showKeyboard == true)
		{
			if (!(mouseX >= 0 && mouseX <= keyboardWidth && mouseY >= 0 && mouseY <= keyboardHeight) && !(x >= 1 && x <= textBoxWidth))
			{
				offsetX -= (x - mouseX); // Reverse the sign for x-axis
				offsetY += (mouseY - y); // Invert Y-axis
				mouseX = x;
				mouseY = y;
			}
		}

		if (showFloatingWindow)
		{
			if (x >= 1 && x <= textBoxWidth && showKeyboard == false)
			{

				if (offsetY1 <= 0)
				{
					offsetY1 += (mouseY - y);
					printf("offsetY1 %lf\n", offsetY1);
					mouseY = y;
					if (textBoxCount > 4)
					{
						if (offsetY1 < -((textBoxCount - 4) * 71))
						{
							offsetY1 = -((textBoxCount - 5) * 71);
						}
					}

					else if (textBoxCount >= 2)
					{
						if (offsetY1 < -((textBoxCount - 1.5) * 71))
						{
							offsetY1 = 0;
						}
					}
					else
					{
						if (offsetY1 < -((textBoxCount - 0.5) * 71))
						{
							offsetY1 = 0;
						}
					}
				}
				else if (offsetY1 > 0)
				{
					offsetY1 = 0;
				}
			}
			else
			{
				if (!(mouseY >= 0 && mouseY <= keyboardHeight + keyboardIconHeight))
				{
					if (offsetY1 <= 0)
					{
						offsetY1 += (mouseY - y);
						printf("offsetY1 %lf\n", offsetY1);
						mouseY = y;
						if (textBoxCount > 4)
						{
							if (offsetY1 < -((textBoxCount - 4) * 71))
							{
								offsetY1 = -((textBoxCount - 5) * 71);
							}
						}

						else if (textBoxCount >= 2)
						{
							if (offsetY1 < -((textBoxCount - 1.5) * 71))
							{
								offsetY1 = 0;
							}
						}
						else
						{
							if (offsetY1 < -((textBoxCount - 0.5) * 71))
							{
								offsetY1 = 0;
							}
						}
					}
					else if (offsetY1 > 0)
					{
						offsetY1 = 0;
					}
				}
			}
		}
	}
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// place your codes here
		printf("x = %d, y= %d\n", mx, my);
		//		x += 10;
		//		y += 10;
		mouseX = mx;
		mouseY = my;
		if (homepage == false)
		{
			if (mx >= angleX && mx <= angleX + angleWidth && my >= angleY && my <= angleY + angleHeight)
			{
				if (showFloatingWindow == false)
				{
					showFloatingWindow = !showFloatingWindow;
					rightAngle = !rightAngle;
				}
			}
			if (mx >= angleX1 && mx <= angleX1 + angleWidth && my >= angleY && my <= angleY + angleHeight)
			{
				if (showFloatingWindow == true)
				{
					showFloatingWindow = !showFloatingWindow;
					leftAngle = !leftAngle;
					rightAngle = !rightAngle;
				}
			}

			if (mx >= angleX && mx <= angleX + angleWidth && my >= angleY && my <= angleY + angleHeight)
			{
				if (textBoxCount < MAX_TEXTBOXES)
				{
					if (textBoxCount == 1)
					{
						// Add a new textbox position
						textBoxPositions[textBoxCount] = textBoxCount * 70;
						++textBoxCount;
						indexNumber[textBoxCount] = 6;
						strcpy(func[textBoxCount - 1], "f(x) =");
						func[textBoxCount - 1][indexNumber[textBoxCount]] = '\0';
					}
					else if (leftAngle == true)
					{
						textBoxPositions[textBoxCount] = textBoxCount * 70;
						++textBoxCount;
						indexNumber[textBoxCount] = 6;
						strcpy(func[textBoxCount - 1], "f(x) =");
						func[textBoxCount - 1][indexNumber[textBoxCount]] = '\0';
					}
				}
			}
			if (mx >= keyboardX && mx <= keyboardX + keyboardIconWidth && my >= keyboardY && my <= keyboardY + keyboardIconHeight)
			{
				if (showKeyboard == false)
				{
					showKeyboard = !showKeyboard;
				}
			}
			if (mx >= keyboardX && mx <= keyboardX + keyboardIconWidth && my >= keyboardY + keyboardHeight && my <= keyboardHeight + keyboardY + keyboardIconHeight)
			{
				if (showKeyboard == true)
				{
					showKeyboard = !showKeyboard;
				}
			}
			if (mouseX >= functionX && mouseX <= functionX + functionWidth && mouseY >= functionY && mouseY <= functionY + functionHeight)
			{
				if (onClickFunction == true)
				{
					onClickFunction = false;
				}
				else
				{
					onClickFunction = true;
				}
			}
			if (showFloatingWindow == true && showKeyboard == false)
			{
				if (!(mouseX >= 0 && mouseX <= textBoxWidth && mouseY >= windowHeight - 40 - offsetY1 - textBoxPositions[selectedTextBox] && mouseY <= windowHeight - textBoxPositions[selectedTextBox] - offsetY1 + 25))
				{
					typeState = false;
				}
			}
			if (showFloatingWindow == true && showKeyboard == true)
			{

				typeState = false;
			}
			keyboardMap();
			if (mouseX >= 320 && mouseX <= 320 + 24 && mouseY >= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] + 6 && mouseY <= windowHeight - offsetY1 - textBoxPositions[selectedTextBox] + 28 + 6)
			{
				if (showFuncList == false)
				{
					showFuncList = !showFuncList;
				}
				else
				{
					showFuncList = !showFuncList;
				}
			}
			if (mouseX >= 320 && mouseX <= 320 + 24 && mouseY >= windowHeight - textBoxPositions[selectedTextBox] - offsetY1 - 18 && mouseY <= windowHeight - textBoxPositions[selectedTextBox] - offsetY1 + 17 && showFuncList == false)
			{

				indexNumber[selectedTextBox] = 0;
				funcType[selectedTextBox] = 0;
				func[selectedTextBox][indexNumber[selectedTextBox]] = '\0';
				int i, j;
				// Copy data from the next textboxes until it reaches textBoxCount
				for (i = selectedTextBox + 1, j = selectedTextBox; i < textBoxCount; i++, j++)
				{
					// Copy data from the next textbox

					strcpy(func[j], func[i]);
					funcType[j] = funcType[i];
					drawState[j] = drawState[i];
					indexNumber[j + 1] = strlen(func[j]);
					cursorIndex[j] = cursorIndex[i];

					// Update textbox positions
					textBoxPositions[j] = (j) * 70;
				}
				textBoxCount--;
				// indexNumber[i-1]=0;
				// func[i-1][indexNumber[selectedTextBox]]='\0';
				textBoxPositions[i - 1] = 0;
				funcType[i - 1] = 0;
				drawState[i - 1] = 0;
				// cursorIndex[i-1]=0;
			}
			if (mouseX >= 11 && mouseX <= 41 && mouseY >= windowHeight - textBoxPositions[selectedTextBox] - offsetY1 - 10 && mouseY <= windowHeight - textBoxPositions[selectedTextBox] - offsetY1 + 20)
			{

				if (showCurve[selectedTextBox] == 1)
				{
					showCurve[selectedTextBox] = 0;
				}
				else
				{
					showCurve[selectedTextBox] = 1;
				}
			}
			if (mouseX >= 1150 && mouseX <= 1187 && mouseY >= 550 && mouseY <= 590)
			{
				if (lightMode == false)
				{
					lightMode = !lightMode;
					int temp = r;
					r = r1;
					r1 = temp;
					temp = g;
					g = g1;
					g1 = temp;
					temp = b;
					b = b1;
					b1 = temp;
				}
				else
				{
					lightMode = !lightMode;
					int temp = r;
					r = r1;
					r1 = temp;
					temp = g;
					g = g1;
					g1 = temp;
					temp = b;
					b = b1;
					b1 = temp;
				}
			}
			if (mouseX >= 1150 && mouseX <= 1187 && mouseY >= 510 && mouseY <= 550)
			{
				if (showAbout == false)
				{
					showAbout = true;
				}
				else
				{
					showAbout = false;
				}
			}
			if (mouseX >= 970 && mouseX <= 1000 && mouseY >= 520 && mouseY <= 550 && showAbout == true)
			{
				showAbout = false;
			}
			if (mouseX >= 1150 && mouseX <= 1187 && mouseY >= 470 && mouseY <= 510)
			{
				if (homepage == false)
				{
					homepage = true;
				}
			}
		}
		if (homepage == true)
		{
			if (mouseX >= 875 && mouseX <= 875 + 300 && mouseY >= 210 && mouseY <= 210 + 90)
			{
				homepage = false;
			}
			if (mouseX >= 875 && mouseX <= 875 + 300 && mouseY >= 110 && mouseY <= 110 + 90)
			{
				showMyself = true;
			}
			if (mouseX >= 875 && mouseX <= 875 + 300 && mouseY >= 10 && mouseY <= 10 + 90)
			{
				showAbout = true;
			}
			if (mouseX >= 970 && mouseX <= 1000 && mouseY >= 520 && mouseY <= 550 && showAbout == true)
			{
				showAbout = false;
			}
			if (mouseX >= 1148 && mouseX <= 1148 + 24 && mouseY >= 550 && mouseY <= 550 + 24 && showMyself == true)
			{
				showMyself = false;
			}
		}

		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			// place your codes here
			//		x -= 10;
			//		y -= 10;
		}
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
	if (homepage == false)
	{
		if (typeState == true)
		{
			if (key != '\r')
			{

				if (key != '\b')
				{
					if (indexNumber[selectedTextBox + 1] >= 6)
					{
						// Allow editing "f(x)=" portion
						func[selectedTextBox][indexNumber[selectedTextBox + 1]] = key;
						token = key;
					}

					indexNumber[selectedTextBox + 1]++;
					if (cursorIndex[selectedTextBox] < windowWidth - 52)
					{

						if (key == 't' || key == 'i' || key == 'f' || key == 'l')
							cursorIndex[selectedTextBox] += 7;
						else if (key == 'w' || key == 'm')
							cursorIndex[selectedTextBox] += 18;
						else if (key == '+')
							cursorIndex[selectedTextBox] += 13;

						else
							cursorIndex[selectedTextBox] += 11;
						func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
					}

					if (cursorIndex[selectedTextBox] >= windowWidth - 52)
					{
						int limit = cursorIndex[selectedTextBox];
						if (cursorIndex[selectedTextBox] == limit)
						{
							offsetX1[selectedTextBox] += 11;

							if (key == 't' || key == 'i' || key == 'f' || key == 'l')
								cursorIndex2[selectedTextBox] += 7;
							else if (key == 'w' || key == 'm')
								cursorIndex2[selectedTextBox] += 18;
							else if (key == '+')
								cursorIndex2[selectedTextBox] += 13;

							else
								cursorIndex2[selectedTextBox] += 11;
							func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
						}
					}
				}
				else
				{
					if (indexNumber[selectedTextBox + 1] > 6)
					{
						if (cursorIndex[selectedTextBox] < windowWidth - 52)
						{
							indexNumber[selectedTextBox + 1]--;
							if (key == 't' || key == 'i' || key == 'f' || key == 'l')
								cursorIndex[selectedTextBox] -= 7;
							else if (key == 'w' || key == 'm')
								cursorIndex[selectedTextBox] -= 18;
							else if (key == '+')
								cursorIndex[selectedTextBox] -= 13;
							else
								cursorIndex[selectedTextBox] -= 11;
							func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
						}
						else
						{
							if (cursorIndex2[selectedTextBox] > 0)
							{
								offsetX1[selectedTextBox] -= 11;
								indexNumber[selectedTextBox + 1]--;
								if (key == 't' || key == 'i' || key == 'f' || key == 'l')
									cursorIndex2[selectedTextBox] -= 7;
								else if (key == 'w' || key == 'm')
									cursorIndex2[selectedTextBox] -= 18;
								else if (key == '+')
									cursorIndex2[selectedTextBox] -= 13;
								else
									cursorIndex2[selectedTextBox] -= 11;
								func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
							}
							else
							{
								indexNumber[selectedTextBox + 1]--;
								if (key == 't' || key == 'i' || key == 'f' || key == 'l')
									cursorIndex[selectedTextBox] -= 7;
								else if (key == 'w' || key == 'm')
									cursorIndex[selectedTextBox] -= 18;
								else if (key == '+')
									cursorIndex[selectedTextBox] -= 13;
								else
									cursorIndex[selectedTextBox] -= 11;
								func[selectedTextBox][indexNumber[selectedTextBox + 1]] = '\0';
							}
						}
					}
				}
			}
			else
			{
				if (textBoxCount < MAX_TEXTBOXES)
				{
					// Add a new textbox position
					textBoxPositions[textBoxCount] = textBoxCount * 70;
					++textBoxCount;

					// Reset indexNumber for the new box and initialize with "f(x)="
					indexNumber[textBoxCount] = 6;
					strcpy(func[textBoxCount - 1], "f(x) =");
					func[textBoxCount - 1][indexNumber[textBoxCount]] = '\0';
				}
			}
		}
	}
}
/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key)
{
	if (key == GLUT_KEY_RIGHT && indexNumber[selectedTextBox] < strlen(func[selectedTextBox]))
	{
		// Move the cursor to the right
	}
	if (key == GLUT_KEY_LEFT && indexNumber[selectedTextBox] > 0)
	{
		// Move the cursor to the left
	}
	// place your codes for other keys here
}

int main()
{
	// place your own initialization codes here.
	iSetTimer(400, change);
	iInitialize(screenWidth, screenHeight, "GraphStudio");

	glutMotionFunc(iMouseMove);
	glutMouseFunc(iMouse);

	return 0;
}
