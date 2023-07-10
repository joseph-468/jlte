#include <stdlib.h>
#include <curses.h>
#define ctrl(x) ((x) & 0x1f)

typedef struct Line {
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void insertLine(Line *currentLine);
int openFile();

void insertLine(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->data = malloc(8);
	newLine->next = NULL;
	newLine->prev = currentLine;
	currentLine->next = newLine;
}

int openFile(WINDOW *win, WINDOW *bar) {
	wclear(bar);
	mvwprintw(bar, 0, 0, "Open file: ");
	short y, x;
	short startX, startY;
	int i = 0;
	char fileName[100] = {'\0'};
	char ch;
	getyx(bar, startY, startX);
	wrefresh(bar);
	while (1) {
		ch = wgetch(win);
		getyx(bar, y, x);
		if (ch == 10) {
			mvwprintw(win, 0, 8, "%s", fileName);
			wrefresh(win);
			FILE *fptr = fopen(fileName, "r");
			if (fptr == NULL) {
				wclear(bar);
				mvwprintw(bar, 0, 0, "File not found");
				wrefresh(bar);
				return -1; // Failure
			}
			else {
				char lineTest[1000];
				wclear(bar);
				mvwprintw(bar, 0, 0, "File opened");
				fscanf(fptr, "%s", lineTest);
				mvwprintw(win, 2, 2, "%s", lineTest);
				wrefresh(bar);
				return 0; // Sucess
			}
		}
		else if (ch == 127) {
			if (startX < x) {
				i -= 1;
				fileName[i] = '\0';
				mvwdelch(bar, y, x-1);
			}
		}
		else {
			fileName[i] = ch;
			i += 1;
			mvwprintw(bar, y, x, "%c", ch);
		}
		wrefresh(bar);
	}
}

int main(int argc, char *argv[]) {
	// Start ncurses
	initscr();
	cbreak();
	noecho();
	if (!has_colors || !can_change_color()) {
		printw("Fuck off lad. No one even wants you here");
		endwin();
		return -1;
	}
	start_color();
	// Create window
	int y, x;
	getmaxyx(stdscr, y, x);
	WINDOW *win = newwin(y-1, x, 0, 0); // Y, X, Y, X
	box(win, 0, 0);	
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	wrefresh(win);
	// Startup variables
	int ch = '\0'; 
	Line fileHead = malloc(sizeof(Line));
	fileHead.next = NULL;
	fileHead.prev = NULL;
	fileHead.data = malloc(8);
	Line *currentLine;
	currentLine = &fileHead;	
	// Bottom Bar setup
	WINDOW *bar = newwin(1, x, y-1, 0);
	init_color(17, 289, 211, 735);
	init_pair(2, 17, 17);
	init_pair(3, COLOR_CYAN, 17);
	wbkgd(bar, COLOR_PAIR(2));
	wrefresh(bar);
	wbkgd(bar, COLOR_PAIR(3));

	// Main loop
	while (1) {
		ch = wgetch(win);
		getyx(win, y, x);
		if (ch == ctrl('o')) {
			openFile(win, bar);
		}
		mvwprintw(win, y, x, "%c", ch);
		wrefresh(win);
	}
	
	// Important end stuff
	getch();
	endwin();
	return 0;
}
