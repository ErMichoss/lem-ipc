#include "lem_ipc.h"

t_position find_enemy(t_player *p) {
    sem_lock(p->semid);

    t_position positions[p->shm->player_count - 1];
    int pos_idx = 0;
    int closest_pos = -1;
    int closes_pos_idx = 0;
    
    for (int y = 0; y < MAX_H; y++) {
        for (int x = 0; x < MAX_W; x++) {
            if (p->shm->grid[y][x] != 0 && 
                p->shm->grid[y][x] != p->team_id &&
                !(x == p->pos.x && y == p->pos.y)) {
                positions[pos_idx].x = x;
                positions[pos_idx].y = y;
                pos_idx++;
            }
        }
    }

    if (pos_idx == 0) {
        sem_unlock(p->semid);
        return p->pos;  // quedarse quieto
    }

    for (int i = 0; i < pos_idx; i++) {
        int pos = manhattan(positions[i], p->pos);
        if (closest_pos == -1) { 
            closest_pos = pos;
            closes_pos_idx = i;
        } else if (pos < closest_pos) { 
            closest_pos = pos;
            closes_pos_idx = i;
        }
    }

    sem_unlock(p->semid);

    return positions[closes_pos_idx];
}