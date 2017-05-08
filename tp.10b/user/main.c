#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"


int main(int argc, char*argv[]) {

	printf("%s\n", );

	printf ("argc=%d\n", argc);
	for (int i=0; i<argc; i++)
		printf ("argv[%d](0x%p)=%s\n", i, &argv[i], argv[i]);

	char* p = "??"; if (argc > 1) p = argv[1];
	int c = 2;      if (argc > 2) c = atol(argv[2]);
	int tmo = 500;  if (argc > 3) tmo = atol(argv[3]);

	for (int i=0; i<c; i++) {
		yield();
		printf ("hello process %s!...\n", p);
		delay(tmo);
	}
	return 0;
}