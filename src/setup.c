#include <curses.h>
#include <stdlib.h>
#include <string.h>
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
	init_color(17, 888, 407, 915);
	init_color(18, 919, 903, 641);
	init_color(19, 641, 653, 915);
	init_color(20, 457, 145, 536);
	init_pair(1, 17, 18);
	init_pair(2, 20, 19);
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
