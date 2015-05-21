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
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <avl.h>
#include "euler_78_cache.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#   define DEB(args...) printf(args)
#else
#   define DEB(args...)
#endif
#define D(x) __FILE__":%d:%s: " x, __LINE__, __func__


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

static const char *basename(const char * const n)
{
    const char *nn = n;

    nn = strrchr(n, '/');
    if (nn) nn++;
    else    nn = n;

    return nn;
} /* basename */

static FILE *popen_cache_file(
        char *buffer,
        size_t bs,
        const char *mode,
        const char *fmt,
        ...)
{
    va_list p;
    FILE *res;

    va_start(p, fmt);
    vsnprintf(buffer, bs, fmt, p);
    res = popen(buffer, mode);
    fprintf(stderr,
            D("popen(\"%s\", \"%s\") ==> %p\n"),
            buffer, mode, res);

    return res;
} /* popen_cache_file */

void cache_save(const struct cache_data *cd, const char *n)
{
    char buffer[1000];
    FILE *o;
    AVL_ITERATOR i;
    size_t len = 0;

    n = basename(n);
    DEB(D("saving state to '" ARCHIVE_NAME "'\n"), n, cd->mod);
    o = popen_cache_file(buffer, sizeof buffer,
            "w",
            GZIP " > '" ARCHIVE_NAME "'",
            n, cd->mod);
    if (!o) {
        fprintf(stderr,
                D("%s: %s(errno=%d)\n"),
                buffer, strerror(errno), errno);
        return;
    } /* if */

    len += fprintf(o, "%u\n%u\n%u\n",
            cd->mod, cd->arg, avl_tree_size(data_p));
    for (i = avl_tree_first(data_p); i; i = avl_iterator_next(i)) {
        struct data_p *p = avl_iterator_data(i);
        DEB(D("p(n=%u) => %u\n"), p->n, p->res);
        len += fprintf(o, "%u\t%u\n", p->n, p->res);
    } /* for */
    len += fprintf(o, "\n%u\n", avl_tree_size(data_q));
    for (i = avl_tree_first(data_q); i; i = avl_iterator_next(i)) {
        struct data_q *q = avl_iterator_data(i);
        DEB(D("q(a=%u, b=%u) => %u\n"), q->a, q->b, q->res);
        len += fprintf(o, "%u\t%u\t%u\n", q->a, q->b, q->res);
    } /* for */
    DEB(D("written %ld bytes\n"), len);
    pclose(o);

} /* cache_save */

#define READ(n, fmt, args...) do { \
        if (fscanf(o, fmt, args) != (n)) { \
            fprintf(stderr, D("syntax error: " #args "\n")); \
            return; \
        } /* if */ \
    } while(0)

void cache_restore(struct cache_data *cd, const char *n)
{
    char buffer[1000];
    FILE *o;
    int nentries, i;
    int res;

    n = basename(n);
    DEB(D("loading state from \"" ARCHIVE_NAME "\"\n"),
            n, cd->mod);
    o = popen_cache_file(buffer, sizeof buffer,
            "r",
            GZIP " -d < '" ARCHIVE_NAME "'",
            n, cd->mod);
    if (!o) {
        fprintf(stderr,
                D("%s: %s(errno=%d)\n"),
                buffer, strerror(errno), errno);
        return;
    } /* if */

    READ(3, "%u%u%u", &cd->mod, &cd->arg, &nentries);
    for (i = 0; i < nentries; i++) {
        unsigned p_n, p_res;
        READ(2, "%u%u", &p_n, &p_res);
        add_p(p_n, p_res);
    } /* for */

    READ(1, "%u", &nentries);
    for (i = 0; i < nentries; i++) {
        unsigned q_a, q_b, q_res;

        READ(3, "%u%u%u", &q_a, &q_b, &q_res);
        add_q(q_a, q_b, q_res);
    } /* for */

    fclose(o);

    fprintf(stderr, D("state loaded\n"));
} /* cache_restore */
