#ifndef SLOWLOG_H
#define SLOWLOG_H

#include <pthread.h>
#include "mbuf.h"
#include "parser.h"


#define SLOWLOG_ENTRY_MAX_ARGC 32
#define SLOWLOG_ENTRY_MAX_STRING 128
#define SLOWLOG_MAX_ARG_LEN 120  // SLOWLOG_ENTRY_MAX_STRING - strlen("%120\r\n\r\n")

// Same with slowlog format of redis.
// Note that slowlog_entry will be created from one thread
// and also held by mutliple reader threads executing 'slowlog get' command.
// It will be freed by the last owner thread.
struct slowlog_entry {
    long long id;
    long long log_time;
    long long remote_latency;
    long long total_latency;
    int refcount;
    int argc;
    struct pos argv[SLOWLOG_ENTRY_MAX_ARGC];
};

struct slowlog_queue {
    struct slowlog_entry **entries;
    pthread_mutex_t *entry_locks;
    size_t capacity;
    size_t curr;
};

struct context;
struct command;

int slowlog_init(struct slowlog_queue *slowlog);
void slowlog_free(struct slowlog_queue *slowlog);

// only called by the thread who creates the log
struct slowlog_entry *slowlog_create_entry(struct command *cmd, int64_t remote_latency, int64_t total_latency);
// create entry for the slowlest sub cmd, its total_latency should be the same as parent cmd
struct slowlog_entry *slowlog_create_sub_entry(struct command *cmd, int64_t total_latency);
// called by all worker threads
void slowlog_set(struct slowlog_queue *queue, struct slowlog_entry *entry);
void slowlog_dec_ref(struct slowlog_entry *entry);
struct slowlog_entry *slowlog_get(struct slowlog_queue *queue, size_t index);
bool slowlog_cmd_enabled();
bool slowlog_type_need_log(struct command *cmd);
bool slowlog_need_log(struct command *cmd, long long latency);

// for statsd
bool slowlog_statsd_enabled();
void slowlog_init_stats();
void slowlog_free_stats();
void slowlog_add_count(struct command *cmd);  // called by worker threads
void slowlog_prepare_stats(struct context *contexts);  // called by stats thread

#endif
