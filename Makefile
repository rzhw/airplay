CC=clang++
CFLAGS=-g

all: program
program: src/main.cpp
	     $(CC) src/blah.cpp src/ethshim.cpp src/EthernetBonjour.cpp src/plist.cpp src/airplay_http.cpp src/main.cpp -std=c++11 -o wat -lmicrohttpd

clean:
	rm wat
