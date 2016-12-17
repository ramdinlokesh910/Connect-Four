//CG Project: Connect Four 
//Author: Shivani, Lokesh, Bhushan

#include "../shared/gltools.h"	// OpenGL toolkit
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#define WIDTH 800
#define HEIGHT 800
#define MARGIN 8	//Margin between the Diamonds	
#define BLACK 2
#define BLUE 3
#define GREEN 4
#define GRAY 5
#define WHITE 6
#define DIAMOND_BORDER 5
#define PULSE_SPEED 5.02	//THis is the blink speed when win
#define PULSE_MAX 6			//This is the pulse_max size 
	
using namespace std;
int player;
int column;
int row;
int field[7][6];
bool winField[7][6];
int timeSinceStart;
int oldTimeSinceStart;
int deltaTime;
int pulseDirection = 1;
int pulseAmount;
bool isGameOver;
int fieldCount;

double color[7][3] = {
	{ 1.0, 0.0, 0.0 }, // Red
	{ 1.0, 1.0, 0.0 }, // Yellow
	{ 0.0, 0.0, 0.0 }, // Black
	{ 0.0, 0.0, 1.0 }, // Blue
	{ 0.0, 1.0, 0.0 }, // Green
	{ 0.5, 0.5, 0.5 }, // Gray
	{ 1.0, 1.0, 1.0 }, // White
};

//To the draw the diamond at the particular postion we divide the screen by 7. eg:800/7
float fieldSize = WIDTH / 7;

// Initialize game
void initGame() {
	isGameOver = false;		//Initially GameOver is False
	fieldCount = 0;
	// Reset board i.e. 7 Column and 6 rows of the of diamond 
	for (int x = 0; x < 7; x++)		
		for (int y = 0; y < 6; y++) 
		{	field[x][y] = -1;
			winField[x][y] = false;
		}
	row = 5;
	column = 3;
	player = 0;
}

// Find available row position in selected column
void getAvailableRow() {
	row = -1;
	for (int y = 5; y >= 0; y--)
		if (field[column][y] == -1) {
			row = y;
			break;
		}
}

// Mark field as winning
void mark(int x, int yy) {
	winField[x][yy] = true;
}

// CHeck for victory condition and mark winning fields
bool check(int x, int y) {//check(column, row)
	bool win = false;		//Program should run at least 1 time
	int vertical = 1;//(Check in Vertical Direction i.e. |)
	int horizontal = 1;//(Check in Horizontal Direction i.e. - )
	int diagonal1 = 1;//( Check in Diagonal Direction i.e. \ )
	int diagonal2 = 1;//(Check in Diagonal Direction i.e. / )
	//char player = field[x][y];
	int yy;//vertical
	int xx;//horizontal
		   //check for vertical(|)
	for (yy = y + 1; field[x][yy] == player && yy <= 5; yy++, vertical++);//Check down
	for (yy = y - 1; field[x][yy] == player && yy >= 0; yy--, vertical++);//Check up
	if (vertical >= 4) {
		for (yy = y + 1; field[x][yy] == player && yy <= 5; yy++, mark(x, yy - 1));//mark down
		for (yy = y - 1; field[x][yy] == player && yy >= 0; yy--, mark(x, yy + 1));//mark up
		win = true;
	}
	//check for horizontal(-)
	for (xx = x - 1; field[xx][y] == player && xx >= 0; xx--, horizontal++);//Check left
	for (xx = x + 1; field[xx][y] == player && xx <= 6; xx++, horizontal++);//Check right
	if (horizontal >= 4) {
		for (xx = x - 1; field[xx][y] == player && xx >= 0; xx--, mark(xx + 1, y));//mark left
		for (xx = x + 1; field[xx][y] == player && xx <= 6; xx++, mark(xx - 1, y));//mark right
		win = true;
	}
	//check for diagonal 1 (\)
	for (xx = x - 1, yy = y - 1; field[xx][yy] == player && xx >= 0 && yy >= 0; diagonal1++, xx--, yy--);//up and left
	for (xx = x + 1, yy = y + 1; field[xx][yy] == player && xx <= 6 && yy <= 5; diagonal1++, xx++, yy++);//down and right
	if (diagonal1 >= 4) {
		for (xx = x - 1, yy = y - 1; field[xx][yy] == player && xx >= 0 && yy >= 0; xx--, yy--, mark(xx + 1, yy + 1));
		for (xx = x + 1, yy = y + 1; field[xx][yy] == player && xx <= 6 && yy <= 5; xx++, yy++, mark(xx - 1, yy - 1));
		win = true;
	}
	//check for diagonal 2(/)
	for (xx = x - 1, yy = y + 1; field[xx][yy] == player && xx >= 0 && yy <= 5; diagonal2++, xx--, yy++);//up and left
	for (xx = x + 1, yy = y - 1; field[xx][yy] == player && xx <= 6 && yy >= 0; diagonal2++, xx++, yy--);//down and right
	if (diagonal2 >= 4) {
		for (xx = x - 1, yy = y + 1; field[xx][yy] == player && xx >= 0 && yy <= 5; xx--, yy++, mark(xx + 1, yy - 1));
		for (xx = x + 1, yy = y - 1; field[xx][yy] == player && xx <= 6 && yy >= 0; xx++, yy--, mark(xx - 1, yy + 1));
		win = true;
	}

	if (win) winField[x][y] = true; //THis is the field above the pointer of the player.Starts blink if win
	return win;
}

