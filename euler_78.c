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
#include <getopt.h>
#include "euler_78_cache.h"

#define MOD     1000000

#ifndef DEBUG
#define DEBUG 0
#endif

unsigned ind = 0;
unsigned mod = MOD;
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

    cdat = lookup_q(a, b);

    if (cdat) {
        if (s) printf(P("/* * * CACHED!!! * * */\n"));
        res = cdat->res;
    } else if (b <= 1) {
        res = 1;
    } else if (a > b) {
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
            res %= mod;
        }
    } /* if */
    if (!cdat) {
        add_q(_a, _b, res);
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
            res %= mod;
        } /* for */
    } /* if */

    if (!cdat) add_p(_a, res);

    ind--;
    if (s) printf(P(") ==> %u\n"), res);

    return res;
} /* p */

int main(int argc, char **argv)
{
    int i, opt;
    char *tr = NULL;
    unsigned arg = 0, res = 1;

    while ((opt = getopt(argc, argv, "vi:m:")) != EOF) {
        switch(opt) {
        case 'v': tr = ""; break;
        case 'i': arg = atoi(optarg); break;
        case 'm': mod = atoi(optarg); break;
        } /* switch */
    } /* while */

    do {
        res = p(++arg, tr);
        printf("p(%u) ~= %u (mod %d)\n",
                arg, res, mod);
    } while (res);

    return 0;
} /* main */
