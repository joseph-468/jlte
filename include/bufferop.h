#ifndef BUFFEROP_H
#define BUFFEROP_H

#include <curses.h>
#include "../include/line.h"

void moveUp(Line **currentLine, WINDOW *win, int *realX, int lastXPos, int x, int y);
void moveDown(Line **currentLine, WINDOW *win, int *realX, int lastXPos, int x, int y);
void moveRight(Line **currentLine, WINDOW *win, int *realX, int *lastXPos, int x, int y);
void moveLeft(Line **currentLine, WINDOW *win, int *realX, int *lastXPos, int x, int y);
void backspace(Line **currentLine, WINDOW *win, int *realX, int x, int y);
void printBuffer(Line **currentLine, Line *bufferHead, WINDOW *win, int resY, int realX, int x, int y);
void createNewLine(Line **currentLine, WINDOW *win, int *realX, int *lastXPos, int y);
void insertCharacter(Line *currentLine, WINDOW *win, int ch, int *realX, int *lastXPos, int x, int y);

#endif
