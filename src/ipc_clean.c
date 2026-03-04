#include "lem_ipc.h"

void ipc_clean(t_player *p) {
    printf("REMOVING PLAYER %i\n", p->number);

    sem_lock(p->semid);
    int player_count = --(p->shm->player_count);
    sem_unlock(p->semid);

    shmdt(p->shm);

    if (player_count < 1) {
        msgctl(p->msqid, IPC_RMID, NULL);
        semctl(p->semid, 0, IPC_RMID);
        shmctl(p->shmid, IPC_RMID, NULL);
    }
}