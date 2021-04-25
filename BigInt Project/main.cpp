#include <iostream>
#include "BigInt.hpp"
using namespace std;

int main()
{
	long long int llia = 258;
	BigInt a(0);

	uint errorsCount = 0;
	uint testsCount = 1000;
	for (uint i = 0; i < testsCount; i++)
	{
		for (uint j = 0; j < testsCount; j++)
		{
			BigInt b(i);
			BigInt c(j);
			bool trueRes = i < j;
			bool myRes = b < c;

			if (trueRes != myRes)
			{
				cout << "ERROR! (" << i << " < " << j << ") = " << trueRes << ", but code answer wrong" << endl;
				errorsCount++;
			}
			else {
				cout << i << " " << j << " ok" << endl;
			}
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