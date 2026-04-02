#ifndef LEM_IPC_H
# define LEM_IPC_H

# ifndef MAX_W
#  define MAX_W 20
# endif

# ifndef MAX_H
#  define MAX_H 20
# endif

# ifndef MAX_TEAMS
#  define MAX_TEAMS 10
# endif

# define SHM_KEY  0x1234
# define MSGQ_KEY 0x5678
# define SEM_KEY  0x9ABC

# include <unistd.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/msg.h>
# include <sys/sem.h> 
# include <time.h>
# include <signal.h>
# include <stdlib.h> 

typedef enum e_event {
    MOVE = 0,
    DEATH,
    VICTORY,
    TARGET
} t_event;

typedef struct s_position {
    int x;
    int y;
} t_position;

typedef struct s_shm {
    int     player_count;
    int     grid[MAX_W][MAX_H];
    int     dirs[MAX_H][MAX_W];
    int     started;
} t_shm;

typedef struct s_player {
    t_shm       *shm;
    pid_t       pid;
    int         team_id;
    int         number;
    t_position  pos;
    bool        alive;
    bool        victory;
    int         shmid;
    int         semid;
    int         msqid;
} t_player;


typedef struct s_message {
    long        mtype;  //tipo de mensaje:
    /*
        1 = mensajes del equipo 1
        2 = mensajes del equipo 2
        MAX_TEAM + 1 = mensajes globales
    */
    int         team;   //equipo emisor
    t_position  pos;    //posición
    t_position  target; //posición del enemigo que persigue
    t_event     event;  //evento
} t_message;

/* ___ AUX FUNCTIONS ___ */
void    *ft_memset(void *str, int c, size_t n);
int     ft_atoi(const char *str);
bool    is_valid_team(const char *str);
bool    check_args(int argc, char *argv[]);
void    ft_putnbr(int n);
void    ft_putchar(char c);
char	*ft_itoa(int n);
int     manhattan(t_position a, t_position b);

/* ___ IPC_INIT ___ */
void    init_shm(t_player *p);
void    init_sem(t_player *p);
void    init_msgq(t_player *p);
int     sem_lock(int semid);
int     sem_unlock(int semid);
void    ipc_init(t_player *p);

/* ___ IPC_CLEAN ___ */
void    ipc_clean(t_player *p);

/* ___ BOARD ___ */
int     put_player(t_player *p);
int     move_player(t_player *p, int dir);
void    remove_player(t_player *p);

/* ___ PLAYER ___ */
bool    am_i_dead(t_player *p);
void    player_loop(t_player *p);

/* ___ MESSAGES ___ */
void    send_msg(t_player *p, int event, t_position target);
int recv_msg(t_player *p, t_message *msg, int mtype);

/* ___ DISPLAY ___ */
void    display_board(t_player *p);

/* ___ BRAIN ___ */
t_position  find_enemy(t_player *p);
int         get_dir(t_player *p, t_position target);

#endif