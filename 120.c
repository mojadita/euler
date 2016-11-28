#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{

    int opt;
    int n = 15, i, j;

    srandomdev();
    while ((opt = getopt(argc, argv, "n:")) != EOF) {
        switch(opt) {
        case 'n': n = atoi(optarg); break;
        } /* switch */
    } /* while */

    for (i = 1; i <= n; i++) {
        for (j = 0; j < i; j++)
            printf("%s%02ld", j ? " " : "", random() % 100);
        puts("");
    }

    return 0;
} /* main */
