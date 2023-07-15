#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

struct Line;

void freeAllLines(Line *bufferHead) {
	bufferHead->length = 2;
	free(bufferHead->data);
	bufferHead->data = malloc(bufferHead->length);
	memset(bufferHead->data, '\0', bufferHead->length);
	bufferHead->data[0] = '\n';
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
	if (currentLine->length <= strlen(currentLine->data)) {
		currentLine->length = currentLine->length * 2;
		currentLine->data = realloc(currentLine->data, currentLine->length);
		currentLine->data[currentLine->length/2] = '\0';
	}
}

void insertLineAfter(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->length = 2;
	newLine->data = malloc(newLine->length);
	memset(newLine->data, '\0', newLine->length);
	newLine->data[0] = '\n';
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
	newLine->data[0] = '\n';
	if (currentLine->prev != NULL) {
		newLine->prev = currentLine->prev;
	}
	else {
		newLine->prev = NULL;
	}
	newLine->next = currentLine;
	currentLine->prev = newLine;
}


