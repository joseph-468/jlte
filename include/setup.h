#ifndef SETUP_H
#include "../include/line.h"
int startNcurses();
void setupColors();
WINDOW * setupMainWindow(int y, int x);
WINDOW * setupBar(int y, int x);
Line * createBufferHead();
#define SETUP_H
#endif
