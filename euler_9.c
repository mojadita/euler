/*
 * Project Euler #9
 * A Pythagorean triplet is a set of three natural numbers,
 * a < b < c, for which, a2 + b2 = c2
 *
 * For example, 32 + 42 = 9 + 16 = 25 = 52.
 *
 * There exists exactly one Pythagorean triplet for which
 * a + b + c = 1000.
 * Find the product abc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{
    int a, b, c, opt, size = 1000;
    while ((opt = getopt(argc, argv, "s:")) != EOF) {
        switch (opt) {
        case 's': size = atoi(optarg); break;
        } /* switch */
    } /* while */


    for (a = 1; a < size; a++)
        for (b = a+1; b < size; b++)
            for (c = b+1; c < size; c++)
                if (a + b + c == size && a*a + b*b == c*c)
                    goto out;
    exit(EXIT_FAILURE);
out:
    printf("a=%d, b=%d, c=%d, abc=%d (size=%d)\n", a, b, c, a*b*c, size);
    exit(EXIT_SUCCESS);
} /* main */
