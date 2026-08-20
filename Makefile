CC = clang
CFLAGS = -ggdb3 -O1 -std=c11 -Wall -Wextra -Werror -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -MMD -MP
LDLIBS = -lm -lncurses

SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

DEPS = $(OBJS:.o=.d)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) a.out

.PHONY: all clean