CC = gcc
CFLAGS = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O2
DIRS = prob1 prob2 prob3 prob4 prob5 prob6 prob7 exercise1 exercise2
BINS = $(addsuffix /main,$(DIRS))

.PHONY: all test
all: $(BINS)

%/main: %/main.c
	$(CC) $(CFLAGS) $< -o $@

prob5/main prob6/main prob7/main: %/main: %/student.h

test: all
	python3 tests/test_chap5.py
