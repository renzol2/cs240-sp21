CC = gcc
INC = -I.
FLAGS = -W -Wall -g
LINKOPTS = -pthread

all: msort gen

msort: msort.c
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) msort.c -o msort

gen: gen.c
	$(CC) $(INC) $(FLAGS) gen.c -o gen

clean:
	rm -rf msort gen
