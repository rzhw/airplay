CC=clang++
CXXFLAGS=-Wall -std=c++11
SRCDIR=src
BUILDDIR=build
TARGET=wat

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/bonjour/blah.o: $(SRCDIR)/bonjour/blah.cpp
$(BUILDDIR)/bonjour/ethshim.o: $(SRCDIR)/bonjour/ethshim.cpp
$(BUILDDIR)/bonjour/blah.o $(BUILDDIR)/bonjour/ethshim.o:
	@mkdir -p $(BUILDDIR)/bonjour
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/bonjour/EthernetBonjour.o: $(SRCDIR)/bonjour/EthernetBonjour.cpp
	@mkdir -p $(BUILDDIR)/bonjour
	$(CC) $(CXXFLAGS) -w -c -o $@ $<

#$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
#	$(CC) $(CXXFLAGS) -c -o $@ $<
$(BUILDDIR)/airplay_http.o: $(SRCDIR)/airplay_http.cpp
$(BUILDDIR)/plist.o: $(SRCDIR)/plist.cpp
$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp
$(BUILDDIR)/airplay_http.o $(BUILDDIR)/plist.o $(BUILDDIR)/main.o:
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/$(TARGET): $(BUILDDIR)/bonjour/blah.o $(BUILDDIR)/bonjour/ethshim.o $(BUILDDIR)/bonjour/EthernetBonjour.o $(BUILDDIR)/airplay_http.o $(BUILDDIR)/plist.o $(BUILDDIR)/main.o
	$(CC) $(CXXFLAGS) -lmicrohttpd $(BUILDDIR)/bonjour/blah.o $(BUILDDIR)/bonjour/ethshim.o $(BUILDDIR)/bonjour/EthernetBonjour.o $(BUILDDIR)/airplay_http.o $(BUILDDIR)/plist.o $(BUILDDIR)/main.o -o $@

clean:
	rm -rf build/
