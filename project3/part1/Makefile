CC = gcc
CFLAGS = -g -Wall
OBJS = indexPage.o crawler.o
CFILE = webSearch.c

all: webSearch
webSearch:
	$(CC) $(CFLAGS) $(OBJS) $(CFILE) $< -o $@
%.o: %.c $.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o webSearch