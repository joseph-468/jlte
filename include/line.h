#ifndef LINE_H
#define LINE_H

typedef struct Line {
	unsigned int length;
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void freeAllLines(Line *bufferHead);
void setSize(Line *currentLine, long size);
void resizeLine(Line *currentLine); 
void removeLine(Line *targetLine);
void insertLineAfter(Line *currentLine);
void insertLineBefore(Line *currentLine);

#endif
