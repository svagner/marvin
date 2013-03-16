.include <bsd.own.mk>

PROG=   marvin
MAN=    marvin.1
SRCS=   screen.c marvin.c
CFLAGS+= -lncurses -I/usr/local/include

.include <bsd.prog.mk>
