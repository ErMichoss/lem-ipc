#include "display.h"

SDL_Color get_team_color(int team_id) {
    SDL_Color color;
    color.r = (team_id * 73)  % 256;
    color.g = (team_id * 151) % 256;
    color.b = (team_id * 211) % 256;
    color.a = 255;
    return color;
}

void sdl_init(t_sdl* sdl) {
    SDL_Surface *surf;
    
    SDL_Init(SDL_INIT_VIDEO);
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        printf("IMG_Init error: %s\n", IMG_GetError());
    TTF_Init();

    sdl->window = SDL_CreateWindow("lem_ipc", 0, 0, MAX_W * 64, MAX_H * 64, 0);
    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0);
    
    surf = IMG_Load("assets/floor_a_64.png");
    sdl->floor[0] = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("assets/floor_b_64.png");
    sdl->floor[1] = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("assets/ant_left_64.png");
    sdl->ant[0] = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("assets/ant_up_64.png");
    sdl->ant[1] = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("assets/ant_right_64.png");
    sdl->ant[2] = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("assets/ant_down_64.png");
    sdl->ant[3] = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    sdl->font = TTF_OpenFont("assets/Roboto-Regular.ttf", 12);
    sdl->victory_font = TTF_OpenFont("assets/Monoton-Regular.ttf", 32);
}

void draw_floor(t_sdl *sdl) {
    for (int y = 0; y < MAX_H; y++){
        for (int x = 0; x < MAX_W; x++){
            SDL_Rect rect = { x * 64, y * 64, 64, 64 };
            int floor_idx = (x + y) % 2;
            SDL_RenderCopy(sdl->renderer, sdl->floor[floor_idx], NULL, &rect);
        }
    }
}

void draw_players(t_sdl *sdl) {
    for (int y = 0; y < MAX_H; y++){
        for (int x = 0; x < MAX_W; x++){
            if (sdl->shm->grid[y][x] != 0) {
                SDL_Rect rect = { x * 64, y * 64, 64, 64 };
                SDL_Color color = get_team_color(sdl->shm->grid[y][x]);

                int dir = sdl->shm->dirs[y][x];

                SDL_SetTextureColorMod(sdl->ant[dir], color.r, color.g, color.b);
                SDL_RenderCopy(sdl->renderer, sdl->ant[dir], NULL, &rect);
                SDL_SetTextureColorMod(sdl->ant[dir], 255, 255, 255);
                
                char *num = ft_itoa(sdl->shm->grid[y][x]);

                SDL_Color white = {255, 255, 255, 255};
                SDL_Surface *surf = TTF_RenderText_Solid(sdl->font, num, white);

                SDL_Texture *tex = SDL_CreateTextureFromSurface(sdl->renderer, surf);
                SDL_FreeSurface(surf);

                int tw, th;
                SDL_QueryTexture(tex, NULL, NULL, &tw, &th);  // obtener tamaño del texto
                SDL_Rect text_rect = {
                    x * 64 + (64 - tw) / 2,
                    y * 64 + (64 - th) / 2,
                    tw, th
                };

                SDL_RenderCopy(sdl->renderer, tex, NULL, &text_rect);
                SDL_DestroyTexture(tex);
                free(num);
            }
        }
    }
}

void draw_victory(t_sdl *sdl, int team) {
    
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 235);
    SDL_Rect overlay = {0, 0, MAX_W * 64, MAX_H * 64};
    SDL_RenderFillRect(sdl->renderer, &overlay);

    SDL_Color color = get_team_color(team);

    char text[64];
    snprintf(text, 64, "TEAM   %d   WINS!", team);
    SDL_Surface *surf = TTF_RenderText_Blended(sdl->victory_font, text, color);

    SDL_Texture *tex = SDL_CreateTextureFromSurface(sdl->renderer, surf);
    SDL_FreeSurface(surf);

    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    SDL_Rect text_rect = {
        (MAX_W * 64 - tw) / 2,
        (MAX_H * 64 - th) / 2,
        tw, th
    };

    SDL_RenderCopy(sdl->renderer, tex, NULL, &text_rect);
    SDL_DestroyTexture(tex);
}

void draw_board(t_sdl *sdl) {
    SDL_RenderClear(sdl->renderer);
    draw_floor(sdl);
    draw_players(sdl);
    SDL_RenderPresent(sdl->renderer);
}

void sdl_loop(t_sdl *sdl) {
    printf("player_count: %d\n", sdl->shm->player_count);
    while (sdl->shm->player_count > 0 && sdl->shm->winner == 0) {
        sem_lock(sdl->semid);
        draw_board(sdl);
        sem_unlock(sdl->semid);
        
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {  // usuario cierra la ventana
                return;
            }
        }
        usleep(200000);
    }

    if (sdl->shm->winner != 0) {
        draw_victory(sdl, sdl->shm->winner);
        SDL_RenderPresent(sdl->renderer);
        
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
                return;
        }
    }
}

void sdl_cleanup(t_sdl *sdl) {
    for (int i = 0; i < 4; i++)
        if (sdl->ant[i]) SDL_DestroyTexture(sdl->ant[i]);
    for (int i = 0; i < 2; i++)
        if (sdl->floor[i]) SDL_DestroyTexture(sdl->floor[i]);
    if (sdl->font) TTF_CloseFont(sdl->font);
    if (sdl->renderer) SDL_DestroyRenderer(sdl->renderer);
    if (sdl->window) SDL_DestroyWindow(sdl->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    if (sdl->shm) shmdt(sdl->shm);
}

int main(void) {
    t_sdl sdl;

    ipc_init_display(&sdl);
    sdl_init(&sdl);
    if (!sdl.floor[0])
        printf("Error loading floor_a: %s\n", IMG_GetError());
    sdl_loop(&sdl);
    sdl_cleanup(&sdl);
    return 0;
}