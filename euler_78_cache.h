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
 *
 * This file implements definitions for a cache of values.
 */

#include <stdio.h>

#define GZIP            "/bin/gzip"
#define ARCHIVE_NAME    ".%s-cache.gz"

struct cache_data {
    unsigned mod;
    unsigned arg;
};

struct data_p {
    unsigned n, res;
};

struct data_q {
    unsigned a, b, res;
};

struct data_p *lookup_p(unsigned n);
struct data_q *lookup_q(unsigned a, unsigned b);

void add_p(unsigned n, unsigned res);
void add_q(unsigned a, unsigned b, unsigned res);

void cache_save(const struct cache_data *cd, const char *n);
void cache_restore(struct cache_data *cd, const char *n);
