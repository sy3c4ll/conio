#ifndef CONIO_H
#define CONIO_H

namespace conio {
int getch();
int getche();
int kbhit();
int putch(int ch);
int ungetch(int ch);

void clrscr();
void clreol();

int wherex();
int wherey();
void gotoxy(int x, int y);

void textcolor(int color);
void textbackground(int color);
}

#endif
