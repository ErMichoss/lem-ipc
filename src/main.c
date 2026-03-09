#include "lem_ipc.h"

t_player *g_player = NULL;

void signal_handler(int sig) {
    (void)sig;
    if (g_player) {
        remove_player(g_player);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    //Validar args
    if (!check_args(argc, argv)) return 1;
    
    srand(time(NULL) ^ getpid());

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
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
    g_player = &p;
    player_loop(&p);

    return 0;
}