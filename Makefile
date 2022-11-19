CC = gcc
HANDLER = src/socket-handler
DAEMON = src/daemon-control/daemon-control.c
OTHER = src/main.c src/tools.c src/parser/parser.c src/event-handler/event-handler.c

SRC = $(OTHER) $(DAEMON) $(HANDLER)/config-manager.c  $(HANDLER)/request.c $(HANDLER)/response.c $(HANDLER)/socket-handler.c
CFLAGS =-Wall -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fsanitize=address -g

httpd:
	$(CC) $(CFLAGS) $(SRC) -o httpd

check:
	reset
	clang-format -i src/* 
	$(CC) $(CFLAGS) $(SRC) -o test


# delete commands  
       
.PHONY : clean 
clean : 
	rm -f test  httpd *~
