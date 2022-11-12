CC = gcc
INC_DIR = include
HANDLER = src/socket-handler
DAEMON = src/daemon-control/daemon-control.c
OTHER = src/main.c src/tools.c src/parser/parser.c

SRC = $(OTHER) $(DAEMON) $(HANDLER)/config-manager.c  $(HANDLER)/request.c $(HANDLER)/response.c $(HANDLER)/socket-handler.c
CFLAGS = -I$(INC_DIR) -Wall -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -g -fsanitize=address



all : 
	reset
	clang-format -i src/*/*.c src/*.c include/*
	$(CC) $(CFLAGS) $(SRC) -o httpd
	./httpd -a start tests/test.conf
	
check:
	reset
	clang-format -i src/* include/*
	$(CC) $(CFLAGS) $(SRC) -o test


# delete commands  
       
.PHONY : clean 
clean : 
	rm -f test *~
