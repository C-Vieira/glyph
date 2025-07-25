## Glyph Framework
CC = gcc

GLYPH_CFLAGS = -std=c99 -Wall -Werror -g ##-fsanitize=address

LIB_NAME = libglyph.a

GLYPH_SOURCE := $(wildcard ./glyph/src/*.c)
GLYPH_OFILES := $(GLYPH_SOURCE:.c=.o)

all: glyph libglyph snake run glyph_fclean snake_clean

## Compile source into obj files
glyph:
	$(CC) $(GLYPH_CFLAGS) -c $(GLYPH_SOURCE) -o $(GLYPH_OFILES)

## Build obj files into a static library
libglyph: $(GLYPH_OFILES)
	ar rcs $(LIB_NAME) $(GLYPH_OFILES)

glyph_clean:
	rm -f $(GLYPH_OFILES)

glyph_fclean: glyph_clean 
	rm -f $(LIB_NAME)

## Snake Example
SNAKE_CFLAGS = -std=c99 -Wall -Werror -g ##-fsanitize=address

## Flags for linking libraries to game source
SNAKE_LDFLAGS = -L. -lglyph -lncurses

SNAKE_SOURCE = $(wildcard ./examples/snake/src/*.c)

## Compile game source and link it with library
snake:
	$(CC) $(SNAKE_CFLAGS) $(SNAKE_SOURCE) -o snake $(SNAKE_LDFLAGS)

run:
	./snake

snake_clean:
	rm snake

