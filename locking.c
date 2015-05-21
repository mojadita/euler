/* locking.c -- resource locking for multithread access.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com
 * Date: jue may 21 08:07:03 EEST 2015
 */

struct block {
    ptread_cond_t   access; /* to access this structure */

    size_t          readers; /* number of actual readers */
    size_t          readers_counter;
    pthread_cond_t  readers_q; /* queue to wait for reading */

    size_t          writers; /* number of demanding writers
                              * (including the one writing now,
                              * if state is writing */
    size_t          writers_counter;
    pthread_cond_t  writers_q; /* queue to wait for writing */

    enum {
        st_idle,
        st_reading,
        st_writing,
    }               state; /* state of resource */

    AVL_TREE        data;
};

static void read_lock(struct block *b)
{
    pthread_cond_wait(b->access);
    while (b->state == st_writing) {
        pthread_cond_signal(b->access);
        pthread_con_wait(b->readers_q);
        pthread_cond_wait(b->access);
    } /* while */
    if (b->state == st_idle) {
        b->state = st_reading;
    } /* if */
    b->readers++;
    pthread_cond_signal(b->access);
}

static void read_unlock(struct block *b)
{
    pthread_cond_wait(b->access);
    assert(b->state == st_reading);
    b->readers--;
    if (!b->readers) {
        b->state = st_idle;
        if (b->writers) {
            pthread_cond_signal(b->writers_q);
        } /* if */
    } /* if */
    b->readers_counter++;
    pthread_cond_signal(b->access);
}

static void write_lock(struct block *b)
{
    pthread_cond_wait(b->access);
    b->writers++;
    while (b->state != st_idle) {
        pthread_cond_signal(b->access);
        pthread_cond_wait(b->writers_q);
        pthread_cond_wait(b->access);
    } /* while */
    b->state = st_writing;
    pthread_cond_signal(b->access);
}

static void write_unlock(struct block *b)
{
    pthread_cond_wait(b->access);
    b->writers--;
    b->state = st_idle;
    if (b->writers) {
        pthread_cond_signal(b->qriters_q);
    } else if (b->readers) {
        pthread_cond_broadcast(b->readers_q);
    } /* if */
}


