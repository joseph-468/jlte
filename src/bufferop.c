#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

void printBuffer(Line **currentLine, Line *bufferHead, WINDOW *win, int resY, int realX, int x, int y) {
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

void createNewLine(Line **currentLine, WINDOW *win, int *realX, int y) {
	// Copy text that will go on new line and remove from current line
	int newLineSize = strlen((*currentLine)->data+*realX);
	char *newLineText = malloc(newLineSize+1);
	if (newLineText != NULL) {
		if (newLineSize > 0) {
			strcpy(newLineText, (*currentLine)->data+*realX);
			memset((*currentLine)->data+*realX, '\0', newLineSize);
			setSize((*currentLine), strlen((*currentLine)->data));
		}
		// Put new line text in new line
		insertLineAfter(*currentLine);
		*currentLine = (*currentLine)->next;
		if (newLineSize > 0) {
			setSize((*currentLine), newLineSize);
			strcpy((*currentLine)->data, newLineText);
		}
		free(newLineText);
		// Print new buffer
		Line *tempCurrentLine = (*currentLine);
		while (tempCurrentLine != NULL) {
			wprintw(win, "\n%s", tempCurrentLine->data);
			tempCurrentLine = tempCurrentLine->next;
		}
		wmove(win, y+1, 0);
		*realX = 0;
	}
}

void insertCharacter(Line *currentLine, WINDOW *win, int ch, int *realX, int *lastXPos, int x, int y) {
	*lastXPos = x+1;
	// Order between resizing and inserting data matters
	resizeLine(currentLine);
	if (currentLine->data[*realX] != '\0') {
		int arrayEnd = strlen(currentLine->data)-1;
		for (int i = arrayEnd; i >= *realX; i--) {
			currentLine->data[i+1] = currentLine->data[i];
		}
		currentLine->data[*realX] = ch;
		mvwprintw(win, y, 0, "%s", currentLine->data);
		wmove(win, y, x+1);
		*realX += 1;
	}
	else {
		currentLine->data[*realX] = ch;
		mvwprintw(win, y, x, "%c", ch);
		*realX += 1;
	}
}
