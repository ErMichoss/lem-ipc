#ifndef DISPLAY_H
# define DISPLAY_H

#include "lem_ipc.h"
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>

typedef struct s_sdl {
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Texture     *ant[4];    // left, right, up, down
    SDL_Texture     *floor[2];  // floor_a, floor_b
    TTF_Font        *font;
    TTF_Font        *victory_font;
    t_shm           *shm;
    int             shmid;
    int             semid;
    int             msqid;
} t_sdl;

/* ___ INIT_IPC ___ */
void ipc_init_display(t_sdl *sdl);
void init_msgq_display(t_sdl *sdl);
void init_sem_display(t_sdl *sdl);
void init_shm_display(t_sdl *sdl);

/* ___ MESSAGES ___ */
int recv_msg_display(t_sdl *sdl, t_message *msg, int mtype);

#endif