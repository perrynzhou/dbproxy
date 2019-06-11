#ifndef STATS_H
#define STATS_H

#include <sys/types.h>
#include "socket.h"
#include "slot.h"

struct memory_stats {
    long long buffers;
    long long cmds;
    long long conns;
    long long conn_info;
    long long buf_times;

    long long free_buffers;
    long long free_cmds;
    long long free_conns;
    long long free_conn_info;
    long long free_buf_times;
};

struct basic_stats {
    long long connected_clients;
    long long completed_commands;
    long long slot_update_jobs;
    long long recv_bytes;
    long long send_bytes;

    long long remote_latency;
    long long total_latency;

    long long ask_recv;
    long long moved_recv;
};

struct stats {
    double used_cpu_sys;
    double used_cpu_user;

    long long last_command_latency[MAX_NODE_LIST];
    char remote_nodes[MAX_NODE_LIST * ADDRESS_LEN];

    struct basic_stats basic;
};

int stats_init();
void stats_kill();
int stats_resolve_addr(char *addr);
void stats_get(struct stats *stats);
void stats_get_memory(struct memory_stats *stats);

void incr_slot_update_counter();

#endif /* end of include guard: STATS_H */
