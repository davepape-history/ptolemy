LIBDIR = $(HOME)/lib
CFLAGS = -O -I$(LIBDIR)
LIBS = -L$(LIBDIR) -lsurf -ldave -limage -lfm -lgl -lm -lmalloc
LINKFLAGS = -O -s

POBJS = main.o draw.o mirrors.o events.o batch.o image_io.o
ptolemy: $(POBJS)
	$(CC) $(LINKFLAGS) -o ptolemy $(POBJS) $(LIBS)

