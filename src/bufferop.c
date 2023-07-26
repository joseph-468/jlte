#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "../include/line.h"
#include "../include/bufferop.h"

void moveUp(Line **currentLine, WINDOW *win, int *realX, int lastXPos, int x, int y) {
	if ((*currentLine)->prev != NULL) {
		*currentLine = (*currentLine)->prev;
		wmove(win, y-1, 0);
		*realX = 0;
		getyx(win, y, x);
		while ((*currentLine)->data[*realX] != '\0' && x != lastXPos) {
			if ((*currentLine)->data[*realX] == 9) {
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
			*realX += 1;
			getyx(win, y, x);
		}
	}
}

void moveDown(Line **currentLine, WINDOW *win, int *realX, int lastXPos, int x, int y) {
	if ((*currentLine)->next != NULL) {
		*currentLine = (*currentLine)->next;
		wmove(win, y+1, 0);
		*realX = 0;
		getyx(win, y, x);
		while ((*currentLine)->data[*realX] != '\0' && x != lastXPos) {
			if ((*currentLine)->data[*realX] == 9) {
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
			*realX += 1;
			getyx(win, y, x);
		}
	}
}

void moveRight(Line **currentLine, WINDOW *win, int *realX, int *lastXPos, int x, int y) {
	if ((*currentLine)->data[*realX] != '\0') {
		if ((*currentLine)->data[*realX] == 9) {
			wmove(win, y, x+(TABSIZE-x%TABSIZE));
			*realX += 1;
		} 
		else {
			wmove(win, y, x+1);
			*realX += 1;
		}
	}
	else {
		if ((*currentLine)->next != NULL) {
			*currentLine = (*currentLine)->next;
			wmove(win, y+1, 0);
			*realX = 0;
		}
	}
	getyx(win, y, x);
	*lastXPos = x;
}

void moveLeft(Line **currentLine, WINDOW *win, int *realX, int *lastXPos, int x, int y) {
	if (x > 0) {
		if ((*currentLine)->data[*realX-1] == 9) {
			int position = 0;
			int latestTab = 0;
			for (int i = 0; position < x; i++) {
				if ((*currentLine)->data[i] == '\0') {
					break;
				}
				if ((*currentLine)->data[i] == 9) {
					latestTab = TABSIZE-position%TABSIZE;
					position += TABSIZE-position%TABSIZE;
				}
				else {
					position += 1;
				}
			}
			wmove(win, y, position-latestTab);
			getyx(win, y, x);
			*realX -= 1;
		}
		else {
			wmove(win, y, x-1);
			*realX -= 1;
		}
	}
	else if ((*currentLine)->prev != NULL) {
		*currentLine = (*currentLine)->prev;
		// Calculate line length with tabs
		int length = 0;
		for (int i = 0; i < strlen((*currentLine)->data); i++) {
			if ((*currentLine)->data[i] == 9) {
				length += TABSIZE;
			}
			else {
				length += 1;
			}
		}
		wmove(win, y-1, length);
		*realX = strlen((*currentLine)->data);
	}
	getyx(win, y, x);
	*lastXPos = x;
}

void backspace(Line **currentLine, WINDOW *win, int leftOffset, int *realX, int x, int y) {
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

void printBuffer(Line **currentLine, Line *bufferHead, WINDOW *win, int *leftOffset, int resY, int realX, int x, int y) {
	wclear(win);
	Line *currentPrintLine = bufferHead;
	int counter = 0;
	int xCounter = 0;
	printLineNumbers(bufferHead, win, leftOffset, 0, resY);
	wmove(win, 0, 2);
	while (currentPrintLine != NULL && counter < resY-1) {
		wmove(win, counter, *leftOffset);
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
	wmove(win, 0, *leftOffset);
	realX = 0;
	getyx(win, y, x);
}

void printLineNumbers(Line *bufferHead, WINDOW *win, int *leftOffset, int resX, int resY) {
	wattron(win, COLOR_PAIR(3));
	// Find length of buffer
	wmove(win, 0, 0);
	int lineCount = 0;
	int tempLineCount = 0;
	int maxPadding = 0;
	Line *currentLine = bufferHead;
	while (currentLine != NULL) {
		lineCount++;	
		currentLine = currentLine->next;
	}
	tempLineCount = lineCount;
	while(tempLineCount >= 10) {
		tempLineCount /= 10;
		maxPadding++;
	}
	*leftOffset = maxPadding+2;
	// Print line numbers
	int y, x;
	int tempCounter = 0;
	int padding = 0;
	char paddingString[10] = {'\0'};
	currentLine = bufferHead;
	for (int i = 0; i < resY-1; i++) {
		if (currentLine == NULL) {
			break;
		}
		padding = 0;
		tempCounter = i;
		memset(paddingString, '\0', 10);
		while(tempCounter >= 10) {
			tempCounter /= 10;
			padding++;
		}	
		for (int i = 0; i < maxPadding-padding; i++) {
			paddingString[i] = ' ';
		}
		getyx(win, y, x);
		wprintw(win, "%s%d ", paddingString, i);
		wmove(win, y+1, 0);
		currentLine = currentLine->next;
	}
	wattron(win, COLOR_PAIR(1));
}

void createNewLine(Line **currentLine, Line *bufferHead, WINDOW *win, int leftOffset, int *realX, int *lastXPos, int y, int resY) {
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
		char leftPadding[10] = {'\0'};
		Line *tempCurrentLine = (*currentLine);
		while (tempCurrentLine != NULL) {
			memset(leftPadding, '\0', 10);
			for (int i = 0; i < leftOffset; i++) {
				leftPadding[i] = ' ';
			}
			wprintw(win, "\n%s%s", leftPadding, tempCurrentLine->data); // Temporary, very inefficient
			tempCurrentLine = tempCurrentLine->next;
		}
		printLineNumbers(bufferHead, win, &leftOffset, 0, resY);
		wmove(win, y+1, leftOffset);
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

