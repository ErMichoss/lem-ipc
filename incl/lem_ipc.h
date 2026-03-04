#ifndef LEM_IPC_H
# define LEM_IPC_H

# ifndef MAX_W
#  define MAX_W 20
# endif

# ifndef MAX_H
#  define MAX_H 20
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

typedef enum e_event {
    MOVE = 0,
    DEATH,
    VICTORY
} t_event;

typedef struct s_position {
    int x;
    int y;
} t_position;

typedef struct s_shm {
    int     player_count;
    int     grid[MAX_W][MAX_H];
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
        0 = cualquier mensaje
        1 = mensajes del equipo 1
        2 = mensajes del equipo 2
        22 = mensajes globales
    */
    int         team;   //equipo emisor
    t_position  pos;    //posición
    t_event     event;  //evento
} t_message;

/* ___ AUX FUNCTIONS ___ */
void	*ft_memset(void *str, int c, size_t n);
#endif