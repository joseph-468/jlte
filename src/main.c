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
	int ch; 
	getmaxyx(stdscr, y, x);
	setupColors();
	WINDOW *bar = setupBar(y, x);
	WINDOW *win = setupMainWindow(y, x);
	Line *bufferHead = createBufferHead();
	Line *currentLine = bufferHead;

	// Main loop
	while (1) {
		ch = wgetch(win);
		getyx(win, y, x);
		if (ch == ctrl('o')) {
			char *tempCurrentFile = currentFile;
			currentFile = openFile(&currentLine, bufferHead, win, bar);
			if (currentFile != NULL) {
				free(tempCurrentFile);
			}
		} 
		else if (ch == ctrl('s')) {
			saveFile(currentFile, bufferHead, win, bar);
			mvwprintw(win, y+1, x, "%s", currentLine->data);
			mvwprintw(win, y+2, x, "%d", currentLine->length);
			wmove(win, y, x);
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
			while (dog != NULL) {
				wprintw(win, "%s", dog->data);
				dog = dog->next;
			}
		}
		else if (ch == KEY_BACKSPACE) {
			// Order between resizing and inserting data matters
			currentLine->data[x-1] = '\0';
			resizeLine(currentLine);
			mvwdelch(win, y, x-1);
		}
		// Regular characters
		else if (ch >= 32 && ch <= 256) {
			// Order between resizing and inserting data matters
			resizeLine(currentLine);
			currentLine->data[x] = ch;
			mvwprintw(win, y, x, "%c", ch);
		}
		wrefresh(win);
	}
	
	// Important end stuff
	getch();
	endwin();
	return 0;
}
