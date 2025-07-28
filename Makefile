## Glyph Framework
CC = gcc

GLYPH_CFLAGS = -std=c99 -Wall -Werror -g ##-fsanitize=address

LIB_NAME = libglyph.a

GLYPH_SOURCE := $(wildcard ./glyph/src/*.c)
GLYPH_OFILES := $(GLYPH_SOURCE:.c=.o)

## Build Shorthand
## Snake
##all: glyph libglyph snake snake_run snake_clean

## Sokoban
all: glyph libglyph sokoban sokoban_run sokoban_clean

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

## Examples
## Compiler flags
CFLAGS = -std=c99 -Wall -Werror -g ##-fsanitize=address

## Flags for linking libraries to game source
LDFLAGS = -L. -lglyph -lncurses

## Snake Example
SNAKE_SOURCE = $(wildcard ./examples/snake/src/*.c)

## Compile game source and link it with library
snake: glyph libglyph
	$(CC) $(CFLAGS) $(SNAKE_SOURCE) -o snake $(LDFLAGS)

snake_run:
	./snake

snake_clean: glyph_fclean
	rm snake

## Sokoban Example
SOKOBAN_SOURCE = $(wildcard ./examples/sokoban/src/*.c)

## Compile game source and link it with library
sokoban: glyph libglyph
	$(CC) $(CFLAGS) $(SOKOBAN_SOURCE) -o sokoban $(LDFLAGS)

sokoban_run:
	./sokoban

sokoban_clean: glyph_fclean
	rm sokoban 
