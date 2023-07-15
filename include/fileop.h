#ifndef FILEOP_H
#include <curses.h>
#include "../include/line.h"
char * openFile(Line **currentLine, Line *bufferHead, WINDOW *win, WINDOW *bar);
void saveFile(char *currentFile, Line *bufferHead, WINDOW *win, WINDOW *bar);
#define FILEOP_H
#endif
