UNAME_S := $(shell uname -s)

CC = gcc
CFLAGS = --std=c99 -Wall -pedantic -O3

ifeq ($(UNAME_S), Darwin)
	LDFLAGS = -framework OpenCL
else
	LDFLAGS = -lOpenCL
endif

SRCS = vecsum.c ../CLManager.c
OBJS = $(SRCS:.c=.o)
MAIN = vecsum


$(MAIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)


.PHONY: clean
clean:
	${RM} $(MAIN) ${OBJS}
