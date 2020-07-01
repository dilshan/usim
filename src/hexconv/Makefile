CC = gcc
CFLAGS = -g -Wall

ODIR =../../obj/hexconv
BINDIR = ../../bin

_OBJ = error.o hexproc.o memmap.o strutil.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/hexconv: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)	

.PHONY: clean	

clean:
	$(RM) $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	$(RM) $(BINDIR)/hexconv
