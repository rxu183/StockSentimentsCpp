CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -Iinclude
LDFLAGS =

SRCDIR = src
INCDIR = include
BINDIR = bin
CONFIGDIR = json

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
EXECUTABLE = $(BINDIR)/app

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BINDIR)/*.o $(EXECUTABLE)

.PHONY: clean
