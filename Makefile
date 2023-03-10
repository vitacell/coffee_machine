CC = gcc
CFLAGS = -Wall -Werror -pedantic
LDFLAGS = -ldialog

all: coffee_machine

coffee_machine: coffee_machine.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f coffee_machine