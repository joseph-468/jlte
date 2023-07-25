#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "../include/setup.h"
#include "../include/line.h"
#include "../include/fileop.h"
#include "../include/bufferop.h"

#define ctrl(x) ((x) & 0x1f)

int main(int argc, char *argv[]) {
	startNcurses();
	// Initialize important variables
	char *currentFile = NULL;
	int resY = 0, resX = 0;
	int y = 0, x = 0;
	int lastXPos = 0;
	int realX = 0;
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
			printBuffer(&currentLine, bufferHead, win, resY, realX, x, y);
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
				printBuffer(&currentLine, bufferHead, win, resY, realX, x, y);
			}
		} 
		// Save file
		else if (ch == ctrl('s')) {
			saveFile(currentFile, bufferHead, win, bar);
		}
		// Print out full file
		else if (ch == ctrl('p')) {
			printBuffer(&currentLine, bufferHead, win, resY, realX, x, y);
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
			else if (strlen(currentLine->data) && currentLine->prev != NULL) {
				currentLine = currentLine->prev;
				wmove(win, y-1, strlen(currentLine->data));
				realX = strlen(currentLine->data);
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
			else {
				if (currentLine->next != NULL) {
					currentLine = currentLine->next;
					wmove(win, y+1, 0);
					realX = 0;
				}
			}
			getyx(win, y, x);
			lastXPos = x;
		}
		// Backspace
		else if (ch == KEY_BACKSPACE) {
			backspace(&currentLine, win, &realX, x, y);
		}
		// Enter (new line)
		else if (ch == 10) {
			createNewLine(&currentLine, win, &realX, &lastXPos, y);
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
