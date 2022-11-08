CC = gcc
SRC = src/
INCLUDE = 
DEPS = 
CFLAGS = -Wall -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

all : 
	clang-format -i src/*
	$(CC) $(CFLAGS) $(SRC) -o httpd

check:
	reset
	clang-format -i src/*
	$(CC) $(CFLAGS) $(SRC) -g -o test


# delete commands  
       
.PHONY : clean 
clean : 
	rm -f test *~