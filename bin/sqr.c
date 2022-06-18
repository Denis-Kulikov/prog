#include <stdio.h>
int sqr(int a)
{
	return a * a;
}
int main()
{
	int i = 2;
	while (i < 1024) {
		i = sqr(i);
		printf("%d ", i);
		printf(" ");
	}
}