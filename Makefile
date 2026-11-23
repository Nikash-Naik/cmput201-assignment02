CC = gcc
CFLAGS = -g -Wall -Werror -std=c99

assg: src/assg.c
	$(CC) $(CFLAGS) src/assg.c -o assg

clean:
	rm -f assg