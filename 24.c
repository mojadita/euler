/* 24.c --- project euler, challenge 24.
 * A permutation is an ordered arrangement of objects.
 * For example, 3124 is one possible permutation of the
 * digits 1, 2, 3 and 4.  If all of the permutations are
 * listed numerically or alphabetically, we call it
 * lexicographic order.  The lexicographic permutations
 * of 0, 1 and 2 are:
 *
 * 012   021   102   120   201   210
 *
 * What is the millionth lexicographic permutation of
 * the digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?
 */

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F(_fmt) __FILE__":%d:%s: " _fmt, __LINE__, __func__

typedef unsigned long long ull;

char *def_lexset[]	= {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
};

ull n					= 1000000ULL;
char **lexset			= def_lexset;
size_t lexset_size		= sizeof def_lexset / sizeof def_lexset[0];

#define FLAG_VERBOSE 				(1 << 0)

int flags				= 0;

char *get_perm(int argc, char **argv, ull n, char *buf, size_t buflen);

int main(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "n:v")) != EOF) {
		switch (opt) {
		case 'n':
			if (sscanf(optarg, "%llu", &n) != 1) {
				fprintf(stderr,
					F("invalid parameter for option -n: %s, "
					"using previous value of %llu\n"),
					optarg, n);
			}
			break;

		case 'v': flags |= FLAG_VERBOSE; break;
		}
	}

	argv += optind;
	argc -= optind;

	if (argc) {
		lexset = argv;
		lexset_size = argc;
	}

	if (flags & FLAG_VERBOSE) {
		printf(F("n = %llu\n"), n);
		int i;
		for (i = 0; i < lexset_size; i++)
			printf(F("lexset[%d] = \"%s\"\n"),
				i, lexset[i]);
	}

	char buffer[10240];

	printf(F("Lexicographic permutation @ index %llu: %s\n"),
		n,
		get_perm(lexset_size, lexset, n, buffer, sizeof buffer));

	exit(EXIT_SUCCESS);
}

char *get_perm(int argc, char **argv, ull n, char *buf, size_t buflen)
{
	ull fact		= 1;
	char
		*sep1		= "",
		*sep2		= ", ",
		*sep		= sep1,
		*p			= buf;

	n--; /* to make the search 0 based */

	int i;
	for (i = 2; i <= argc; i++)
		fact *= i;

	if (n >= fact) {
		fprintf(stderr,
			F("error, n(%llu) > fact(card(lexset)) (%llu)\n"),
			n, fact);
		return "nothing appropiate";
	}

	while (argc) {
		fact /= argc;
		int i = n / fact;
		n %= fact;
		ssize_t out = snprintf(p, buflen, "%s%s", sep, argv[i]);
		if (out < 0) {
			fprintf(stderr, F("snrpintf: %s\n"),
				strerror(errno));
			return "<<<error>>>";
		}
		p += out;
		buflen -= out;
		int j;
		for (j = i; j < argc - 1; j++)
			argv[j] = argv[j+1];
		argc--;
		sep = sep2;
	}
	return buf;
}
