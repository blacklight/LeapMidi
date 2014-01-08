INCLUDES = -I./include

ARCH := $(shell getconf LONG_BIT)
LIBS_32 = -L./lib/x86 -lLeap -lasound
LIBS_64 = -L./lib/x64 -lLeap -lasound

SOURCES = \
		leap.cpp \
		midi.cpp \
		main.cpp

APPNAME = leapmidi

CFLAGS = -Wall -g

CC = g++

all:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(APPNAME) $(SOURCES) $(LIBS_$(ARCH))

