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

#include <stdlib.h>
#include <assert.h>
#include <avl.h>
#include "euler_78_cache.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#   define DEB(args...) printf(args)
#   define D(x) __FILE__":%d:%s: " x, __LINE__, __func__
#else
#   define DEB(args...)
#endif

static AVL_TREE data_p = NULL;
static AVL_TREE data_q = NULL;

static int q_cmp(const void *_a, const void *_b)
{
    struct data_q *a = (struct data_q *)_a,
                  *b = (struct data_q *)_b;
    return a->a == b->a ? a->b - b->b : a->a - b->a;
} /* q_cmp */

static int p_cmp(const void *_a, const void *_b)
{
    struct data_p *a = (struct data_p *)_a,
                  *b = (struct data_p *)_b;
    return a->n - b->n;
} /* p_cmp */

struct data_p *lookup_p(unsigned n)
{
    struct data_p key;

    if (!data_p) return NULL;

    key.n = n;
    return avl_tree_get(data_p, &key);
} /* lookup_p */

struct data_q *lookup_q(unsigned a, unsigned b)
{
    struct data_q key;

    if (!data_q) return NULL;

    key.a = a;
    key.b = b;
    return avl_tree_get(data_q, &key);
} /* lookup_q */

void add_p(unsigned n, unsigned res)
{
    struct data_p *p;

    if (!data_p) {
        assert(data_p = new_avl_tree(p_cmp, NULL, NULL, NULL));
    } /* if */
    assert(p = malloc(sizeof (struct data_p)));
    p->n = n;
    p->res = res;
    DEB(D("(n=%u, res=%u)\n"), n, res);
    avl_tree_put(data_p, p, p);
} /* add_p */

void add_q(unsigned a, unsigned b, unsigned res)
{
    struct data_q *q;

    if (!data_q) {
        assert(data_q = new_avl_tree(q_cmp, NULL, NULL, NULL));
    } /* if */
    assert(q = malloc(sizeof (struct data_q)));
    q->a = a;
    q->b = b;
    q->res = res;
    DEB(D("(a=%u, b=%u, res=%u)\n"), a, b, res);
    avl_tree_put(data_q, q, q);
} /* add_q */

void cache_print(void)
{
    AVL_ITERATOR i;
    DEB(D("CACHE p(n)\n"));
    for (i = avl_tree_first(data_p); i; i = avl_iterator_next(i)) {
        struct data_p *p = avl_iterator_data(i);
        DEB(D("p(n=%u) => %u\n"), p->n, p->res);
    } /* for */
    DEB(D("CACHE q(a,b):\n"));
    for (i = avl_tree_first(data_q); i; i = avl_iterator_next(i)) {
        struct data_q *q = avl_iterator_data(i);
        DEB(D("q(a=%u, b=%u) => %u\n"), q->a, q->b, q->res);
    } /* for */
} /* cache_print */
