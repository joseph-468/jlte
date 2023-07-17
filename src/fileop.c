#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

#define ASCII_BACKSPACE 127
#define ASCII_ENTER 10

char * openFile(Line **currentLine, Line *bufferHead, WINDOW *win, WINDOW *bar) {
	wclear(bar);
	mvwprintw(bar, 0, 0, "Open file: ");
	int y, x;
	int startX, startY;
	int i = 0;
	char fileName[100] = {'\0'};
	int ch;
	getyx(bar, startY, startX);
	wrefresh(bar);
	while (1) {
		ch = wgetch(bar);
		getyx(bar, y, x);
		if (ch == ASCII_ENTER) {
			FILE *fptr = fopen(fileName, "r");
			if (fptr == NULL) {
				fptr = fopen(fileName, "w");
				if (fptr == NULL) {
					wclear(bar);
					mvwprintw(bar, 0, 0, "Error opening file");
					wrefresh(bar);
					return NULL;
				}
				freeAllLines(bufferHead);
				char *fileNamePointer = malloc(strlen(fileName));
				strcpy(fileNamePointer, fileName);
				wclear(bar);
				wclear(win);
				mvwprintw(bar, 0, 0, "File created");
				wrefresh(bar);	
				return fileNamePointer; // Success
			}
			else {
				freeAllLines(bufferHead);
				char *fileNamePointer = malloc(strlen(fileName));
				Line *currentFileLine = bufferHead;
				short hasData = 0;
				strcpy(fileNamePointer, fileName);
				wclear(bar);
				wclear(win);
				mvwprintw(bar, 0, 0, "File opened");
				// Open file in lines
				char string[1000];
				while(fgets(string, 1000, fptr)) {
					string[strlen(string)-1] = '\0'; // Get rid of linebreak
					strcpy(currentFileLine->data, string);
					insertLineAfter(currentFileLine);
					currentFileLine = currentFileLine->next;
					hasData = 1;
				}
				if (hasData == 1) {	
					*currentLine = currentFileLine;
				}
				else {
					*currentLine = bufferHead;					
				}
				// Exit function
				wrefresh(bar);
				if (fclose(fptr) != 0) {
					wclear(bar);
					mvwprintw(bar, 0, 0, "Error closing file");
					wrefresh(bar);
					return NULL;
				}
				return fileNamePointer; // Sucess
			}
		}
		else if (ch == ASCII_BACKSPACE) {
			if (startX < x) { 
				i -= 1;
				fileName[i] = '\0';
				mvwdelch(bar, y, x-1);
			}
		}
		else if (ch >= 32 && ch <= 256) {
			fileName[i] = ch;
			i += 1;
			mvwprintw(bar, y, x, "%c", ch);
		}
		wrefresh(bar);
	}
}

void saveFile(char *currentFile, Line *bufferHead, WINDOW *win, WINDOW *bar) {
	if (currentFile == NULL) {
		wclear(bar);
		mvwprintw(bar, 0, 0, "No file is currently open");
		wrefresh(bar);
		return;
	}
	FILE *fptr = fopen(currentFile, "w");
	if (fptr == NULL) {
		wclear(bar);
		mvwprintw(bar, 0, 0, "Error opening file");
		wrefresh(bar);
		return;
	}
	Line *linePointer = bufferHead;
	while (linePointer != NULL) {
		if (strlen(linePointer->data) > 0) {
			if (fprintf(fptr, "%s\n", linePointer->data) < 0) {
				wclear(bar);
				mvwprintw(bar, 0, 0, "Error writing to file");
				wrefresh(bar);
				fclose(fptr);
				return;	
			}
		}
		linePointer = linePointer->next;
	}
	if (fclose(fptr) != 0) {
		wclear(bar);
		mvwprintw(bar, 0, 0, "Error closing file");
		wrefresh(bar);
		return;
	}
	wclear(bar);
	mvwprintw(bar, 0, 0, "File successfully saved");
	wrefresh(bar);
}

