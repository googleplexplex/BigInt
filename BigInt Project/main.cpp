#include <iostream>
#include <string>
#include "BigInt.hpp"
#include "BigUInt.hpp"
using namespace std;

int main()
{
	BigInt a(10);
	string c = "";
	try {
		auto b = float(a);
	} catch (exception ex) {
		c = ex.what();
	}

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