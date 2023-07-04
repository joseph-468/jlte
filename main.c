#include <stdlib.h>
#include <curses.h>
#include "gbuffer.h"

typedef struct Line {
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void insertLine(Line *currentLine);

void insertLine(Line *currentLine) {
	Line newLine;
	newLine.data = malloc(8);
	newLine.next = NULL;
	newLine.prev = currentLine;
	currentLine->next = &newLine;
}

int main(int argc, char *argv[]) {
	// Important startup
	initscr();
	if (!has_colors) {
		printw("Fuck off lad. No one even wants you here");
		return -1;
	}
	start_color();
	cbreak();
	noecho();
	WINDOW *win = newwin(8, 16, 0, 0); // Y, X, Y, X
	char ch = '\0'; 
	int y, x;
	Line fileHead = {malloc(8), NULL, NULL};
	Line *currentLine;
	currentLine = &fileHead;	
	refresh();
	box(win, 0, 0);	
	wrefresh(win);
	
	// Gap buffer
	char *dog = malloc(64);
	GapBuffer gap = {dog, 8, 0, 10};
	gap.data[0] = 'f';
	gap.data[3] = 'u';
	gap.data[30] = 'c';
	gap.data[63] = 'k';
	char *frog = printBuffer(&gap);
	wprintw(win, "%s\n", frog);
	grow(&gap, 5);
	gap.data[72] = 'u';
	frog = printBuffer(&gap);
	wprintw(win, "%s", frog);
	getch();
	free(frog);
	getch();
	

	// Middle stuff
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	while (1) {
		ch = getch();
		getyx(win, y, x);
		//mvwprintw(win, y, x, "%c", ch);
		wrefresh(win);
	}

	// Important end stuff
	getch();
	wattroff(win, COLOR_PAIR(1));
	endwin();
	return 0;
}
