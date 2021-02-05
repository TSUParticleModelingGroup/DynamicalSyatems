//gcc Collatz3x+1Simple.c -o temp -lm
#include <stdio.h>

int main(int argc, char** argv)
{
	int i;
	int n = 40;
	int x = 200;
	
	printf("\n\n  x[%d] = %d\n", 0, x);
	for(i = 0; i < n; i++)
	{
		if(x%2 == 0) x = x/2;
		else x = 3*x + 1;
		printf("  x[%d] = %d\n", i, x);  
	}
}
