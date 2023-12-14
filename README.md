# Conio for C++

A cross-platform, sanitised, C++ replacement for `conio.h`.

## Introduction

There is no good reason to use `conio.h` at this time and age. Maybe you're a
wee kid who's just heard of `getch` and `system` and is inspired to write the
next greatest console game. Or maybe you're a student, forced to use `conio.h`
for the sake of 'education'. Buf if you're a grown-ass dev trying to make this
abomination work just because, please take a look at
[notcurses](https://github.com/dankamongmen/notcurses) because you deserve
better.

This library, like `conio.h` and its use cases, is extremely messy and hacky.
It uses `conio.h` where it is available, carefully taking what's most likely to
be there, and relies on platform-specific implementation details elsewhere. Its
purpose is to provide pain relief for those who must bear with `conio.h` by
facilitating cross-platform coding, making the library available where it isn't
and implementing a few stray functions that may be missing from your local copy
of `conio.h` because `conio.h` is non-standard, get over it. Its purpose is
**not** to be a safe and complete terminal manipulation library! To these ends,
it provides a firm, clean basis for which you know will compile on other
platforms, has certain functions implemented, and has nothing else, no jumpscare
macros to give you debugging nightmares, no weird symbol names, no nonsense.

Have fun.

## How to use

The most obvious way would be to copy over `conio.h` and `conio.cpp`, and
compile them within your project as if they were your own. I shan't bore you
with specifics, but resources on how to structure and build your project should
be abundant upon the Web.

_I highly recommend **against** combining the two files for a single import._
This loses the benefits of a secluded, sanitised environment the simple header
file provides.

Alternatively, you could compile `conio.cpp` into a separate static or dynamic
library, include `conio.h` in your source files then link the library after. If
this option came to your mind, I'm sure I don't need to guide you through it.

Remember to qualify the name! Use either `conio::foo`, or just `foo` after
`using conio::foo`. You can use `using namespace conio` if you want to forget
about namespaces and use the library as a drop-in replacement for `conio.h`.

## Implemented

- `getch`
- `getche`
- `kbhit`
- `putch`
- `ungetch`
- `clrscr`
- `clreol`
- `wherex`
- `wherey`
- `gotoxy`
- `textcolor`
- `textbackground`

## Benefits

Using this library provides you with the following benefits (over using your
stock `conio.h`):

- **Cross-platform**
  All functions are implemented for UNIX-derivative systems as well.
- **Extensive**
  Well not really, but at least it has `clrscr`...
- **Uniform**
  You don't know what functions you get with stray `conio.h` implementations.
  This library takes the lowest common denominator and implements everything
  else.
- **Clean**
  No unnecessary symbols, no hidden `#define`s. You know how bad those can screw
  over your debugging sessions.
- **Modern**
  All functions are placed under the namespace `conio`, preventing name clashes.
- **Single-file**
  All these benefits can be gained without installing or bundling massive
  codebases, by just copy-pasting two files!

## How it works

As stated before, this library uses `conio.h` where it is available.

On 32-bit and 64-bit Windows, the following carries over from `conio.h`:

- `getch`
- `getche`
- `kbhit`
- `putch`
- `ungetch`

These five functions were deemed safe to use because they exist in MinGW, with
arguably the slimmest `conio.h` of them all.

The rest are implemented through the
[Windows Console API](https://learn.microsoft.com/en-us/windows/console/),
`windows.h`.

On UNIX derivatives, this library makes extensive use of the [General Terminal
Interface](https://pubs.opengroup.org/onlinepubs/7908799/xbd/termios.html) and
[ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code). Canonical
mode and echo is turned off to emulate `getch`, and nonblock is also enabled
for `kbhit`.

## Pending implementations (TODOs)

- [ ] Unified constants for colours and attributes

## Known issues

- [x] Rapid consecutive calls to `getch` or `kbhit` can reveal input on UNIX
  systems, possibly because calls to `tcgetattr` are slow

## (Un)license

All material in this repo is hereby released to the public domain, in the
spirit of allowing you to use, hack or adapt the source code to fit your needs,
as I have done to others before me, without worrying about crediting the author.
You are free to use any written material within this repository, in part or in
whole, for any purpose, without crediting me. See [UNLICENSE](./UNLICENSE) for
more details.

## References

- [kozlek/xplatform](https://github.com/kozlek/xplatform/blob/master/XPlatform.c)
- [ChaiTeaNunes/CPP-Console-Game](https://github.com/ChaiTeaNunes/CPP-Console-Game/blob/master/platform_conio.cpp)
- [Flawww/linux_conio](https://github.com/Flawww/linux_conio/blob/main/linux_conio.cpp)
- [Console documentation - Windows Console | Microsoft Learn](https://learn.microsoft.com/en-us/windows/console/)
- [conio.h - RAD Studio](https://docwiki.embarcadero.com/RADStudio/Alexandria/en/Conio.h_Index)
- [conio.h functions in mingw?](https://cboard.cprogramming.com/cplusplus-programming/93806-conio-h-functions-mingw.html#post672795)
- [termios(3) - Linux manual page](https://man7.org/linux/man-pages/man3/termios.3.html)
- [ANSI/VT100 Terminal Control](https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm)
- [Clear the screen - C++ Forum](https://cplusplus.com/forum/articles/10515/)
