CFLAGS=-framework ApplicationServices -framework Carbon
SOURCES=main.c keyrec.c
EXECUTABLE=keyrec

all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
