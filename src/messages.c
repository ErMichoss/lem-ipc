#include "lem_ipc.h"

void send_msg(t_player *p, int event) {
    t_message msg;
    msg.mtype = p->team_id;
    if(event == DEATH)
        msg.mtype = MAX_TEAMS + 1;
    msg.team = p->team_id;
    msg.pos = p->pos;
    msg.event = event;

    if (msgsnd(p->msqid, &msg, sizeof(t_message) - sizeof(long), 0) == -1)
        printf("Error in msgsnd File: %s - Line: %d\n", __FILE__, __LINE__);
}

int recv_msg(t_player *p, t_message *msg) {
    int res;
    //primero los mensajes globales
    res = msgrcv(p->msqid, msg, sizeof(t_message) - sizeof(long), MAX_TEAMS + 1, IPC_NOWAIT);
    if (res > -1) return res;
    //luego los mensajes del equipo
    res = msgrcv(p->msqid, msg, sizeof(t_message) - sizeof(long), p->team_id, IPC_NOWAIT);
    if (res > -1) return res;
    return -1;
}