TARGET = motionapp

CC=g++

# Flags for debug
# CFLAGS=-I. -std=c++0x -O0 -g3 -Wall
# Flags for releases
CFLAGS=-I. -std=c++0x -O2 -Wall
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`
LDFLAGS+=-ljson 

SRC=settings.cpp capture.cpp motiondetector.cpp fileaction.cpp socketexception.cpp
SRC+=udpsocket.cpp tcpsocket.cpp motionapp.cpp sessionmanager.cpp connector.cpp main.cpp
DEPS=capture.h settings.h motiondetector.h action.h fileaction.h socketexception.h
DEPS+=udpsocket.h tcpsocket.h sessionmanager.h appobject.h motionapp.h connector.h
OBJ=$(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS) 

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o *~ core $(TARGET)

