CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++14 -Iinclude
LDFLAGS = -Llib
LDLIBS = -lcurl

SRCDIR = src
INCDIR = include
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
EXECUTABLE = $(BINDIR)/app

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@ $(LDLIBS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BINDIR)/*.o $(EXECUTABLE)

.PHONY: clean
