#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "../include/setup.h"
#include "../include/line.h"
#include "../include/fileop.h"
#include "../include/bufferop.h"

#define ctrl(x) ((x) & 0x1f)

int main(int argc, char *argv[]) {
	// Start ncurses and create important variables
	if (startNcurses() != 0) {
		endwin();
		return -1;
	}
	char *currentFile = NULL;
	int resY, resX;
	int y, x;
	int lastXPos;
	int realX = 0;
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
		if (currentFile != NULL) {
			printFullBuffer(&currentLine, bufferHead, win, resY, realX, x, y);
		}
	}
	
	// Main loop
	while (1) {
		// Get key pressed and cursor position
		ch = wgetch(win);
		getyx(win, y, x);

		// Exit program
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
				printFullBuffer(&currentLine, bufferHead, win, resY, realX, x, y);
			}
		} 
		// Save file
		else if (ch == ctrl('s')) {
			saveFile(currentFile, bufferHead, win, bar);
		}
		// Print out full file
		else if (ch == ctrl('p')) {
			printFullBuffer(&currentLine, bufferHead, win, resY, realX, x, y);
		}
		// Arrow keys
		else if (ch == KEY_UP) {
			if (currentLine->prev != NULL) {
				currentLine = currentLine->prev;
				wmove(win, y-1, 0);
				realX = 0;
				getyx(win, y, x);
				while (currentLine->data[realX] != '\0' && x != lastXPos) {
					if (currentLine->data[realX] == 9) {
						if (x+(TABSIZE-x%TABSIZE) > lastXPos) {
							break;
						}
						else {
							wmove(win, y, x+(TABSIZE-x%TABSIZE));
						}
					}
					else {
						wmove(win, y, x+1);
					}
					realX += 1;
					getyx(win, y, x);
				}
			}
		}
		else if (ch == KEY_DOWN) {
			if (currentLine->next != NULL) {
				currentLine = currentLine->next;
				wmove(win, y+1, 0);
				realX = 0;
				getyx(win, y, x);
				while (currentLine->data[realX] != '\0' && x != lastXPos) {
					if (currentLine->data[realX] == 9) {
						if (x+(TABSIZE-x%TABSIZE) > lastXPos) {
							break;
						}
						else {
							wmove(win, y, x+(TABSIZE-x%TABSIZE));
						}
					}
					else {
						wmove(win, y, x+1);
					}
					realX += 1;
					getyx(win, y, x);
				}
			}
		}
		else if (ch == KEY_LEFT) {
			if (x > 0) {
				if (currentLine->data[realX-1] == 9) {
					int position = 0;
					int latestTab = 0;
					for (int i = 0; position < x; i++) {
						if (currentLine->data[i] == '\0') {
							break;
						}
						if (currentLine->data[i] == 9) {
							latestTab = TABSIZE-position%TABSIZE;
							position += TABSIZE-position%TABSIZE;
						}
						else {
							position += 1;
						}
					}
					wmove(win, y, position-latestTab);
					getyx(win, y, x);
					realX -= 1;
				}
				else {
					wmove(win, y, x-1);
					realX -= 1;
				}
			}
			getyx(win, y, x);
			lastXPos = x;
		}
		else if (ch == KEY_RIGHT) {
			if (currentLine->data[realX] != '\0') {
				if (currentLine->data[realX] == 9) {
					wmove(win, y, x+(TABSIZE-x%TABSIZE));
					realX += 1;
				} 
				else {
					wmove(win, y, x+1);
					realX += 1;
				}
			}
			getyx(win, y, x);
			lastXPos = x;
		}
		// Backspace
		else if (ch == KEY_BACKSPACE) {
			// Order between resizing and inserting data matters
			currentLine->data[realX-1] = '\0';
			resizeLine(currentLine);
			mvwdelch(win, y, x-1);
		}
		// Enter (new line)
		else if (ch == 10) {
			// Copy text that will go on new line and remove from current line
			int newLineSize = strlen(currentLine->data+realX);
			char *newLineText = malloc(newLineSize+1);
			if (newLineText != NULL) {
				if (newLineSize > 0) {
					strcpy(newLineText, currentLine->data+realX);
					memset(currentLine->data+realX, '\0', newLineSize);
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
				realX = 0;
			}
		}
		// Regular characters
		else if (ch >= 32 && ch <= 256) {
			lastXPos = x+1;
			// Order between resizing and inserting data matters
			resizeLine(currentLine);
			if (currentLine->data[realX] != '\0') {
				int arrayEnd = strlen(currentLine->data)-1;
				for (int i = arrayEnd; i >= realX; i--) {
					currentLine->data[i+1] = currentLine->data[i];
				}
				currentLine->data[realX] = ch;
				mvwprintw(win, y, 0, "%s", currentLine->data);
				wmove(win, y, x+1);
				realX += 1;
			}
			else {
				currentLine->data[realX] = ch;
				mvwprintw(win, y, x, "%c", ch);
				realX += 1;
			}
		}
		wrefresh(win);
	}
	
	// Close program (unused)
	endwin();
	exit(0);
}
