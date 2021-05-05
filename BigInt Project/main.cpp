#include <iostream>
#include <string>
#include "BigInt.hpp"
using namespace std;

int main()
{
	long long int llia = 258;
	BigInt a(0);

	uint errorsCount = 0;
	uint testsCount = 500;

	for (int i = 1; i <= testsCount; i++)
	{
		for (int j = 2; j <= testsCount; j++)
		{
			BigInt a(i);
			BigInt b(j);
			long long int trueRes = i * j;
			long long int myRes = a * b;

			if (trueRes != myRes)
			{
				cout << "ERROR! " << i << ' ' << j << endl;
				errorsCount++;
			}
			else {
				cout << i << ' ' << j << endl;
			}
		}
	}
	cout << testsCount * testsCount - errorsCount << " tests passed" << endl;
	cout << errorsCount << " tests failed" << endl;

	return 0;
}

//Убрать лишний ноль в сложении
//Дебажить сложный случай вычитания

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