NAME    = lemipc

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
IFLAGS  = -I incl

SRCS    = srcs/main.c        \
          srcs/ipc_init.c    \
          srcs/ipc_clean.c   \
          srcs/board.c       \
          srcs/player.c      \
          srcs/move.c        \
          srcs/messages.c    \
          srcs/display.c

OBJS    = $(SRCS:.c=.o)

LIBFT   = libft/libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L libft -lft -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

ifdef BOARD
CFLAGS += -DMAX_H=$(word 1, $(BOARD)) -DMAX_W=$(word 2, $(BOARD))
endif

.PHONY: all clean fclean re