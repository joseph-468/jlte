#include <stdlib.h>
#include <curses.h>

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
	// Start ncurses
	initscr();
	cbreak();
	noecho();
	if (!has_colors) {
		printw("Fuck off lad. No one even wants you here");
		return -1;
	}
	start_color();
	// Create window
	int y, x;
	getmaxyx(stdscr, y, x);
	WINDOW *win = newwin(y, x, 0, 0); // Y, X, Y, X
	refresh();
	box(win, 0, 0);	
	wrefresh(win);
	// Startup variables
	int ch = '\0'; 
	Line fileHead = {malloc(8), NULL, NULL};
	Line *currentLine;
	currentLine = &fileHead;	

	// Middle stuff
	keypad(win, true);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	while (1) {
		ch = wgetch(win);
		getyx(win, y, x);
		mvwprintw(win, y, x, "%c", ch);
		wrefresh(win);
	}

	// Important end stuff
	getch();
	wattroff(win, COLOR_PAIR(1));
	endwin();
	return 0;
}
