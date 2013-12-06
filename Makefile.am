## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS} 
AM_CXXFLAGS = -std=c++0x -lopencv

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = motionapp

motionapp_SOURCES	= capture.cpp motiondetector.cpp motionapp.cpp main.cpp
