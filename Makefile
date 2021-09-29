CC = cc
CFLAGS= -L/usr/X11/lib -lX11
DEPS = hardware.h
WORD=$PWD
PREFIX=/usr/local/

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dwmstatus: hardware.o status.o 
	$(CC) ${CFLAGS} -o dwmstatus hardware.o status.o

install: dwmstatus 
	mkdir -p /usr/local/bin
	cp dwmstatus /usr/local/bin/dwmstatus

uninstall:
	@rm -f /usr/local/bin/dwmstatus

clean:
	@rm -f hardware.o status.o dwmstatus
