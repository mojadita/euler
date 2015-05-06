/*
 * Project Euler #78
 * Let p(n) represent the number of different ways in which n
 * coins can be separated into piles. For example, five coins
 * can be separated into piles in exactly seven different ways,
 * so p(5)=7.
 *
 * [OOOOO]
 * [OOOO] [O]
 * [OOO] [OO]
 * [OOO] [O] [O]
 * [OO] [OO] [O]
 * [OO] [O] [O] [O]
 * [O] [O] [O] [O] [O]
 * Find the least value of n for which p(n) is divisible by
 * one million.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include "euler_78_cache.h"

#if _POSIX_TIMERS
#define XSECS_PER_SECOND    1000000000UL
#define NSECFMT             "%09d"
#else
#include <sys/time.h>
#define clock_gettime(a,b) gettimeofday((b), NULL)
#define tv_nsec tv_usec
#define timespec    timeval
#define XSECS_PER_SECOND    1000000UL
#define NSECFMT             "%06d"
#endif


#define MOD     1000000

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#define DEB(args...) do{printf(args);}while(0)
#define TR(arg) do{DEB(D("%s;\n"), #arg); arg;}while(0)
#else
#define DEB(args...)
#define TR(arg) do{arg;}while(0)
#endif
#define D(x) __FILE__":%d:%s:" x, __LINE__, __func__

unsigned ind = 0;

struct cache_data global_state = {
    .mod = 0,
    .arg = 0,
};

unsigned volatile interrupted = 0;

#define P(x) "%*s" x, ind<<2, ""

unsigned p(const unsigned _a, const char * const s);
unsigned q(const unsigned _a, const unsigned _b, const char * const s);

unsigned q(const unsigned _a, const unsigned _b, const char * const s)
{
    unsigned res;
    struct data_q *cdat;
    unsigned a = _a, b = _b;

    if (s) printf(P("%sq(a=%u, b=%u) = (\n"), s, a, b);
    ind++;

    if (b <= 1) {
        /* don't cache these entries
         * (the most frequent) */
        res = 1;
    } else {
        cdat = lookup_q(_a, _b);

        if (cdat) {
            if (s) printf(P("/* * * CACHED!!! * * */\n"));
            res = cdat->res;
        } else {
            /* no cache entry */
            if (a > b) {
                res = p(b, s ? "" : NULL);
            } else { /* a <= b */
                int i = 0;
                res = 0;
                while (a > 0) {
                    if (a > b)
                        res += p(b, s ? i++ ? "+ " : "" : NULL);
                    else
                        res += q(a, b-a, s ? i++ ? "+ " : "" : NULL);
                    a--;
                    res %= global_state.mod;
                }
                add_q(_a, _b, res);
            } /* if */
        } /* if */
    } /* if */

    ind--;
    if (s) printf(P(") ==> %u\n"), res);

    return res;
}

unsigned p(const unsigned _a, const char * const s)
{
    unsigned b;
    unsigned res = 0;
    struct data_p *cdat;
    unsigned a = _a;

    if (s) printf(P("%sp(a=%u) = (\n"), s, a);
    ind++;

    cdat = lookup_p(a);
    if (cdat) {
        if (s) printf(P("/* * * CACHED!!! * * */\n"));
        res = cdat->res;
    } else {
        for (b = 0; a > 0; a--, b++) {
            res += q(a, b, s ? b ? "+ " : "" : NULL);
            res %= global_state.mod;
        } /* for */
        add_p(_a, res);
    } /* if */

    ind--;
    if (s) printf(P(") ==> %u\n"), res);

    return res;
} /* p */

void signal_handler(int s)
{
    interrupted = 1;
} /* signal_handler */

void set_signals()
{
    static struct sigaction sa = {
        .sa_handler = signal_handler,
        .sa_mask = 0,
        .sa_flags = 0,
    };
    int signals[] = {
        SIGINT,
        SIGHUP,
        SIGTERM,
        SIGUSR1,
        SIGUSR2,
    };
    size_t signals_n = sizeof signals / sizeof signals[0];
    int i;

    for (i = 0; i < signals_n; i++) {
        DEB(D("sigaction(%d)\n"), signals[i]);
        if (sigaction(signals[i], &sa, NULL) < 0) {
            fprintf(stderr,
                    D("sigaction: %s(errno=%d)"),
                    strerror(errno), errno);
            continue;
        } /* if */
    } /* for */

} /* set_signals */


int main(int argc, char **argv)
{
    int i, opt;
    char *tr = NULL;
    unsigned res;
    struct timespec t0, t1;

    TR(cache_restore(&global_state, argv[0]));

    while ((opt = getopt(argc, argv, "vi:m:")) != EOF) {
        switch(opt) {
        case 'v': tr = ""; break;
        case 'i': global_state.arg = atoi(optarg); break;
        case 'm': if (global_state.mod) {
                      fprintf(stderr,
                              D("trying to set global_state.mod twice\n"));
                  } else {
                      global_state.mod = atoi(optarg);
                  } /* if */
                  break;
        } /* switch */
    } /* while */

    if (!global_state.mod)
        global_state.mod = MOD;

    TR(set_signals());

    clock_gettime(CLOCK_REALTIME, &t1);
    do {
        int sec, nsec;
        t0 = t1;
        TR(res = p(++global_state.arg, tr));
        clock_gettime(CLOCK_REALTIME, &t1);
        nsec = t1.tv_nsec - t0.tv_nsec;
        sec = t1.tv_sec - t0.tv_sec;
        if (t0.tv_nsec > t1.tv_nsec) {
            nsec += XSECS_PER_SECOND;
            sec--;
        } /* if */
        printf("p(%u) ~= %u (mod %d) (%d." NSECFMT "s)\n",
                global_state.arg, res, global_state.mod,
                sec, nsec);
    } while (res && !interrupted);

    if (interrupted)
        TR(cache_save(global_state.arg, argv[0]));

    return 0;
} /* main */
