#include "lem_ipc.h"

void display_board(t_player *p) {
    write(1, "\033[2J", 4);

    for (int j = 0; j < MAX_H; j++) {
        for (int i = 0; i < MAX_W; i++){
            if (p->shm->grid[j][i] == 0) { ft_putchar('.');  } else { ft_putnbr(p->shm->grid[j][i]); }
            ft_putchar(' ');
        }
        ft_putchar('\n');
    }
}