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