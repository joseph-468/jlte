#include <stdlib.h>
#include "../include/line.h"

struct Line;

void insertLineAfter(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->data = malloc(100);
	if (currentLine->next != NULL) {
		newLine->next = currentLine->next;
	}
	else {
		newLine->next = NULL;
	}
	newLine->prev = currentLine;
	currentLine->next = newLine;
}

void insertLineBefore(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->data = malloc(100);
	if (currentLine->prev != NULL) {
		newLine->prev = currentLine->prev;
	}
	else {
		newLine->prev = NULL;
	}
	newLine->next = currentLine;
	currentLine->prev = newLine;
}

