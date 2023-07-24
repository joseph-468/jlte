#include <curses.h>
#include <string.h>
#include "../include/line.h"

void printFullBuffer(Line **currentLine, Line *bufferHead, WINDOW *win, int resY, int realX, int x, int y) {
	Line *currentPrintLine = bufferHead;
	int counter = 0;
	int xCounter = 0;
	while (currentPrintLine != NULL && counter < resY-1) {
		for (int i = 0; i < strlen(currentPrintLine->data); i++) {
			if (currentPrintLine->data[i] == 9) {
				int xPos = xCounter;
				for (int j = 0; j < TABSIZE-xPos%TABSIZE; j++) {
					wprintw(win, " ");
					xCounter += 1;
				}
			}
			else {
				wprintw(win, "%c", currentPrintLine->data[i]);
				xCounter += 1;
			}
		}
		wprintw(win, "\n");
		currentPrintLine = currentPrintLine->next;
		counter++;
		xCounter = 0;
	}
	*currentLine = bufferHead;
	wmove(win, 0, 0);
	realX = 0;
	getyx(win, y, x);
}
