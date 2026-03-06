#include "lem_ipc.h"

int main(int argc, char *argv[]) {
    //Validar args
    if (!check_args(argc, argv)) return 1;
    
    srand(time(NULL) ^ getpid());
    
    int team_id = ft_atoi(argv[1]);
    t_position pos;
    pos.x = 0;
    pos.y = 0;

    t_player p;
    p.alive = true;
    p.team_id = team_id;
    p.pos = pos;
    p.pid = getpid();
    p.victory = false;

    ipc_init(&p);
    put_player(&p);
    p.number = p.shm->player_count;
    player_loop(&p);

    return 0;
}