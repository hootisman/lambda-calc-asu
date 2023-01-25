#include "limits.h"
#include "stdio.h" 

long long a=0, b=0, c=0, x=0, y=0, z=0;


int main()
{
	long long c = 1;
	a = 10;
	x = 100;
	y = 1000;
	{// z=f(x,y);
		long long x = 100;
		long long y = 1000;
		long long a = 10;
		b = 100;
		c = 1000;
		{// x = g(10)
			long long a = 10;
			printf("g: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
			a = 2 * a; b = 2 * b; y = 2 * y;
			x = (x + y - z) * a;
			
		}
		{
			long long y=0;
			{// y = g(x) = g(42000)
				long long a = x;
				printf("g: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
				a = 2 * a; b = 2 * b; y = 2 * y;
				y = (x + y - z) * a;
			} // end y=g(x)
			{
				long long x;
				x = a + b;
				{ // b = g(a) = g(10)
					long long a = 10;
					printf("g: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
					a = 2 * a; b = 2 * b; y = 2 * y;
					b = (x + y - z) * a;
				}// end b = g(a)

			}
			printf("f: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
		}
		{ // z = g(0)
			long long a = 0;
			printf("g: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
			a = 2 * a; b = 2 * b; y = 2 * y;
			z = (x + y - z) * a;
		} // end z = g(0)
		printf("f: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
		z = x * a - y * b + z * c;
	}// end z = f(x,y)
	{// z = g(a)
		long long a = 10;
		printf("g: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
		a = 2 * a; b = 2 * b; y = 2 * y;
		z = (x + y - z) * a;
	}// end z = g(a)
	printf("main: %lld %lld %lld %lld %lld\n", a, b, x, y, z);
	return 0;
}
