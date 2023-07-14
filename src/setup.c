#include <curses.h>
#include <stdlib.h>
#include "../include/line.h"

int startNcurses() {
	initscr();
	cbreak();
	noecho();
	raw();
	// Close program if it doesn't support enough colors
	if (!has_colors || !can_change_color()) {
		return -1;
	}
	start_color();
	return 0;
}

void setupColors() {
	init_color(17, 289, 211, 735);
	init_pair(2, 17, 17);
	init_pair(3, COLOR_CYAN, 17);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
}

WINDOW * setupMainWindow(int y, int x) {
	WINDOW *win = newwin(y-1, x, 0, 0);
	keypad(win, TRUE);
	box(win, 0, 0);	
	wattron(win, COLOR_PAIR(1));
	wrefresh(win);
	return win;
}

WINDOW * setupBar(int y, int x) {
	WINDOW *bar = newwin(1, x, y-1, 0);
	keypad(bar, TRUE);
	wbkgd(bar, COLOR_PAIR(3));
	wrefresh(bar);
	return bar;
}

Line *createBufferHead() {
	Line *bufferHead = malloc(sizeof(Line));
	bufferHead->length = 2;
	bufferHead->data = malloc(bufferHead->length);
	bufferHead->next = NULL;
	bufferHead->prev = NULL;
	return bufferHead;
}