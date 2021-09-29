CC = cc
CFLAGS= -L/usr/X11/lib -lX11
DEPS = hardware.h


%.o: %.c $(DEPS)
	$(CC) -c  ${CFLAGS} -o $@ $< $(CFLAGS)

dwmstatus: hardware.o status.o 
	$(CC) ${CFLAGS} -o dwmstatus hardware.o status.o
