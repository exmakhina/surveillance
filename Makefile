TARGET = motionapp

CC=g++

CFLAGS=-I. -std=c++0x -O2 -Wall
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`
LDFLAGS+=-ljson 

SRC=settings.cpp capture.cpp motiondetector.cpp fileaction.cpp motionapp.cpp main.cpp
DEPS=capture.h settings.h motiondetector.h action.h fileaction.h motionapp.h
OBJ=$(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS) 

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o *~ core $(TARGET)

