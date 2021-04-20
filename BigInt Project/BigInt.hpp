#pragma once
#include <vector>
using namespace std;

#define GET_BYTE(x, pos) ((byte*)(&(x)))[(pos)]

using uint = unsigned int;
using byte = unsigned char;

class BigInt
{
public:
	uint size;
	vector<byte> val;
	bool nonNegative;

	template <typename startValueType>
	BigInt(uint size = 16, startValueType value = 0)
	{
		if (size >= sizeof(value))
		{
			this->size = size;
			val.assign(size, 0);
			nonNegative = bool(value >= 0);
			startValueType valueModule = abs(value);
			memcpy(&(val[0]), &valueModule, sizeof(valueModule));
		}
		else
		{
			for (int i = sizeof(value) - 1; i >= 0; i--)
			{
				if (GET_BYTE(value, i) != NULL)
				{
					this->size = i + 1;
					val.assign(this->size, 0);
					nonNegative = bool(value >= 0);
					startValueType valueModule = abs(value);
					memcpy(&(val[0]), &valueModule, this->size);
					break;
				}
			}
		}
	}

	//Деконструткоры

	//Методы

	//Арифметические операторы

	//Операторы преобразования
};

//Целые числа

//Дроби