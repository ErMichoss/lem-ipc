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

t_position find_best_pos_around_target(t_player *p, t_position target) {
    int offsets[8][2] = {
        {-1,-1}, {0,-1}, {1,-1},
        {-1, 0},         {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };
    t_position best = target;
    int best_dist = -1;

    for (int i = 0; i < 8; i++) {
        int x = target.x + offsets[i][0];
        int y = target.y + offsets[i][1];

        if (x < 0 || x >= MAX_W || y < 0 || y >= MAX_H)
            continue;
        if (p->shm->grid[y][x] != 0)
            continue;

        t_position candidate = {x, y};
        int dist = manhattan(p->pos, candidate);
        if (best_dist == -1 || dist < best_dist) {
            best_dist = dist;
            best = candidate;
        }
    }
    return best;
}

int get_dir(t_player *p, t_position target) {

    if (target.x == p->pos.x && target.y == p->pos.y)
        return 0;


    int x_dif = target.x - p->pos.x ;
    int y_dif = target.y - p->pos.y;
    if (x_dif == 0) {
        if (y_dif < 0) { return 2; } else { return 4;}
    } else if (y_dif == 0) {
        if (x_dif < 0) { return 1; } else { return 3; }
    } else if (x_dif < 0 && y_dif < 0) {
        if (x_dif < y_dif) { return 1; } else { return 2; }
    } else if (x_dif < 0 && y_dif > 0) {
        if (x_dif < (y_dif * -1)) { return 1; } else {return 4;} 
    } else if (x_dif > 0 && y_dif > 0) {
        if (x_dif < y_dif) { return 3; } else { return 4; }
    } else if (x_dif > 0 && y_dif < 0) {
        if (x_dif < (y_dif * -1)) { return 3; } else { return 2; }
    }
    return 0;
}