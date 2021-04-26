#include <iostream>
#include <string>
#include "BigInt.hpp"
using namespace std;

int main()
{
	long long int llia = 258;
	BigInt a(0);

	uint errorsCount = 0;
	long long int testsCount = 500000;

	for (long long int i = -testsCount; i <= testsCount; i++)
	{
		BigInt b(i);
		long long int trueRes = i;
		long long int myRes = b;

		if (trueRes != myRes)
		{
			cout << "ERROR! " << i << " interpreted as " << myRes << endl;
			errorsCount++;
		}
		else
			cout << i << " ok" << endl;
	}

	for (uint i = 0; i <= testsCount; i++)
	{
		for (uint j = 0; j <= testsCount; j++)
		{
			BigInt b(i);
			BigInt c(j);
			long long int trueRes = i - j;
			long long int myRes = b - c;

			if (trueRes != myRes)
			{
				cout << "ERROR! " << i << " - " << j << " = " << trueRes << ", but code answer is " << myRes << endl;
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