// Handle input
void user_input(unsigned char key, int x, int y) {
	if (key == 32) { // Space key
		PlaySound(TEXT("ding.wav"), NULL, SND_ALIAS);
		if (isGameOver) {
			initGame();
		}
		//Here we fill the colors in Diamonds based on the input player 1 or 2
		else if (row >= 0) {
			field[column][row] = player;
			fieldCount++;
			// Is game over?
			if (check(column, row) || fieldCount == 42) {// IF any one is true then Game over
				isGameOver = true;
			}
			else { // if not continue as usual
				getAvailableRow();
				player++;
				if (player == 2) player = 0;
			}
		}
	}
}

//Set color of the particular field
void setColor(int c) {
	glColor3f(color[c][0], color[c][1], color[c][2]);
}

void setPlayerColor() {
	glColor3f(color[player][0], color[player][1], color[player][2]);
}

// Draw diamonds on the board
void drawDiamond(int x, int y, int offset, int color) {
	setColor(color);
	glBegin(GL_POLYGON);
	if (isGameOver) { // Animate diamonds on game over
		if (winField[x][y]) offset += pulseAmount;
	}
	//Draw co-ordinates of the Diamonds to be filled
	glVertex2i(fieldSize * (x + 0.5), fieldSize * (7 - y) - MARGIN - offset);		//Vertex point of Upper side of the Diamonds 
	glVertex2i(fieldSize * x + MARGIN + offset, fieldSize * (6.5 - y));			//Vertex point of Right side of the Diamonds 
	glVertex2i(fieldSize * (x + 0.5), fieldSize * (6 - y) + MARGIN + offset);	//Vertex point of Down side of the Diamonds 
	glVertex2i(fieldSize * (x + 1.0) - MARGIN - offset, fieldSize * (6.5 - y));		//Vertex point of Right side of the Diamonds 


	glEnd();
}

// Draw board
void drawBoard() {
	for (int x = 0; x < 7; x++)
		for (int y = 0; y < 6; y++) {
			drawDiamond(x, y, 0, BLUE);
			// Default color is black / empty
			int color = BLACK;
			// ..or set player color
			if (field[x][y] >= 0) color = field[x][y];
			drawDiamond(x, y, DIAMOND_BORDER, color);
		}
}

// Draw column for selector/Player
void drawSelector() {
	// Draw player indicator / column selector
	setPlayerColor();
	glBegin(GL_TRIANGLES);
			glVertex2i(fieldSize * (column + 0.5), fieldSize - MARGIN);
			glVertex2i(fieldSize * column + MARGIN, MARGIN);
			glVertex2i(fieldSize * (column + 1) - MARGIN, MARGIN);
	glEnd();

	// Highlight placement position above the player
	if (row >= 0) drawDiamond(column, row, DIAMOND_BORDER, GRAY);	//Look ahead and move forward
}

// Render everything
void Render() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	const int   win_width = glutGet(GLUT_WINDOW_WIDTH),
		win_height = glutGet(GLUT_WINDOW_HEIGHT);
	glClearColor(0.0, 0.0, 0.0, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, win_width, win_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
	
	drawBoard();
	if (!isGameOver) drawSelector();
	
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Update game variables
void GameLogic() {
	if (isGameOver) 
	{	pulseAmount += pulseDirection * PULSE_SPEED * deltaTime;	//Highlight the particular pulses/Win Areas 
		if (pulseDirection == 1) {
			if (pulseAmount > PULSE_MAX) 
				{
					//PlaySound(TEXT("ding.wav"), NULL, SND_ALIAS);
					pulseDirection = -1;		//Increase and decrease the pulse of diamond after win
				}
		}
		else if (pulseAmount < -PULSE_MAX) 
			{
				pulseDirection = 1;
				//PlaySound(TEXT("ding.wav"), NULL, SND_ALIAS);
			}
	}
}

// Handle input
void ArrowKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		column--;
		if (column < 0) column = 0;
		getAvailableRow();
	}
	if (key == GLUT_KEY_RIGHT) {
		column++;
		if (column > 6) column = 6;
		getAvailableRow();
	}
}

int main(int argc, char **argv) {
	FreeConsole();
	initGame();	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Connect Four");	
	glutKeyboardFunc(user_input);	//User Input
	glutSpecialFunc(ArrowKeys);		
	glutIdleFunc(GameLogic);
	glutDisplayFunc(Render);
	glutMainLoop();
	return 0;
}