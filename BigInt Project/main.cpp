#include <iostream>
#include <string>
#include "BigInt.hpp"
using namespace std;

int main()
{
	long long int llia = 258;
	BigInt a(0);

	uint errorsCount = 0;
	uint testsCount = 1000000;

	for (int i = 0; i <= testsCount; i++)
	{
		BigInt b(i);
		long long int j = i;
		long long int trueRes = j--;
		long long int myRes = b--;

		if (trueRes != myRes)
		{
			cout << "ERROR! " << endl;
			errorsCount++;
		}
		else {
			cout << i << endl;
		}
	}
	cout << testsCount * testsCount - errorsCount << " tests passed" << endl;
	cout << errorsCount << " tests failed" << endl;

	return 0;
}

/*
	++a;
	--a;
	a++;
	a--;

	a + 1;
	a - 1;
	a * 1;
	a / 1;
	a % 1;

	a & 1;
	a ^ 1;
	a | 1;
	a << 1;
	a >> 1;

	a < 1;
	a > 1;
	a <= 1;
	a >= 1;
	a == 1;
	a != 1;

	+a;
	-a;
	a = 1;
	a[1];
*/