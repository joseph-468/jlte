#ifndef FILEOP_H
#define FILEOP_H

#include <curses.h>
#include "../include/line.h"

char * openFileFromName(char fileName[], Line **currentLine, Line *bufferHead, WINDOW *win, WINDOW *bar);
char * openFile(Line **currentLine, Line *bufferHead, WINDOW *win, WINDOW *bar);
void saveFile(char *currentFile, Line *bufferHead, WINDOW *win, WINDOW *bar);

#endif
