CC = gcc
HANDLER = src/socket-handler
DAEMON = src/daemon-control/daemon-control.c
OTHER = src/main.c src/tools.c src/parser/parser.c

SRC = $(OTHER) $(DAEMON) $(HANDLER)/config-manager.c  $(HANDLER)/request.c $(HANDLER)/response.c $(HANDLER)/socket-handler.c
CFLAGS =-Wall -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fsanitize=address -g

all : 
	reset
	clang-format -i src/*/*.c src/*.c src/*/*.h src/*.h
	$(CC) $(CFLAGS) $(SRC) -o httpd
	./httpd -a start tests/test.conf

httpd:
		
check:
	reset
	clang-format -i src/* 
	$(CC) $(CFLAGS) $(SRC) -o test


# delete commands  
       
.PHONY : clean 
clean : 
	rm -f test *~
