#ifndef BUFFEROP_H
#define BUFFEROP_H

#include <curses.h>
#include "../include/line.h"

void printFullBuffer(Line **currentLine, Line *bufferHead, WINDOW *win, int resY, int realX, int x, int y);
void createNewLine(Line **currentLine, WINDOW *win, int *realX, int y);
void insertCharacter(Line *currentLine, WINDOW *win, int ch, int *realX, int *lastXPos, int x, int y);

#endif
