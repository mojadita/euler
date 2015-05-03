#include <stdio.h>
#include <assert.h>

unsigned p(unsigned a, unsigned b, char *s)
{
    static unsigned ind = 0;
    unsigned res;

    assert(a > 0);
    printf("%*s%sp(%d,%d) = (\n", ind, "", s, a, b);
    ind += 2;

    if (a == 1) { /* p(1, b) => 1 */
        res = 1;
    } else if (b == 0) { /* a > 1 && b == 0 */
        printf("%*s/* b == 0 */\n", ind, "");
        printf("%*s1\n", ind, "");
        res = 1;
        while(--a > 0) {
            res += p(a, ++b, "+ ");
        } /* while */
    } else if (a > b) { /* b > 0 && a > b */
        printf("%*s/* a > b, b > 0 */\n", ind, "");
        res = p(b, 0, "");
    } else { /* a <= b, b > 0 */
        printf("%*s/* a <= b, b > 0 */\n", ind, "");
        res = 0;
        while (a > 0) {
            res += p(a, b-a, res ? "+ " : "");
            a--;
        } /* while */
    } /* if */

    ind -= 2;
    printf("%*s) --> %d\n", ind, "", res);

    return res;
} /* p */


int main(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; i++) {
        unsigned n = atol(argv[i]);
        printf("\033[32mp(%d,0)\033[0m = %d\n", n, p(n, 0, ""));
    }
    return 0;
}
