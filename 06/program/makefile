CC = clang
CFLAGS = -g -Wall -ferror-limit=3
TARGET = program
INCLUDES = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I.
SRCS = ./src/translate.c ./src/program.c ./src/parser.c
DEST = ./bin/$(TARGET)

all: $(TARGET)

$(TARGET): 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DEST) $(SRCS)
