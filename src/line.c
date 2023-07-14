#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

struct Line;

void resizeLine(Line *currentLine) {
	if (currentLine->length <= strlen(currentLine->data)) {
		currentLine->length = currentLine->length * 2;
		currentLine->data = realloc(currentLine->data, currentLine->length);
	}
}

void insertLineAfter(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->length = 2;
	newLine->data = malloc(newLine->length);
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
	newLine->length = 2;
	newLine->data = malloc(newLine->length);
	if (currentLine->prev != NULL) {
		newLine->prev = currentLine->prev;
	}
	else {
		newLine->prev = NULL;
	}
	newLine->next = currentLine;
	currentLine->prev = newLine;
}


