#ifndef EVENT_H
#define EVENT_H

#include <sys/epoll.h>
#include "connection.h"

#define E_NONE 0
#define E_WRITABLE 1
#define E_READABLE 2
#define E_ERROR 4

struct event_loop {
    int epfd;
    struct epoll_event *events;
    int nevent;
};

int event_init(struct event_loop *loop, int nevent);
void event_free(struct event_loop *loop);
int event_register(struct event_loop *loop, struct connection *c, int mask);
int event_reregister(struct event_loop *loop, struct connection *c, int mask);
int event_deregister(struct event_loop *loop, struct connection *c);
int event_wait(struct event_loop *loop, int timeout);

#endif /* end of include guard: EVENT_H */
