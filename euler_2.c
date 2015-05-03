#include <stdio.h>
#include <stdint.h>

int main()
{
    uint64_t a = 1, b = 2;
    int i;
    uint64_t acum = 0;

    for (i = 0; b < 4000000; i++) {
        uint64_t c = a + b;
        if (i % 3 == 0) {
            acum += b;
        }
        a = b; b = c;
    }
    printf("%lld\n", acum);
}
