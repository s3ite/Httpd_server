CC = gcc
INC_DIR = include
SRC = src/socket-handler/socket-handler.c src/main.c src/parser/parser.c 
CFLAGS = -I$(INC_DIR) -Wall -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -g

all : 
	reset
	clang-format -i src/*/*
	$(CC) $(CFLAGS) $(SRC) -o httpd

check:
	reset
	clang-format -i src/* include/*
	$(CC) $(CFLAGS) $(SRC) -o test


# delete commands  
       
.PHONY : clean 
clean : 
	rm -f test *~