P = prettybtab
OBJS = main.o note_table.o
CFLAGS = -g -Wall -Wstrict-prototypes
LDLIBS = -lb

$(P): $(OBJS)
	$(CC) -o $(P) $(LDFLAGS) $(OBJS) $(LDLIBS)

clean:
	rm -f *o; rm -f $(P)
