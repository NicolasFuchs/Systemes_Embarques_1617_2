#include <stdio.h>
#include <stdlib.h>

void yield() 
{
}


void delay(int t)
{
	(void)t;
}

int main(int argc, char*argv[]) {

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

