CFLAGS = -O4 -Wall -pedantic -std=c99 -F /usr/local/lib/ -I /usr/local/include -lSDL2 -lSDL2_ttf -lSDL2_image
            
TARGET = pathMaker
SOURCES = $(TARGET).c
LIBS = -lm
CC = gcc 

all: $(TARGET)

$(TARGET) $(SOURCES): $(INCS)
	$(CC) $(SOURCES) -o pathMaker $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
