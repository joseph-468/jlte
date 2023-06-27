#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Line {
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void getTerminalSize(int *tWidth, int *tHeight);
void insertLine(Line *currentLine);

void getTerminalSize(int *tWidth, int *tHeight) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*tWidth = size.ws_col;
	*tHeight = size.ws_row;
}

void insertLine(Line *currentLine) {
	Line newLine;
	newLine.data = malloc(8);
	newLine.next = NULL;
	newLine.prev = currentLine;
	currentLine->next = &newLine;
}

int main(int argc, char *argv[]) {
	// Important variables
	int tWidth, tHeight;
	Line fileHead = {malloc(8), NULL, NULL};
	Line *currentLine;
	currentLine = &fileHead;

	return 0;
}
