`View full version of file: $ man ./README.7`

## NAME

ncs \- Just a random C shitcode
that i wrote to test something.

## FILE AND FDs

In libc (musl for example) FILE struct is a just
fd wrapper. You can open any file descriptor as
file via fdopen(3). Also, FILE stream has more
functions that fd, for example getline(3) accepts
only FILE pointer, not fd.

`filesize.c <filename>`
Shows file size via two ways:
ftell(3) and lseek(2). It should be equals.

`sock_as_file.c`
Creates normal unix socket at
.B /tmp/sock_as_file.socket
but recieving any fd as FILE and
uses getline(3) to read lines.

`write_to_sock.c`
Helper program that writes data to
.B /tmp/sock_as_file.socket

## KEYBOARD AND "NCURSES-LIKE" APPS

Dumb programs, that uses termios(3)
and some ANSI escape codes (like CSI)
to draw tui interface.

`keyboard.c`
Just a test keyboard display that can...
umm... display key, that you pressed. Very
useful app, please ignore.

`keyboard-v2.c`
Same as keyboard.c, but it have numbers layout
and can record 32 last symbols. Also, it very
buggy.

`disp.c <filename>`
Um... This use termios(3), but it does not have
"tui". It works like cat(1), but it clears screen
and display file content. Also, uses sendfile(2)
syscall instead of normal write(2) (very useful
for program, that display files).

## NANOQSH-RELATED

Special for @nanoqsh...

`pt.c`
Threads can handle signals. Yes.

`vec.c`
Very dumb implementation of rust's std::vec::Vec.
Buggy and unsafe. Anyway, copyright for that you can
find on github.com/rust-lang/rust/blob/master/LICENSE-MIT

## I USED WEBPACK TO COMPILE MY C CODE GUYS

`wtf.c`
Sums two complex numbers. Also, can sum segfaults.

## LICENSE

For all except vec.c: The Unlicense (unlicense.org)

For vec.c: The Unlicense (unlicense.org)

Author of all: Amchik <github.com/Amchik>

## BUGS

You can drop any found bugs via special file
on linux:

$ echo "Hello, i found a bug in somefile.c: ..." > /dev/null

Also, please attach result of this benchmark
(if results less than 7EiB/s i will ignore it)

$ dd if=/dev/zero of=/dev/null status=progress

### KNOWN BUGS
my skill in English and C languages
(wontfix)

## SEE ALSO

fdopen(3)
getline(3)
ftell(3)
lseek(2)
termios(3)
cat(1)
sendfile(2)
write(2)

https://github.com/rust-lang/rust/blob/master/LICENSE-MIT

https://github.com/Amchik/vol2-utils

https://github.com/Amchik/snus
