
CC=gcc

# The line with -DDEBUG can be used for development. When
# building your code for evaluation, however, the line *without*
# the -DDEBUG will be used.
#

CFLAGS=-c -Wall -g -DDEBUG -D_GNU_SOURCE -std=c99 -O0


all: route_manager

route_manager: route_manager.o
	$(CC) route_manager.o -o route_manager

route_manager.o: route_manager.c
	$(CC) $(CFLAGS) route_manager.c

clean:
	rm -rf *.o route_manager 
