CXXVERSION = 14
CFLAGS= -std=c++$(CXXVERSION)  -g 
FLAGS=`pkg-config libgvc --cflags`
LDFLAGS=`pkg-config libgvc --libs`

all: 
	g++  $(CFLAGS) $(FLAGS) UnBFluxo.cpp $(LDFLAGS) 