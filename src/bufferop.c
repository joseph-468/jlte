#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "../include/line.h"


void backspace(Line **currentLine, WINDOW *win, int *realX, int x, int y) {
	if (strlen((*currentLine)->data) > 0 && *realX > 0) {
		int index = *realX-1;
		short isTab = ((*currentLine)->data[index] == 9) ? 1 : 0;
		while ((*currentLine)->data[index] != '\0') {
			(*currentLine)->data[index] = (*currentLine)->data[index+1];
			index++;
		}
		resizeLine((*currentLine));	
		if (isTab == 1) {
			wmove(win, y, 0);
			wclrtoeol(win);
			for (int i = 0; i < strlen((*currentLine)->data); i++) {
				if ((*currentLine)->data[i] == 9) {
					for (int j = 0; j < TABSIZE-x%TABSIZE; j++) {
						wprintw(win, " ");
					}
				}
				else {
					wprintw(win, "%c", (*currentLine)->data[i]);
				}
			}
			wmove(win, y, x-TABSIZE-x%TABSIZE);
			*realX = *realX-1;
		}
		else {
			mvwdelch(win, y, x-1);
			*realX = *realX-1;
		}
	}
	else if (*realX == 0 && (*currentLine)->prev != NULL) {
		int afterPosX;
		int afterPosY;
		Line *previousLine = (*currentLine)->prev;
		if (strlen((*currentLine)->data) > 0) {
			char *currentLineData = malloc(strlen((*currentLine)->data)+1);
			strcpy(currentLineData, (*currentLine)->data);
			removeLine((*currentLine));
			*currentLine = previousLine;	
			afterPosX = strlen((*currentLine)->data);
			setSize((*currentLine), strlen((*currentLine)->data)+strlen(currentLineData));
			strcat((*currentLine)->data, currentLineData);
			free(currentLineData);
		}
		else {
			removeLine((*currentLine));
			*currentLine = previousLine;	
			afterPosX = strlen((*currentLine)->data);
		}
		// Print new buffer
		afterPosY = y-1;
		wmove(win, afterPosY, x);
		Line *tempCurrentLine = (*currentLine);
		while (tempCurrentLine != NULL) {
			wprintw(win, "%s\n", tempCurrentLine->data);
			tempCurrentLine = tempCurrentLine->next;
		}
		getyx(win, y, x);
		wmove(win, y, 0);
		wclrtoeol(win);
		wmove(win, afterPosY, afterPosX);
		*realX = afterPosX;
	}
}

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

void createNewLine(Line **currentLine, WINDOW *win, int *realX, int *lastXPos, int y) {
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
		*lastXPos = 0;
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
		if (ch == 9) wmove(win, y, x+TABSIZE-x%TABSIZE);
		else wmove(win, y, x+1);
		*realX += 1;
	}
	else {
		currentLine->data[*realX] = ch;
		mvwprintw(win, y, x, "%c", ch);
		*realX += 1;
	}
}
