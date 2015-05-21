/* locking.h -- resource locking for multithread access.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: jue may 21 08:08:16 EEST 2015
 */

#ifndef _LOCKING_H
#define _LOCKING_H

struct l_stat {
    size_t readers_counter;
    size_t writers_counter;
};

typedef struct l_lock_s L_LOCK, *L_HNDL;

L_HNDL l_new();

void l_init(L_HNDL h);

void l_rd_acq(L_HNDL r);
void l_wr_acq(L_HNDL r);
void l_rd_rel(L_HNDL r);
void l_wr_rel(L_HNDL r);

struct l_stat *l_stat_get(L_LOCK r);
void l_stat_reset(L_LOCK r);

#endif /* _LOCKING_H */
