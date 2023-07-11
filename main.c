#include <stdlib.h>
#include <curses.h>
#include <string.h>

#define ctrl(x) ((x) & 0x1f)
#define ASCII_BACKSPACE 127
#define ASCII_ENTER 10

typedef struct Line {
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void insertLine(Line *currentLine);
char * openFile(Line *currentLine, WINDOW *win, WINDOW *bar);
void saveFile(char *currentFile, Line *fileHead, WINDOW *win, WINDOW *bar);

void insertLine(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->data = malloc(100);
	newLine->next = NULL;
	newLine->prev = currentLine;
	currentLine->next = newLine;
}

char * openFile(Line *currentLine, WINDOW *win, WINDOW *bar) {
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
		if (ch == ASCII_ENTER) {
			mvwprintw(win, 0, 8, "%s", fileName);
			wrefresh(win);
			FILE *fptr = fopen(fileName, "r");
			if (fptr == NULL) {
				wclear(bar);
				mvwprintw(bar, 0, 0, "Error opening file");
				wrefresh(bar);
				return NULL;
			}
			else {
				char *fileNamePointer = malloc(strlen(fileName));
				strcpy(fileNamePointer, fileName);
				char lineTest[100];
				wclear(bar);
				mvwprintw(bar, 0, 0, "File opened");
				// Append to list
				char string[100];
				while(fgets(string, 100, fptr)) {
					insertLine(currentLine);
					strcpy(currentLine->data, string);
					currentLine = currentLine->next;
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
		else {
			fileName[i] = ch;
			i += 1;
			mvwprintw(bar, y, x, "%c", ch);
		}
		wrefresh(bar);
	}
}

void saveFile(char *currentFile, Line *fileHead, WINDOW *win, WINDOW *bar) {
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
	Line *linePointer = fileHead;
	while (linePointer->next != NULL) {
		if (fprintf(fptr, "%s", linePointer->data) < 0) {
			wclear(bar);
			mvwprintw(bar, 0, 0, "Error writing to file");
			wrefresh(bar);
			fclose(fptr);
			return;	
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

int main(int argc, char *argv[]) {
	// Start ncurses
	initscr();
	cbreak();
	noecho();
	raw();
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
	char *currentFile = NULL;
	int ch; 
	Line *fileHead = malloc(sizeof(Line));
	fileHead->next = NULL;
	fileHead->prev = NULL;
	fileHead->data = malloc(100);
	Line *currentLine;
	currentLine = fileHead;	
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
			currentFile = openFile(fileHead, win, bar);
		} 
		else if (ch == ctrl('s')) {
			saveFile(currentFile, fileHead, win, bar);
		}
		else if (ch == ctrl('c')) {
			endwin();
			return 0;
		}
		else if (ch == ctrl('p')) {
			// Print out full file
			Line *dog = fileHead;
			while (dog->next != NULL) {
				wprintw(win, "\n%s", dog->data);
				dog = dog->next;
			}
		}
		else {
			mvwprintw(win, y, x, "%c", ch);
		}
		wrefresh(win);
	}
	
	// Important end stuff
	getch();
	endwin();
	return 0;
}
