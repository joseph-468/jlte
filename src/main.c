#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "../include/setup.h"
#include "../include/line.h"
#include "../include/fileop.h"

#define ctrl(x) ((x) & 0x1f)

int main(int argc, char *argv[]) {
	// Start ncurses and create important variables
	if (startNcurses() != 0) {
		endwin();
		return -1;
	}
	char *currentFile = NULL;
	int y, x;
	int resY, resX;
	int lastXPos;
	int ch; 
	getmaxyx(stdscr, resY, resX);
	setupColors();
	WINDOW *bar = setupBar(resY, resX);
	WINDOW *win = setupMainWindow(resY, resX);
	Line *bufferHead = createBufferHead();
	Line *currentLine = bufferHead;
	
	// Open file from command line args	
	if (argc == 2) {
		currentFile = openFileFromName(argv[1], &currentLine, bufferHead, win, bar);
	}
	
	// Main loop
	while (1) {
		ch = wgetch(win);
		getyx(win, y, x);
		// Open file and free old filename
		if (ch == ctrl('o')) {
			char *tempCurrentFile = currentFile;
			currentFile = openFile(&currentLine, bufferHead, win, bar);
			if (currentFile != NULL) {
				free(tempCurrentFile);
			}
		} 
		else if (ch == ctrl('s')) {
			saveFile(currentFile, bufferHead, win, bar);
		}
		else if (ch == ctrl('c')) {
			endwin();
			return 0;
		}
		// Test line insertion
		else if (ch == ctrl('t')) {
			insertLineAfter(currentLine);
			currentLine = currentLine->next;
		}
		// Print out full file
		else if (ch == ctrl('p')) {
			Line *dog = bufferHead;
			int counter = 0;
			while (dog != NULL && counter < resY-1) {
				wprintw(win, "%s\n", dog->data);
				dog = dog->next;
				counter++;
			}
			currentLine = bufferHead;
			wmove(win, 0, 0);
			getyx(win, y, x);
		}
		// Print ascii value of character under cursor
		else if (ch == ctrl('d')) {
			wprintw(win, "%d", currentLine->data[x]);
		}
		// Arrow keys
		else if (ch == KEY_UP) {
			if (currentLine->prev != NULL) {
				currentLine = currentLine->prev;
				wmove(win, y-1, 0);
				getyx(win, y, x);
				while (currentLine->data[x] != '\0' && x != lastXPos) {
					wmove(win, y, x+1);
					getyx(win, y, x);
				}
			}
		}
		else if (ch == KEY_DOWN) {
			if (currentLine->next != NULL) {
				currentLine = currentLine->next;
				wmove(win, y+1, 0);
				getyx(win, y, x);
				while (currentLine->data[x] != '\0' && x != lastXPos) {
					wmove(win, y, x+1);
					getyx(win, y, x);
				}
			}
		}
		else if (ch == KEY_LEFT) {
			if (x > 0) {
				wmove(win, y, x-1);
			}
			lastXPos = x;
		}
		else if (ch == KEY_RIGHT) {
			if (currentLine->data[x] != '\0') {
				wmove(win, y, x+1);
			}
			lastXPos = x;
		}
		// Backspace
		else if (ch == KEY_BACKSPACE) {
			// Order between resizing and inserting data matters
			currentLine->data[x-1] = '\0';
			resizeLine(currentLine);
			mvwdelch(win, y, x-1);
		}
		// Creates new line (Enter key)
		else if (ch == 10) {
			// Copy text that will go on new line and remove from current line
			int newLineSize = strlen(currentLine->data+x);
			char *newLineText = malloc(newLineSize+1);
			if (newLineText != NULL) {
				if (newLineSize > 0) {
					strcpy(newLineText, currentLine->data+x);
					memset(currentLine->data+x, '\0', newLineSize);
					setSize(currentLine, strlen(currentLine->data));
				}
				// Put new line text in new line
				insertLineAfter(currentLine);
				currentLine = currentLine->next;
				if (newLineSize > 0) {
					setSize(currentLine, newLineSize);
					strcpy(currentLine->data, newLineText);
				}
				free(newLineText);
				// Print new buffer
				Line *tempCurrentLine = currentLine;
				while (tempCurrentLine != NULL) {
					wprintw(win, "\n%s", tempCurrentLine->data);
					tempCurrentLine = tempCurrentLine->next;
				}
				wmove(win, y+1, 0);
			}
		}
		// Regular characters
		else if (ch >= 32 && ch <= 256) {
			lastXPos = x+1;
			// Order between resizing and inserting data matters
			resizeLine(currentLine);
			if (currentLine->data[x] != '\0') {
				int arrayEnd = strlen(currentLine->data)-1;
				for (int i = arrayEnd; i >= x; i--) {
					currentLine->data[i+1] = currentLine->data[i];
				}
				currentLine->data[x] = ch;
				mvwprintw(win, y, 0, "%s", currentLine->data);
				wmove(win, y, x+1);
			}
			else {
				currentLine->data[x] = ch;
				mvwprintw(win, y, x, "%c", ch);
			}
		}
		wrefresh(win);
	}
	
	// Important end stuff
	getch();
	endwin();
	return 0;
}
