NAME    = lemipc

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
IFLAGS  = -I incl \
          -I sdl2/include/SDL2 \
          -I sdl2_image/include/SDL2

LDFLAGS = -L sdl2/lib -lSDL2 \
          -L sdl2_image/lib -lSDL2_image \
          -Wl,-rpath,$(shell pwd)/sdl2/lib \
          -Wl,-rpath,$(shell pwd)/sdl2_image/lib

SRCS    = src/main.c        \
          src/ipc_init.c    \
          src/ipc_clean.c   \
          src/board.c       \
          src/player.c      \
          src/move.c        \
          src/messages.c    \
          src/display.c     \
          src/aux.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

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