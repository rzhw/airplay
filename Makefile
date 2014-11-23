CC=clang++
CFLAGS=-g

all: program
program: src/main.cpp
	     $(CC) src/blah.cpp src/ethshim.cpp src/EthernetBonjour.cpp src/main.cpp -o wat -lmicrohttpd

clean:
	rm wat
