#if defined(_WIN32) || defined(_WIN64)

#include <conio.h>
#include <windows.h>

namespace conio {
int getch() { return _getch(); }
int getche() { return _getche(); }
int kbhit() { return _kbhit(); }
int putch(int ch) { return _putch(ch); }
int ungetch(int ch) { return _ungetch(ch); }

void clrscr() {
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD home = {0, 0};
  DWORD count;
  DWORD nwrite;

  GetConsoleScreenBufferInfo(hStdOut, &csbi);

  count = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hStdOut, ' ', count, home, &nwrite);
  FillConsoleOutputAttribute(hStdOut, csbi.wAttribute, count, home, &nwrite);

  SetConsoleCursorPosition(hStdOut, home);
}
void clreol() {
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD cur;
  DWORD count;
  DWORD nwrite;

  GetConsoleScreenBufferInfo(hStdOut, &csbi);

  cur = csbi.dwCursorPosition;
  count = csbi.dwSize.X - csbi.dwCursorPosition.X;
  FillConsoleOutputCharacter(hStdOut, ' ', count, cur, &nwrite);
  FillConsoleOutputAttribute(hStdOut, csbi.wAttribute, count, cur, &nwrite);
}

int wherex() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  return csbi.dwCursorPosition.X;
}
int wherey() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  return csbi.dwCursorPosition.Y;
}
void gotoxy(int x, int y) {
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{x, y});
}

void textcolor(int color) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void textbackground(int color) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color << 4);
}
}

#elif defined(__APPLE__) || defined(__MACH__) || defined(__unix__)

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

inline void _enable_canon() {
  struct termios attr;
  tcgetattr(STDIN_FILENO, &attr);
  attr.c_lflag |= ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}
inline void _enable_echo() {
  struct termios attr;
  tcgetattr(STDIN_FILENO, &attr);
  attr.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}
inline void _enable_block() {
  int fl = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, fl & ~O_NONBLOCK);
}
inline void _disable_canon() {
  struct termios attr;
  tcgetattr(STDIN_FILENO, &attr);
  attr.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}
inline void _disable_echo() {
  struct termios attr;
  tcgetattr(STDIN_FILENO, &attr);
  attr.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}
inline void _disable_block() {
  int fl = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK);
}

namespace conio {
int getch() {
  _disable_canon();
  _disable_echo();
  _enable_block();

  int ch = getchar();

  _enable_canon();
  _enable_echo();

  return ch;
}
int getche() {
  _disable_canon();
  _enable_echo();
  _enable_block();

  int ch = getchar();

  _enable_canon();

  return ch;
}
int kbhit() {
  _disable_canon();
  _disable_echo();
  _disable_block();

  int ch = getchar();

  _enable_canon();
  _enable_echo();
  _enable_block();

  if (ch != EOF)
    ungetc(ch, stdin);
  return ch != EOF;
}
int putch(int ch) {
  return putchar(ch);
}
int ungetch(int ch) {
  return ungetc(ch, stdin);
}

void clrscr() {
  printf("\e[2J");
}
void clreol() {
  printf("\e[K");
}

int wherex() {
  _disable_canon();
  _disable_echo();
  _enable_block();

  int x;
  printf("\e[6n");
  scanf("\e[%*d;%dR", &x);

  _enable_canon();
  _enable_echo();

  return x - 1;
}
int wherey() {
  _disable_canon();
  _disable_echo();
  _enable_block();

  int y;
  printf("\e[6n");
  scanf("\e[%d;%*dR", &y);

  _enable_canon();
  _enable_echo();

  return y - 1;
}
void gotoxy(int x, int y) {
  printf("\e[%d;%dH", y + 1, x + 1);
}

void textcolor(int color) {
  printf("\e[%dm", color + 30);
}
void textbackground(int color) {
  printf("\e[%dm", color + 40);
}
}

#else

#error "Unsupported platform"

#endif
