CC := gcc
CFLAGS := -g -Wall -Werror -std=c18
CFLAGS += `pkg-config flexiblas sdl3 --cflags`
LDLIBS := `pkg-config flexiblas sdl3 --libs`

TARGET := vong

.PHONY: all clean

all: $(TARGET)

$(TARGET): vong.o

clean:
	rm -rf *.o $(TARGET)