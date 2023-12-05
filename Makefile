# compiler and flags
CXX = g++
CXXFLAGS = -Wall -Iinclude $(shell find include -type d | sed 's/^/-I/')
LDFLAGS = -lodbc

# boost
BOOST_INCLUDE = /usr/lib/boost/include
BOOST_LIB = /usr/lib/boost/lib

CXXFLAGS += -I$(BOOST_INCLUDE)
LDFLAGS += -L$(BOOST_LIB) -lboost_locale

# dirs
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))
TARGET = $(BINDIR)/myapp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/* $(TARGET)

.PHONY: all clean
