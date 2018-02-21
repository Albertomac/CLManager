# Makefile CLManager

CC = cc
CFLAGS = -fPIC -Wall -Wextra -O2 -g
LDFLAGS = -shared -framework OpenCL
RM = rm -f
TARGET_LIB = clmanager.so

SRCS = CLManager.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
