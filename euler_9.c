#include <stdio.h>

int main()
{
    int a, b, c;

    for (a = 1; a < 1000; a++)
        for (b = a+1; b < 1000; b++)
            for (c = b+1; c < 1000; c++)
                if (a + b + c == 1000 && a*a + b*b == c*c)
                    goto out;
out:
    printf("a=%d, b=%d, c=%d, abc=%d\n", a, b, c, a*b*c);
}
