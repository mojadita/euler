#include <stdio.h>
#include <stdint.h>

/* project Euler #2
 * Each new term in the Fibonacci sequence is generated by adding
 * the previous two terms. By starting with 1 and 2, the first 10
 * terms will be:
 *
 * 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...
 *
 * By considering the terms in the Fibonacci sequence whose values
 * do not exceed four million, find the sum of the even-valued
 * terms.
 */

int main()
{
    uint64_t a = 1, b = 2;
    int i, n = 0;
    uint64_t acum = 0;

    for (i = 0; b < 4000000; i++) {
        uint64_t c = a + b;
        if (i % 3 == 0) {
            printf("%s%d", n++ ? "+" : "", b);
            acum += b;
        }
        a = b; b = c;
    }
    printf("=%lld\n", acum);
}
