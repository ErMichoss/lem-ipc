NAME         = lemipc
NAME_DISPLAY = lemipc_display

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
IFLAGS  = -I incl \
          -I sdl2/include/SDL2 \
          -I sdl2_image/include/SDL2 \
          -I sdl2_ttf/include/SDL2

LDFLAGS = -L sdl2/lib -lSDL2 \
          -L sdl2_image/lib -lSDL2_image \
          -L sdl2_ttf/lib -lSDL2_ttf \
          -Wl,-rpath,$(shell pwd)/sdl2/lib \
          -Wl,-rpath,$(shell pwd)/sdl2_image/lib \
          -Wl,-rpath,$(shell pwd)/sdl2_ttf/lib \
          -lm

SRCS = src/main.c     \
       src/ipc_init.c \
       src/ipc_clean.c \
       src/board.c    \
       src/player.c   \
       src/display.c  \
       src/messages.c \
       src/brain.c    \
       src/aux.c

SRCS_DISPLAY = src/display_main.c \
               src/ipc_init.c     \
               src/aux.c

OBJS         = $(SRCS:.c=.o)
OBJS_DISPLAY = $(SRCS_DISPLAY:.c=.o)

all: $(NAME) $(NAME_DISPLAY)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(NAME_DISPLAY): $(OBJS_DISPLAY)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS_DISPLAY) $(LDFLAGS) -o $(NAME_DISPLAY)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_DISPLAY)

fclean: clean
	rm -f $(NAME) $(NAME_DISPLAY)

re: fclean all

ifdef BOARD
CFLAGS += -DMAX_H=$(word 1, $(BOARD)) -DMAX_W=$(word 2, $(BOARD))
endif

ifdef TEAMS
CFLAGS += -DMAX_TEAMS=$(TEAMS)
endif

.PHONY: all clean fclean re