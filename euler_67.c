/* Maximum path sum II
 * Problem 67
 * By starting at the top of the triangle below and moving to adjacent numbers on the row
 * below, the maximum total from top to bottom is 23.
 *
 *    [3]
 *   [7] 4
 *  2 [4] 6
 * 8 5 [9] 3
 *
 * That is, 3 + 7 + 4 + 9 = 23.
 *
 * Find the maximum total from top to bottom in triangle.txt (right click and 'Save
 * Link/Target As...'), a 15K text file containing a triangle with one-hundred rows.
 *
 * NOTE: This is a much more difficult version of Problem 18. It is not possible to try
 * every route to solve this problem, as there are 299 altogether! If you could check one
 * trillion (1012) routes every second it would take over twenty billion years to check them
 * all. There is an efficient algorithm to solve it. ;o)
 */

#include <stdio.h>
#include <assert.h>

#define max_height  (256)
#define N           (max_height*(max_height-1)/2)

#define D(X) __FILE__":%d:%s:" X, __LINE__, __func__

struct cell {
    unsigned char   flg;
    unsigned char   val;
    unsigned short  sum;
};

struct cell array[N+1];
struct cell *lines[max_height];

int main()
{
    int i = 0, j = 0, n = 0;
    int nlines;
    struct cell *p = array;
    unsigned val;
    short max = 0;

    /* read data */
    while (scanf("%u", &val) == 1) {
        p->val = val;
        if (i == 0 && j == 0) { /* top of the tree */
            lines[0] = p;
            p->flg = 0x00;
            p->sum = p->val;
        } else if (j == 0) { /* leftmost cell, parent is right */
            lines[i] = p;
            p->flg = 0x01;
            p->sum = p->val + (p - i)->sum; /* this is the parent right */
        } else if (j == i) { /* rightmost cell */
            p->flg = 0x02;
            p->sum = p->val + (p - i - 1)->sum; /* this is the parent left */
        } else { /* inner node */
            p->flg = 0x00;
            if ((p-i-1)->sum >= (p-i)->sum)
                p->flg |= 0x02; /* parent left is a parent */
            if ((p-i-1)->sum <= (p-i)->sum)
                p->flg |= 0x01; /* parent right is a parent */
            p->sum = p->flg & 0x01 /* compute sum */
                ? p->val + (p-i)->sum
                : p->val + (p-i-1)->sum;
        } /* if */
        j++;
        if (j > i) {
            j = 0; i++;
        } /* if */
        p++; n++;
        assert(n <= N);
    } /* while */

    if (j) {
        fprintf(stderr,
                D("line incomplete, completing with 0 values\n"));
        while (j < i) {
            p->val = 0;
            p->flg = 0x00;
            if ((p-i-1)->sum >= (p-i)->sum)
                p->flg |= 0x02;
            if ((p-i-1)->sum <= (p-i)->sum)
                p->flg |= 0x01;
            p->sum = p->flg & 0x01
                ? (p-i)->sum
                : (p-i-1)->sum;
            j++;
            if (j > i) {
                j = 0;
                i++;
            } /* if */
            p++; n++;
        } /* while */
        /* j == i */
        p->flg = 0x02;
        p->sum = p->val + (p - i - 1)->sum;
        j = 0;
        i++; p++; n++;
    } /* if */
    nlines = i;

    /* now we have n complete lines, get the max */
    for (i = 0; i < nlines; i++) {
        if (lines[nlines-1][i].sum > max) {
            max = lines[nlines-1][i].sum;
            j = i;
        }
    } /* for */
    printf(D("max = %hd\n"), max);

    /* get the path back to the root */
    for (i = 0; i < nlines; i++) /* last line */
        if (lines[nlines-1][i].sum == max)
            lines[nlines-1][i].flg |= 0x04;

    for (i = nlines-1; i; i--) { /* go up to the top */
        for (j = 0; j <= i; j++) {
            if (lines[i][j].flg & 0x04) {
                if (lines[i][j].flg & 0x02) /* has a left parent */
                    lines[i-1][j-1].flg |= 0x04;
                if (lines[i][j].flg & 0x01) /* has a right parent (or both) */
                    lines[i-1][j].flg |= 0x04;
            } /* if */
        } /* for */
    } /* for */

    for (i = 0; i < nlines; i++) { /* print the triangle */
        printf("%*s", 2*(nlines-i), "");
        for (j = 0; j <= i; j++) {
            char *fmt;
            if (lines[i][j].flg & 0x04)
                fmt = "%s\033[7m%02d\033[0m";
            else
                fmt = "%s%02d";
            printf(fmt, j ? ", " : "", lines[i][j].val);
        } /* for */
        puts("");
    } /* for */
} /* main */

