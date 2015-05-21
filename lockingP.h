/* lockingP.h -- private file for locking resources.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: jue may 21 09:35:49 EEST 2015
 */
#ifndef _LOCKINGP_H
#define _LOCKINGP_H

#include <pthread.h>

#include "locking.h"

struct l_lock_s {
    pthread_cond_t  access;     /* to access this structure */
    pthread_cond_t  readers_q;  /* queue to wait for reading */
    pthread_cond_t  writers_q;  /* queue to wait for writing */

    size_t          readers_n;
    size_t          writers_n;
    pthread_t       writer_id;  /* Thread id of actual writer */

    enum {
        st_idle,                /* nobody reading or writing */
        st_reading,             /* readers are reading */
        st_writing,             /* writers are writing */
    }               state;      /* state of resource */
    struct l_stat   stats;      /* statistics */
};

#endif /* _LOCKINGP_H */
