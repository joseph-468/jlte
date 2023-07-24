#include <stdlib.h>
#include <string.h>
#include "../include/line.h"

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

void setSize(Line *currentLine, long size) {
	// Find nearest power of 2
	long i = 2;
	while (i < size+1) {
		i = i * 2;
	}
	// Allocate new memory
	char *tempData = malloc(strlen(currentLine->data));
	strcpy(tempData, currentLine->data);
	currentLine->length = i;
	currentLine->data = malloc(currentLine->length);
	strcpy(currentLine->data, tempData);
	free(tempData);
	memset(currentLine->data+strlen(currentLine->data), '\0', currentLine->length-strlen(currentLine->data));
}

void resizeLine(Line *currentLine) {
	if (strlen(currentLine->data)+1 >= currentLine->length) {
		currentLine->length = currentLine->length * 2;
		currentLine->data = realloc(currentLine->data, currentLine->length);
		memset(currentLine->data+currentLine->length/2, '\0', currentLine->length/2);
	}
	else if (strlen(currentLine->data)+1 <= currentLine->length/2 && currentLine->length/2 > 1) {
		currentLine->length = currentLine->length/2;
		currentLine->data = realloc(currentLine->data, currentLine->length);
	}
}

void removeLine(Line *targetLine) {
	targetLine->prev->next = targetLine->next;
	targetLine->next->prev = targetLine->prev;
	free(targetLine->data);
	free(targetLine);
}

void insertLineAfter(Line *currentLine) {
	Line *newLine = malloc(sizeof(Line));
	newLine->length = 2;
	newLine->data = malloc(newLine->length);
	memset(newLine->data, '\0', newLine->length);
	if (currentLine->next != NULL) {
		newLine->next = currentLine->next;
		currentLine->next->prev = newLine;
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
		currentLine->prev->next = newLine;
	}
	else {
		newLine->prev = NULL;
	}
	newLine->next = currentLine;
	currentLine->prev = newLine;
}


