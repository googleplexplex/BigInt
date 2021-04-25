#pragma once
#include <vector>
using namespace std;

#define GET_BYTE(x, pos) ((byte*)(&(x)))[(pos)]
#define nop() __asm { nop }
#define logout cout

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
	BigInt(uint size = 16)
	{
		this->size = size;
		val.assign(size, 0);
		nonNegative = true;
	}

	//Деконструкторы

	//Методы

	//Внутриклассовые операторы
	const BigInt& operator=(const BigInt& i);
	const BigInt& operator[](const int index);

	//Операторы преобразования
	//(Любой численный тип)
};
const BigInt nullBigInt(0u);

//Операторы
//Инк/декремент		++v --v v++ v--
const BigInt& operator++(BigInt& i)
{
	logout << "++v called" << endl;
	return nullBigInt;
}
const BigInt& operator--(BigInt& i)
{
	logout << "--v called" << endl;
	return nullBigInt;
}
const BigInt operator++(BigInt& i, int)
{
	logout << "v++ called" << endl;
	return nullBigInt;
}
const BigInt operator--(BigInt& i, int)
{
	logout << "v-- called" << endl;
	return nullBigInt;
}
//Арифметика		+ - * / %  (op=)
const BigInt& operator+(const BigInt& i, int j)
{
	logout << "v+j called" << endl;
	return nullBigInt;
}
const BigInt operator-(const BigInt& i, int j)
{
	logout << "v-j called" << endl;
	return nullBigInt;
}
const BigInt operator*(const BigInt& i, int j)
{
	logout << "v*j called" << endl;
	return nullBigInt;
}
const BigInt operator/(const BigInt& i, int j)
{
	logout << "v/j called" << endl;
	return nullBigInt;
}
const BigInt operator%(const BigInt& i, int j)
{
	logout << "v%j called" << endl;
	return nullBigInt;
}
//(op=)
//Битовые			v&x v<<x v>>x v^x v|x
const BigInt operator&(const BigInt& i, int j)
{
	logout << "v&j called" << endl;
	return nullBigInt;
}
const BigInt operator^(const BigInt& i, int j)
{
	logout << "v^j called" << endl;
	return nullBigInt;
}
const BigInt operator|(const BigInt& i, int j)
{
	logout << "v|j called" << endl;
	return nullBigInt;
}
const BigInt operator<<(const BigInt& i, int j)
{
	logout << "v<<j called" << endl;
	return nullBigInt;
}
const BigInt operator>>(const BigInt& i, int j)
{
	logout << "v>>j called" << endl;
	return nullBigInt;
}
//Логические		v<x v>x v==x v!=x  (op=)
const BigInt operator<(const BigInt& i, int j)
{
	logout << "v<j called" << endl;
	return nullBigInt;
}
const BigInt operator>(const BigInt& i, int j)
{
	logout << "v>j called" << endl;
	return nullBigInt;
}
const BigInt operator<=(const BigInt& i, int j)
{
	logout << "v<=j called" << endl;
	return nullBigInt;
}
const BigInt operator>=(const BigInt& i, int j)
{
	logout << "v>=j called" << endl;
	return nullBigInt;
}
const BigInt operator==(const BigInt& i, int j)
{
	logout << "v==j called" << endl;
	return nullBigInt;
}
const BigInt operator!=(const BigInt& i, int j)
{
	logout << "v!=j called" << endl;
	return nullBigInt;
}
//(Остальное)		+v -v v=x v[x](разряд x)
const BigInt operator+(const BigInt& i)
{
	logout << "+v called" << endl;
	return nullBigInt;
}
const BigInt operator-(const BigInt& i)
{
	logout << "-v called" << endl;
	return nullBigInt;
}
const BigInt& BigInt::operator=(const BigInt& i)
{
	logout << "v=j called" << endl;
	return nullBigInt;
}
const BigInt& BigInt::operator[](const int index)
{
	logout << "v[j] called" << endl;
	return nullBigInt;
}
//Неиспользованные	!v &v v&&x v||x v() (v,x) v->x



//Неотрицательные числа



//Дроби