CC = clang
CFLAGS = -ggdb3 -O1 -std=c11 -Wall -Wextra -Werror -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
LDLIBS = -lm -lncurses

SRCS = main.c helper.c

OBJS = $(SRCS:.c=.o)


TARGET = mein_programm

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET) a.out

.PHONY: all clean