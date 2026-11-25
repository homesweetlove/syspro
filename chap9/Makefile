CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200809L

SOURCES = $(wildcard prob*/*.c hw*/*.c)
TARGETS = $(patsubst %.c,build/%,$(SOURCES))

.PHONY: all
all: $(TARGETS)

build/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
