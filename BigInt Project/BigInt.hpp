#pragma once
#include <vector>
#include "functions.hpp"
using namespace std;

template<typename T>
T Abs(T num);


class BigInt
{
public:
	uint size;
	vector<byte> val;
	bool nonNegative;

	//totest
	template <typename startValueType>
	BigInt(startValueType value, uint size)
	{
		if (!is_arithmetic<startValueType> && startValueType != BigInt)
			throw notArithmeticTypeException("startValueType");

		if (size >= sizeof(value))
		{
			this->size = size;
			val.assign(size, 0);
			nonNegative = bool(value >= 0);
			startValueType valueModule = Abs(value);
			memcpy(&(val[0]), &valueModule, sizeof(valueModule));
		}
		else
		{
			for (int i = sizeof(value) - 1; true; i--)
			{
				if (GET_BYTE(value, i) != NULL || i == 0)
				{
					this->size = i + 1;
					val.assign(this->size, 0);
					nonNegative = bool(value >= 0);
					startValueType valueModule = Abs(value);
					memcpy(&(val[0]), &valueModule, this->size);
					break;
				}
			}
		}
	}
	
	//totest
	template <typename startValueType>
	BigInt(startValueType value = 0)
	{
		if (!is_arithmetic<startValueType> && startValueType != BigInt)
			throw notArithmeticTypeException("startValueType");

		for (int i = sizeof(value) - 1; true; i--)
		{
			if (GET_BYTE(value, i) != NULL || i == 0)
			{
				this->size = i + 1;
				val.assign(this->size, 0);
				nonNegative = bool(value >= 0);
				startValueType valueModule = Abs(value);
				memcpy(&(val[0]), &valueModule, this->size);
				break;
			}
		}
	}

	//Деконструкторы

	//Методы

	//Внутриклассовые операторы
	const BigInt& operator=(const BigInt& i);
	const BigInt& operator[](const int index);

	//Операторы преобразования
	//(Любой численный тип)
};
const BigInt nullBigInt(0);

//Операторы
//Инк/декремент		++v --v v++ v--
const BigInt& operator++(BigInt& i)
{
	logout << "++v called" << endl;

	i = i + 1;
	return i;
}
const BigInt& operator--(BigInt& i)
{
	logout << "--v called" << endl;

	i = i - 1;
	return i;
}
const BigInt operator++(BigInt& i, int)
{
	logout << "v++ called" << endl;

	BigInt result = i;
	i = i + 1;
	return result;
}
const BigInt operator--(BigInt& i, int)
{
	logout << "v-- called" << endl;

	BigInt result = i;
	i = i - 1;
	return result;
}
//Арифметика		+ - * / %  (op=)
const BigInt& operator+(const BigInt& i, BigInt j)
{
	logout << "v+j called" << endl;


	return nullBigInt;
}
const BigInt operator-(const BigInt& i, BigInt j)
{
	logout << "v-j called" << endl;
	return nullBigInt;
}
const BigInt operator*(const BigInt& i, BigInt j)
{
	logout << "v*j called" << endl;
	return nullBigInt;
}
const BigInt operator/(const BigInt& i, BigInt j)
{
	logout << "v/j called" << endl;
	return nullBigInt;
}
const BigInt operator%(const BigInt& i, BigInt j)
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






template<typename T>
T Abs(T num)
{
	if (!is_arithmetic<T> || T != BigInt)
		throw notArithmeticTypeException("T");

	if (is_unsigned<T> || num >= 0)
		return num;
	else //T is signed && num < 0
		return -num;
}