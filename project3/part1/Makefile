CC = gcc
CFLAGS = -g -Wall
OBJS = indexPage.o crawler.o
CFILE = webSearch.c

all: webSearch

%.o: %.c $.h
	$(CC) $(CFLAGS) -c $< -o $@
webSearch: $(CFILE) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@
clean:
	rm *.o webSearch