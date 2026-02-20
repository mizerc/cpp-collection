#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include <gmp.h>

int main(int argc, char **argv)
{
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <index>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *endptr = NULL;
	long limit = strtol(argv[1], &endptr, 10);

	if(endptr == argv[1] || limit < 0) {
		fprintf(stderr, "Invalid numeric input. Exiting.\n");
		return EXIT_FAILURE;
	}

	// Initialize big integers
	mpz_t prev, curr, next;
	mpz_init_set_ui(prev, 1);
	mpz_init_set_ui(curr, 0);
	mpz_init(next);

	// Compute Fibonacci with timing
	clock_t start_time = clock();
	for(long i = 0; i < limit; i++) {
		mpz_add(next, prev, curr);
		mpz_set(prev, curr);
		mpz_set(curr, next);
	}
	clock_t end_time = clock();

	// Print result
	printf("Fibonacci Number %ld: ", limit);
	mpz_out_str(stdout, 10, curr);
	printf("\n");

	// Cleanup
	mpz_clear(prev);
	mpz_clear(curr);
	mpz_clear(next);

	// Print elapsed time
	double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	printf("Time: %.6f seconds\n", elapsed);

	return EXIT_SUCCESS;
}