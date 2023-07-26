#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "../include/setup.h"
#include "../include/line.h"
#include "../include/fileop.h"
#include "../include/bufferop.h"

#define ctrl(x) ((x) & 0x1f)
#undef KEY_ENTER
#define KEY_ENTER 10

int main(int argc, char *argv[]) {
	startNcurses();
	// Initialize important variables
	char *currentFile = NULL;
	int leftOffset = 1;
	int resY = 0, resX = 0;
	int y = 0, x = 0;
	int realX = 0;
	int lastXPos = 0;
	int ch = '\0'; 
	// Setup ncurses windows and buffer
	getmaxyx(stdscr, resY, resX);
	setupColors();
	WINDOW *bar = setupBar(resY, resX);
	WINDOW *win = setupMainWindow(resY, resX);
	Line *bufferHead = createBufferHead();
	Line *currentLine = bufferHead;	
	// Open file from command line args	
	if (argc == 2) {
		currentFile = openFileFromName(argv[1], &currentLine, bufferHead, win, bar);
		if (currentFile != NULL) {
			printBuffer(&currentLine, bufferHead, win, &leftOffset, resY, realX, x, y);
		}
	}

	// Main loop
	while (1) {
		// Get key pressed and cursor position
		ch = wgetch(win);
		getyx(win, y, x);
		// Exit program (^C)
		if (ch == ctrl('c')) {
			endwin();
			exit(0);
		}
		// Open file and free old filename
		else if (ch == ctrl('o')) {
			char *tempCurrentFile = currentFile;
			currentFile = openFile(&currentLine, bufferHead, win, bar);
			if (currentFile != NULL) {
				free(tempCurrentFile);
				printBuffer(&currentLine, bufferHead, win, &leftOffset, resY, realX, x, y);
			}
		} 
		// Save file
		else if (ch == ctrl('s')) {
			saveFile(currentFile, bufferHead, win, bar);
		}
		// Print out full file
		else if (ch == ctrl('p')) {
			printBuffer(&currentLine, bufferHead, win, &leftOffset, resY, realX, x, y);
		}
		// Arrow keys
		else if (ch == KEY_UP) {
			moveUp(&currentLine, win, &realX, lastXPos, x, y);
		}
		else if (ch == KEY_DOWN) {
			moveDown(&currentLine, win, &realX, lastXPos, x, y);
		}
		else if (ch == KEY_RIGHT) {
			moveRight(&currentLine, win, &realX, &lastXPos, x, y);
		}
		else if (ch == KEY_LEFT) {
			moveLeft(&currentLine, win, &realX, &lastXPos, x, y);
		}
		// Backspace
		else if (ch == KEY_BACKSPACE) {
			backspace(&currentLine, win, leftOffset, &realX, x, y);
		}
		// New line (enter)
		else if (ch == KEY_ENTER) {
			createNewLine(&currentLine, bufferHead, win, leftOffset, &realX, &lastXPos, y, resY);
		}
		// Regular characters
		else if (ch > 32 && ch < 127 || ch == 9) {
			insertCharacter(currentLine, win, ch, &realX, &lastXPos, x, y);
		}
		wrefresh(win);
	}
	
	// Close program (unused)
	endwin();
	exit(0);
}
