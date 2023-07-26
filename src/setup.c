#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

void startNcurses() {
	initscr();
	noecho();
	nocbreak();
	raw();
	// Program runs in black and white if it can't change colors
	if (has_colors() && can_change_color()) {
		start_color();
	}
}

void setupColors() {
	init_color(COLOR_BLACK, 145, 149, 153);
	init_color(COLOR_BLACK+1, 828, 831, 851);
	init_color(COLOR_BLACK+2, 377, 341, 439);
	init_pair(1, COLOR_BLACK+1, COLOR_BLACK);
	init_pair(2, COLOR_BLACK+1, COLOR_BLACK+2);
}

WINDOW * setupMainWindow(int y, int x) {
	WINDOW *win = newwin(y-1, x, 0, 0);
	keypad(win, TRUE);
	wbkgd(win, COLOR_PAIR(1));
	wrefresh(win);
	return win;
}

WINDOW * setupBar(int y, int x) {
	WINDOW *bar = newwin(1, x, y-1, 0);
	keypad(bar, TRUE);
	wbkgd(bar, COLOR_PAIR(2));
	wrefresh(bar);
	return bar;
}

Line *createBufferHead() {
	Line *bufferHead = malloc(sizeof(Line));
	bufferHead->length = 2;
	bufferHead->data = malloc(bufferHead->length);
	memset(bufferHead->data, '\0', bufferHead->length);
	bufferHead->next = NULL;
	bufferHead->prev = NULL;
	return bufferHead;
}
