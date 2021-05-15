#include <iostream>
#include <string>
#include "BigInt.hpp"
using namespace std;

int main()
{

	for (uint i = 15; i < UINT_MAX - 1; i++)
	{
		string trueStr = to_string(i);
		BigInt j(trueStr);

		if (BigInt(i) != j)
		{
			cout << "ERROR! " << j << " not " << i << endl;
		}
		else {
			cout << j << " ok" << endl;
		}
	}





	long long int llia = 258;
	BigInt a(0);

	uint errorsCount = 0;
	uint testsCount = 500;

	for (int i = 0; i <= testsCount; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			BigInt a(i);
			a.addBytes(8 - a.size);
			BigInt b(j);
			unsigned long long int t = 0; t--;
			long long int trueRes = i >> j;
			long long int myRes = a >> j;

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