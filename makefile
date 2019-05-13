CXXVERSION = 14
CFLAGS= -std=c++$(CXXVERSION) -Wall -g -O2 
FLAGS=`pkg-config libgvc --cflags`
LDFLAGS=`pkg-config libgvc --libs`

all: 
	g++  $(CFLAGS) main.cpp $(LDFLAGS) 