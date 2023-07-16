#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

struct Line;

void freeAllLines(Line *bufferHead) {
	bufferHead->length = 2;
	free(bufferHead->data);
	bufferHead->data = malloc(bufferHead->length);
	memset(bufferHead->data, '\0', bufferHead->length);
	Line *currentLine = bufferHead->next;
	Line *nextLine;	
	while (currentLine != NULL) {
		nextLine = currentLine->next;	
		free(currentLine->data);
		free(currentLine);
		currentLine = nextLine;
	}
	bufferHead->next = NULL;
	bufferHead->prev = NULL;
}

void resizeLine(Line *currentLine) {
	if (strlen(currentLine->data) >= currentLine->length) {
		currentLine->length = currentLine->length * 2;
		currentLine->data = realloc(currentLine->data, currentLine->length);
		memset(currentLine->data+currentLine->length/2, '\0', currentLine->length/2);
	}
	else if (strlen(currentLine->data) <= currentLine->length/2 && currentLine->length/2 > 1) {
		currentLine->length = currentLine->length/2;
		currentLine->data = realloc(currentLine->data, currentLine->length);
	}
}

void insertLineAfter(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->length = 2;
	newLine->data = malloc(newLine->length);
	memset(newLine->data, '\0', newLine->length);
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
	memset(newLine->data, '\0', newLine->length);
	if (currentLine->prev != NULL) {
		newLine->prev = currentLine->prev;
	}
	else {
		newLine->prev = NULL;
	}
	newLine->next = currentLine;
	currentLine->prev = newLine;
}


