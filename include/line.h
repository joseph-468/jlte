#ifndef LINE_H
#define LINE_H
typedef struct Line {
	char *data;
	struct Line *next;
	struct Line *prev;
} Line;

void insertLineAfter(Line *currentLine);
void insertLineBefore(Line *currentLine);
#endif
