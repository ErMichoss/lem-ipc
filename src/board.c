#include "lem_ipc.h"

/**
 * @brief Puts player in the board
 * @return 0 if everything is right, 1 if an error occurs
 */
int put_player(t_player *p) {
    int x;
    int y;

    sem_lock(p->semid);

    int free_tiles = 0;
    for (int y = 0; y < MAX_H; y++)
        for (int x = 0; x < MAX_W; x++)
            if (p->shm->grid[y][x] == 0)
                free_tiles++;
    if (free_tiles == 0) {
        printf("Error: board is full, cannot add more players\n");
        sem_unlock(p->semid);
        shmdt(p->shm);
        return 1;
    }

    do {
        x = rand() % MAX_W;
        y = rand() % MAX_H;
    } while (p->shm->grid[y][x] != 0);
    p->shm->grid[y][x] = p->team_id;
    p->shm->player_count++;

    sem_unlock(p->semid);

    p->pos.x = x;
    p->pos.y = y;

    return 0;
}

/**
 * @brief Move player around the board
 * @param dir Directrion the player moves, 1 = left, 2 = up, 3 = right, 4 = down
 * @return 0 if everything is right, 1 if an error occurs
 */
int move_player(t_player *p, int dir) {
    if (dir == 0) return 0;

    int nx = p->pos.x;
    int ny = p->pos.y;

    if (dir == 1) nx -= 1;
    else if (dir == 2) ny -= 1;
    else if (dir == 3) nx += 1;
    else if (dir == 4) ny += 1;

    if (nx < 0 || nx >= MAX_W || ny < 0 || ny >= MAX_H) {
        printf("Unable to move, out of bounds (Player %i)\n", p->number);
        return 1;
    }

    sem_lock(p->semid);
    if (p->shm->grid[ny][nx] == 0) {
        p->shm->grid[ny][nx] = p->team_id;
        p->shm->grid[p->pos.y][p->pos.x] = 0;
        p->pos.x = nx;
        p->pos.y = ny;
    } else {
        printf("Unable to move, square occupied (Player %i)\n", p->number);
        sem_unlock(p->semid);
        return 1;
    }
    p->shm->dirs[ny][nx] = dir - 1;
    sem_unlock(p->semid);
    return 0;
}

/**
 * @brief Removes player from the board
 */
void remove_player(t_player *p) {
    sem_lock(p->semid);

    p->shm->grid[p->pos.y][p->pos.x] = 0;

    sem_unlock(p->semid);

    ipc_clean(p);
}