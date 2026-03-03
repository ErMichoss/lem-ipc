#include "lem_ipc.h"

void init_shm(t_player *p){
    int shmid;

    shmid = shmget(SHM_KEY, 0, 0666);
    if (shmid == -1) {
        //primer proceso crear todo
        shmid = shmget(SHM_KEY, sizeof(t_shm), IPC_CREAT|0666);
        if (shmid == -1) {
            printf("Error in shmget\n");
            return; 
        }
        
        void* shm = shmat(shmid, NULL, 0);
        if (shm == (void *)-1) { 
            printf("Error in shmat\n");
            shmctl(shmid, IPC_RMID, NULL);
            return; 
        }

        ft_memset(shm, 0, sizeof(t_shm));
        p->shm = shm;
    } else {
        //ya esta el chiringuito montado conectarme
        void* shm = shmat(shmid, NULL, 0);
        if (shm == (void *)-1) { 
            printf("Error in shmat\n");
            shmctl(shmid, IPC_RMID, NULL);
            return; 
        }
        p->shm = shm;
    }
}