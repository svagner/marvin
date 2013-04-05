.include <bsd.own.mk>

PROG=   marvin
MAN=    marvin.1
SRCS=   screen.c marvin.c
CFLAGS+= -lncurses -lmenu -lform -I/usr/local/include -ggdb

.include <bsd.prog.mk>
