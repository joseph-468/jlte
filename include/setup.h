#ifndef SETUP_H
#define SETUP_H

#include "../include/line.h"

void startNcurses();
void setupColors();
WINDOW * setupMainWindow(int y, int x);
WINDOW * setupBar(int y, int x);
Line * createBufferHead();

#endif
