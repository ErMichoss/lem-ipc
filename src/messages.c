#include "lem_ipc.h"

void send_msg(t_player *p, int event, t_position target) {
    t_message msg;
    msg.mtype = p->team_id;
    if (event == DEATH)
        msg.mtype = MAX_TEAMS + 1;
    else if (event == TARGET)
        msg.mtype = p->team_id + MAX_TEAMS + 1;
    else
        msg.mtype = p->team_id;
    msg.pos = p->pos;
    msg.target = target;
    msg.event = event;

    if (msgsnd(p->msqid, &msg, sizeof(t_message) - sizeof(long), 0) == -1)
        printf("Error in msgsnd File: %s - Line: %d\n", __FILE__, __LINE__);
}

int recv_msg(t_player *p, t_message *msg, int mtype) {
    return msgrcv(p->msqid, msg, sizeof(t_message) - sizeof(long), mtype, IPC_NOWAIT);
}