#include <stdio.h>
#include <assert.h>

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

unsigned p_r(unsigned a, unsigned b, char *s)
{
    static unsigned ind = 0;
    unsigned res;

    assert(a > 0);
    printf("%*s%sp_r(%d,%d) = (\n", ind, "", s, a, b);
    ind += 2;

    if (b == 0) {
        res = 1;
        printf("%*s/* b == 0 */\n", ind, "");
        printf("%*s1\n", ind, "");
        while (--a > ++b) {
            res += p_r(a, b, "+ ");
        } /* while */
        if (a > 0)
            res += p_r(a, b, "+ ");
    } else if (a > b) { /* b > 0 */
        printf("%*s/* a > b, b > 0 */\n", ind, "");
        res = p_r(b, 0, "");
    } else { /* b > 0, a <= b */
        unsigned c = a;
        printf("%*s/* a <= b, b > 0 */\n", ind, "");
        res = 0;
        while (a > 0) {
            res += p_r(a, b-a, "");
            a--;
        } /*  while */
        a = c;
        while (++b, --a > 0) {
            res += p_r(a, b, res ? "+ " : "");
        }
    } /* if */

    ind -= 2;
    printf("%*s) --> %d\n", ind, "", res);

    return res;
} /* p */

unsigned p(unsigned a)
{
    unsigned b = 0;
    res = 0;
    while (a > 0)
        res += p_r(a--, b++, res ? "+ " : "");
    return res;
}

int main(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; i++) {
        unsigned n = atol(argv[i]);
        printf("\033[32mp(%d,0)\033[0m = %d\n",
                n, p_r(n, 0, ""));
    }
    return 0;
}
