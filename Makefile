CC = clang
CFLAGS = -ggdb3 -O1 -std=c11 -Wall -Wextra -Werror -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
LDLIBS = -lm

%: %.c
	$(CC) $(CFLAGS) $< $(LDLIBS) -o $@

clean:
	rm -f *.o a.out
