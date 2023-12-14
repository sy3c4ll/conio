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

struct termios* _normal_attr() {
  static struct termios attr;
  static int init = 0;

  if (!init) {
    tcgetattr(STDIN_FILENO, &attr);
    init = 1;
  }

  return &attr;
}
struct termios* _getch_attr() {
  static struct termios attr;
  static int init = 0;

  if (!init) {
    attr = *_normal_attr();
    attr.c_lflag &= ~(ICANON | ECHO);
    init = 1;
  }

  return &attr;
}
struct termios* _getche_attr() {
  static struct termios attr;
  static int init = 0;

  if (!init) {
    attr = *_normal_attr();
    attr.c_lflag &= ~ICANON;
    init = 1;
  }

  return &attr;
}
int _normal_fl() {
  static int fl;
  static int init = 0;

  if (!init) {
    fl = fcntl(STDIN_FILENO, F_GETFL);
    init = 1;
  }

  return fl;
}
int _kbhit_fl() {
  static int fl;
  static int init = 0;

  if (!init) {
    fl = _normal_fl() | O_NONBLOCK;
    init = 1;
  }

  return fl;
}

inline void _normal_mode() {
  tcsetattr(STDIN_FILENO, TCSANOW, _normal_attr());
  fcntl(STDIN_FILENO, F_SETFL, _normal_fl());
}
inline void _getch_mode() {
  tcsetattr(STDIN_FILENO, TCSANOW, _getch_attr());
}
inline void _getche_mode() {
  tcsetattr(STDIN_FILENO, TCSANOW, _getche_attr());
}
inline void _kbhit_mode() {
  tcsetattr(STDIN_FILENO, TCSANOW, _getch_attr());
  fcntl(STDIN_FILENO, F_SETFL, _kbhit_fl());
}

namespace conio {
int getch() {
  _getch_mode();
  int ch = getchar();
  _normal_mode();
  return ch;
}
int getche() {
  _getche_mode();
  int ch = getchar();
  _normal_mode();
  return ch;
}
int kbhit() {
  _kbhit_mode();
  int ch = getchar();
  if (ch != EOF)
    ungetc(ch, stdin);
  _normal_mode();
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
  int x;
  _getch_mode();
  printf("\e[6n");
  scanf("\e[%*d;%dR", &x);
  _normal_mode();
  return x - 1;
}
int wherey() {
  int y;
  _getch_mode();
  printf("\e[6n");
  scanf("\e[%d;%*dR", &y);
  _normal_mode();
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
