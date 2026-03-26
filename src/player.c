#include "lem_ipc.h"

bool am_i_dead(t_player *p) {
    int offsets[8][2] = {
        {-1,-1}, {0,-1}, {1,-1},
        {-1, 0},         {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };
    int enemies[MAX_TEAMS];
    int x;
    int y;
    int box;

    ft_memset(enemies, 0, sizeof(enemies));

    sem_lock(p->semid);
    for (int i = 0; i < 8; i++) {
        x = p->pos.x + offsets[i][0];
        y = p->pos.y + offsets[i][1];

        if (x < 0 || x >= MAX_W || y < 0 || y >= MAX_H)
            continue;

        box = p->shm->grid[y][x];
        if (box != 0 && box != p->team_id)
            enemies[box]++;
    }
    sem_unlock(p->semid);

    for (int j = 0; j < MAX_TEAMS; j++) {
        if (enemies[j] >= 2) {
            p->alive = false;
            return true;
        }
    }
    return !p->alive;
}

void player_loop(t_player *p) {
    while (!am_i_dead(p)) {
        t_message msg;
        t_position target;
        if (recv_msg(p, &msg, p->team_id + MAX_TEAMS + 1) != -1) {
            target = msg.target;
        } else {
            // Decidir a donde voy
            /*
                Pasos para desarrollar un jugador coherente.
                1. Jugador es tonto -> se mueve aleatorio
                2. Una vez funcione el 1 implementar que vaya a por un jugador cercano
                3. Una vez funciones el 2 implementar que lea los mensajes para que se cordinee con su equipo.
                4. Una vez funcion todo, volverlo inteligente para que analice riesgo y recompensa
            */
            target = find_enemy(p);
            send_msg(p, TARGET, target);
        }
        int dir = get_dir(p, target);
        if (move_player(p, dir) != 0) {
            if (dir == 1 || dir == 3){
                if (move_player(p, 2) != 0)
                    move_player(p, 4);
            } else {
                if (move_player(p, 1) != 0)
                    move_player(p, 3);
            }
        }
        display_board(p);
        send_msg(p, MOVE, (t_position){-1, -1});
        
        while (recv_msg(p, &msg, p->team_id) != -1);
        while (recv_msg(p, &msg, p->team_id + MAX_TEAMS + 1) != -1);
        while (recv_msg(p, &msg, MAX_TEAMS + 1) != -1);

        //dormir un ratito
        usleep(200000);
    }

    send_msg(p, DEATH, (t_position){-1, -1});
    remove_player(p);
}

