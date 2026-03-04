#include "lem_ipc.h"

void init_shm(t_player *p) {
    int shmid;

    shmid = shmget(SHM_KEY, 0, 0666);
    if (shmid == -1) {
        //primer proceso crear todo
        shmid = shmget(SHM_KEY, sizeof(t_shm), IPC_CREAT|0666);
        if (shmid == -1) {
            printf("Error in shmget (init_shm) File: %s - Line: %d\n", __FILE__, __LINE__);
            return; 
        }
        
        void* shm = shmat(shmid, NULL, 0);
        if (shm == (void *)-1) { 
            printf("Error in shmat (init_shm) File: %s - Line: %d\n", __FILE__, __LINE__);
            shmctl(shmid, IPC_RMID, NULL);
            return; 
        }

        ft_memset(shm, 0, sizeof(t_shm));
        p->shm = shm;
        p->shmid = shmid;
    } else {
        //ya esta el chiringuito montado conectarme
        void* shm = shmat(shmid, NULL, 0);
        if (shm == (void *)-1) { 
            printf("Error in shmat (init_shm) File: %s - Line: %d\n", __FILE__, __LINE__);
            return; 
        }
        p->shm = shm;
        p->shmid = shmid;
    }
}

void init_sem(t_player *p) {
    int semid;

    semid = semget(SEM_KEY, 0, 0666);
    if (semid == -1) {
        semid = semget(SEM_KEY, 1, IPC_CREAT|0666);
        if (semid == -1) {
            printf("Error in semget (init_sem) File: %s - Line: %d\n", __FILE__, __LINE__);
            return;
        }
        semctl(semid, 0, SETVAL, 1); //Iniciar el semaforo a 1. -- 1 abierto, 0 cerrado.
    }
    p->semid = semid;
}

void init_msgq(t_player *p) {
    int msqid;

    msqid = msgget(MSGQ_KEY, 0666);
    if (msqid == -1) {
        msqid = msgget(MSGQ_KEY, IPC_CREAT | 0666);
        if (msqid == -1) {
            printf("Error in msgget (init_msgq) File: %s - Line: %d\n", __FILE__, __LINE__);
            return;
        }
    }
    p->msqid = msqid;
}

int sem_lock(int semid) {
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;

    return semop(semid, &buf, 1);
}

int sem_unlock(int semid) {
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;

    return semop(semid, &buf, 1);
}

void ipc_init(t_player *p) {
    init_shm(p);
    if (p->shm == (void*)-1) {
        printf("Error in init_shm (ipc_init) File: %s - Line: %d\n", __FILE__, __LINE__);
        return;
    }
    init_sem(p);
    if (p->semid == -1) {
        printf("Error in init_sem (ipc_init) File: %s - Line: %d\n", __FILE__, __LINE__);
        shmctl(p->shmid, IPC_RMID, NULL);
        return;
    }
    init_msgq(p);
    if (p->msqid == -1) {
        printf("Error in init_msq (ipc_init) File: %s - Line: %d\n", __FILE__, __LINE__);
        shmctl(p->shmid, IPC_RMID, NULL);
        return;
    }
}