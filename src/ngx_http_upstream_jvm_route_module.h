typedef struct {
    ngx_uint_t                          nreq; /* active requests to the peer */
    ngx_uint_t                          total_req;
    ngx_uint_t                          last_req_id;
    ngx_uint_t                          fails;
    ngx_uint_t                          total_fails;
    time_t                              accessed;
    ngx_int_t                           current_weight;
} ngx_http_upstream_jvm_route_shared_t;

typedef struct {
    ngx_http_upstream_jvm_route_shared_t *shared;
    struct sockaddr                *sockaddr;
    socklen_t                       socklen;
    ngx_str_t                       name;

    ngx_int_t                       weight;
    ngx_uint_t                      max_fails;
    ngx_uint_t                      max_busy;
    time_t                          fail_timeout;
    ngx_uint_t                      down;          /* unsigned  down:1; */
    ngx_str_t                       srun_id;
#if (NGX_HTTP_UPSTREAM_CHECK)
    ngx_uint_t                      check_index;
#endif

#if (NGX_HTTP_SSL)
    ngx_ssl_session_t              *ssl_session;   /* local to a process */
#endif
} ngx_http_upstream_jvm_route_peer_t;

typedef struct ngx_http_upstream_jvm_route_peers_s ngx_http_upstream_jvm_route_peers_t;

typedef struct {
    ngx_uint_t                           generation;
    ngx_http_upstream_jvm_route_peers_t *peers;
    ngx_uint_t                           total_nreq;
    ngx_uint_t                           total_requests;
    ngx_atomic_t                         lock;
    ngx_http_upstream_jvm_route_shared_t stats[1];
} ngx_http_upstream_jvm_route_shm_block_t;

struct ngx_http_upstream_jvm_route_peers_s {
    /* data should be shared between processes */
    ngx_http_upstream_jvm_route_shm_block_t *shared;

    ngx_uint_t                               current;
    ngx_uint_t                               number;
    ngx_str_t                               *name;
    ngx_str_t                                shm_name;

    /* for backup peers support, not really used yet */
    ngx_http_upstream_jvm_route_peers_t     *next;

    ngx_http_upstream_jvm_route_peer_t      peer[1];
};
