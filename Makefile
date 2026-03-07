NAME    = lemipc

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
IFLAGS  = -I incl

SRCS    = src/main.c        \
          src/ipc_init.c    \
          src/ipc_clean.c   \
          src/board.c       \
          src/player.c      \
          src/messages.c    \
          src/display.c     \
          src/aux.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

ifdef BOARD
CFLAGS += -DMAX_H=$(word 1, $(BOARD)) -DMAX_W=$(word 2, $(BOARD))
endif

ifdef TEAMS
CFLAGS += -DMAX_TEAMS=$(TEAMS)
endif

.PHONY: all clean fclean re