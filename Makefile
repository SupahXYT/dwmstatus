CC = cc
CFLAGS= -L/usr/X11/lib -lX11
DEPS = hardware.h
WORD=$PWD
PREFIX=/usr/local/

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dwmstatus: hardware.o status.o 
	$(CC) ${CFLAGS} -o dwmstatus hardware.o status.o

install:
	mkdir -p $(DESTDIR)$(DESTPREFIX)/bin
	cp dwmstatus $(DESTDIR)$(PREFIX)/bin/dwmstatus

uninstall

