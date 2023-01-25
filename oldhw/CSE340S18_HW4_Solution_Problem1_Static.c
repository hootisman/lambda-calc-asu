#include "limits.h"
#include "stdio.h" 

long long a, b, c, x, y, z;

void print(long long p){
	if (p == 0)
		printf("f: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
	else if (p == 1)
		printf("g: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
	else if (p == 2)
		printf("main: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
}

long long g(long long a) 
{
	print(1);
	a = 2*a; b = 2*b; y = 2*y;
	return (x + y - z) * a;
}

long long f(long long x, long long y)
{
	long long a = 10;
	b = 100; c = 1000; x = g(10);
	{
		long long y;
		y = g(x);
		{ long long x;
               		x = a+b;
               		b = g(a);
		} 
		print(0);
	}
	z = g(0);
	print(0);
	return x * a - y * b + z * c;
}

int main()
{
           long long c = 1;
           a = 10;
           x = 100;
           y = 1000;
           z = f(x, y);
           z = g(a);
           print(2);
           return 0;
}
