LIBDIR = $(HOME)/lib
CFLAGS = -O2 -I$(LIBDIR)
LIBS = -L$(LIBDIR) -lsurf -ldave -lgl -lm -limage
LINKFLAGS = -O2 -s

MOBJS = main.o draw.o events.o batch.o

map: $(MOBJS)
	$(CC) $(LINKFLAGS) -o map $(MOBJS) $(LIBS)


$(MOBJS): map.h
