CC = gcc
CFLAGS = -g -Wall

ODIR =../../obj/usim
BINDIR = ../../bin

_OBJ = vmcpu.o error.o dump.o console.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/usim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)	

.PHONY: clean	

clean:
	$(RM) $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	$(RM) $(BINDIR)/usim
