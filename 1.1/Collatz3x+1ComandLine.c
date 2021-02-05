//gcc Collatz3x+1ComandLine.c -o temp -lm
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int Collatz(int x)
{
	if(x%2 == 0) return(x/2);
	else return(3*x + 1);
}

int main(int argc, char** argv)
{
	int i, x, n;
	if(argc < 3)
	{
		printf("\n  Too few arguments!");
		printf("\n  First you need to enter a starting value.");
		printf("\n  Second you need to enter a how many times you want to iterate the system.");
		printf("\n  ./function x0 n \n\n");
		exit(0);
	}
	else if(argc == 3)
	{
		x = atoi(argv[1]);
		n = atoi(argv[2]);
	}
	else
	{
		printf("\n  Too many arguments!");
		printf("\n  First you need to enter a starting value.");
		printf("\n  Second you need to enter a how many times you want to iterate the system.");
		printf("\n  ./function x0 n \n\n");
		exit(0);
	}
	
	printf("\n\n  x[%d] = %d\n", 0, x);
	for(i = 0; i < n; i++)
	{
		x = Collatz(x);
		printf("  x[%d] = %d\n", i, x);
	}
}
