# twp
### C++ and ncurses simple word processor

twp (the word processor) is an attempt to study the data structures needed for a tiny word processor (not a text editor) based on NCURSES for simplicity and study purpose.

--------
## How to build it
You need to manually `mkdir obj` and then just... `make` it.
It's a very simple project, it needs only ncurses and pthreads.

## Notes on running
There's a logging facility on the stderr stream, so in order to run you should do something like: 
```
mkfifo twp.log
tail -f twp.log
```
on a terminal, and on another one:
```
./twp 2>twp.log
```
