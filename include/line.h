#ifndef LINE_H
#define LINE_H

typedef struct Line {
	unsigned int length;
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void freeAllLines(Line *bufferHead);
void resizeLine(Line *currentLine); 
void insertLineAfter(Line *currentLine);
void insertLineBefore(Line *currentLine);
#endif
