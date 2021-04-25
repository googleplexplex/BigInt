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
	bool nonNegative;
public:
	vector<byte> val;

	//totest
	template <typename startValueType>
	BigInt(startValueType value, uint size)
	{
		if constexpr (!is_arithmetic<startValueType>::value && is_same<startValueType, BigInt>::value)
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
		if constexpr (!is_arithmetic<startValueType>::value && is_same<startValueType, BigInt>::value)
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
	void addByte()
	{
		this->size++;
		val.emplace_back(byte(0));
	}
	/*int size()
	{
		return size;
	}
	bool nonNegative()
	{
		return nonNegative;
	}*/

	//Внутриклассовые операторы
	friend const BigInt& operator++(BigInt& i);
	friend const BigInt& operator--(BigInt& i);
	friend const BigInt operator++(BigInt& i, int);
	friend const BigInt operator--(BigInt& i, int);
	friend const BigInt& operator+(const BigInt& f, const BigInt& s);
	friend const BigInt operator-(const BigInt& f, const BigInt& s);
	friend const BigInt operator*(const BigInt& f, const BigInt& s);
	friend const BigInt operator/(const BigInt& f, const BigInt& s);
	friend const BigInt operator%(const BigInt& f, const BigInt& s);
	//Битовые			v&x v<<x v>>x v^x v|x
	friend const BigInt operator&(const BigInt& i, int j);
	friend const BigInt operator^(const BigInt& i, int j);
	friend const BigInt operator|(const BigInt& i, int j);
	friend const BigInt operator<<(const BigInt& i, int j);
	friend const BigInt operator>>(const BigInt& i, int j);
	//Логические		v<x v>x v==x v!=x  (op=)
	friend bool operator<(const BigInt& f, const BigInt& s);
	friend bool operator>(const BigInt& f, const BigInt& s);
	friend bool operator<=(const BigInt& f, const BigInt& s);
	friend bool operator>=(const BigInt& f, const BigInt& s);
	friend bool operator==(const BigInt& f, const BigInt& s);
	friend bool operator!=(const BigInt& f, const BigInt& s);
	//(Остальное)		+v -v v=x v[x](разряд x)
	friend const BigInt operator+(const BigInt& i);
	friend const BigInt operator-(const BigInt& i);
	const BigInt& operator=(const BigInt& i);
	const BigInt& operator[](const int index);

	//Операторы преобразования
	//(Любой численный тип)
};

//Операторы
//Инк/декремент		++v --v v++ v--
const BigInt& operator++(BigInt& i) //totest
{
	logout << "++v called" << endl;

	i = i + 1;
	return i;
}
const BigInt& operator--(BigInt& i) //totest
{
	logout << "--v called" << endl;

	i = i - 1;
	return i;
}
const BigInt operator++(BigInt& i, int) //totest
{
	logout << "v++ called" << endl;

	BigInt result = i;
	i = i + 1;
	return result;
}
const BigInt operator--(BigInt& i, int) //totest
{
	logout << "v-- called" << endl;

	BigInt result = i;
	i = i - 1;
	return result;
}
//Арифметика		+ - * / %  (op=)
const BigInt& operator+(const BigInt& f, const BigInt& s) //totest
{
	logout << "v+j called" << endl;

	//Валидируем аргументы
	if (!f.nonNegative && s.nonNegative) //(-f) + s = s - (-(-f))
		return s - (-f);
	if (f.nonNegative && !s.nonNegative) //f + (-s) = f - (-(-s))
		return f - (-s);
	if (!f.nonNegative && !s.nonNegative) //(-f) + (-s) = -((-(-f)) + (-(-s)))
		return -((-f) + (-s));
	//f >= 0, s >= 0

	BigInt result(0, ((f >= s) ? (f.size) : (s.size)) + 1);
	const BigInt& biggerInt = (f >= s) ? (f) : (s);
	const BigInt& smallerInt = (f >= s) ? (s) : (f);

	bool addOne = false;
	for (int i = 0; i < smallerInt.size; i++) //Складываем числа
	{
		short presentResult = f.val[i] + s.val[i];

		if (addOne)
		{
			presentResult++;
			addOne = false;
		}
		if (presentResult > BYTE_MAX)
		{
			addOne = true;
			presentResult -= BYTE_MAX;
		}

		result.val[i] = byte(presentResult);
	}
	int i;
	for (i = smallerInt.size; i < result.size && addOne; i++) //Переносим единицу от сложения, если такая осталась
	{
		short presentResult = biggerInt.val[i] + 1;

		if (addOne)
		{
			presentResult++;
			addOne = false;
		}
		if (presentResult > BYTE_MAX)
		{
			addOne = true;
			presentResult -= BYTE_MAX;
		}

		result.val[i] = byte(presentResult);
	}
	for (i = smallerInt.size; i < result.size; i++) //Копируем оставшуюся часть большего числа в результат
	{
		result.val[i] = biggerInt.val[i];
	}

	return result;
}
const BigInt operator-(const BigInt& f, const BigInt& s) //totest
{
	logout << "v-j called" << endl;

	//Валидируем аргументы
	if (!f.nonNegative && s.nonNegative) //-f - s = -(f + s) = -(-(-f) + s)
		return -(-f + s);
	if (f.nonNegative && !s.nonNegative) //f - (-s) = f + s = f + -(-s)
		return f + -s;
	if (!f.nonNegative && !s.nonNegative) //-f - (-s) = -f + s = s - f = -(-s) - -(-f)
		return -s - -f;
	if (s > f) //f - s = -(s - f)  (Только если s > f)
		return -(s - f);
	//f >= 0, s >= 0, f >= s


	const BigInt& biggerInt = (f >= s) ? (f) : (s);
	const BigInt& smallerInt = (f >= s) ? (s) : (f);
	BigInt result = biggerInt;

	for (int i = smallerInt.size; i >= 0; i--)
	{
		if (f.val[i] >= s.val[i])
			result.val[i] = f.val[i] - s.val[i];
		else {
			result.val[i] = BYTE_MAX + f.val[i] - s.val[i];

			for (int j = i + 1; j < result.size; j++)
			{
				if (result.val[j] != 0)
				{
					result.val[j]--;
					break;
				}
				else {
					result.val[j] = BYTE_MAX;
				}
			}
		}
	}

	return result;
}
const BigInt operator*(const BigInt& f, const BigInt& s)
{
	logout << "v*j called" << endl;
	throw exception("still in dev!");
}
const BigInt operator/(const BigInt& f, const BigInt& s)
{
	logout << "v/j called" << endl;
	throw exception("still in dev!");
}
const BigInt operator%(const BigInt& f, const BigInt& s)
{
	logout << "v%j called" << endl;
	throw exception("still in dev!");
}
//(op=)
//Битовые			v&x v<<x v>>x v^x v|x
const BigInt operator&(const BigInt& i, int j)
{
	logout << "v&j called" << endl;
	throw exception("still in dev!");
}
const BigInt operator^(const BigInt& i, int j)
{
	logout << "v^j called" << endl;
	throw exception("still in dev!");
}
const BigInt operator|(const BigInt& i, int j)
{
	logout << "v|j called" << endl;
	throw exception("still in dev!");
}
const BigInt operator<<(const BigInt& i, int j)
{
	logout << "v<<j called" << endl;
	throw exception("still in dev!");
}
const BigInt operator>>(const BigInt& i, int j)
{
	logout << "v>>j called" << endl;
	throw exception("still in dev!");
}
//Логические		v<x v>x v==x v!=x  (op=)
bool operator<(const BigInt& f, const BigInt& s)
{
	logout << "v<j called" << endl;
	throw exception("still in dev!");
}
bool operator>(const BigInt& f, const BigInt& s)
{
	logout << "v>j called" << endl;
	throw exception("still in dev!");
}
bool operator<=(const BigInt& f, const BigInt& s)
{
	logout << "v<=j called" << endl;
	throw exception("still in dev!");
}
bool operator>=(const BigInt& f, const BigInt& s)
{
	logout << "v>=j called" << endl;
	throw exception("still in dev!");
}
bool operator==(const BigInt& f, const BigInt& s)
{
	logout << "v==j called" << endl;
	throw exception("still in dev!");
}
bool operator!=(const BigInt& f, const BigInt& s)
{
	logout << "v!=j called" << endl;
	throw exception("still in dev!");
}
//(Остальное)		+v -v v=x v[x](разряд x)
const BigInt operator+(const BigInt& f)
{
	logout << "+v called" << endl;
	return f;
}
const BigInt operator-(const BigInt& f)
{
	logout << "-v called" << endl;

	BigInt result = f;
	result.nonNegative = !result.nonNegative;
	return result;
}
const BigInt& BigInt::operator=(const BigInt& i)
{
	logout << "v=j called" << endl;
	throw exception("still in dev!");
}
const BigInt& BigInt::operator[](const int index)
{
	logout << "v[j] called" << endl;
	throw exception("still in dev!");
}
//Неиспользованные	!v &v v&&x v||x v() (v,x) v->x



//Неотрицательные числа



//Дроби






template<typename T>
T Abs(T num)
{
	if (!is_arithmetic<T>::value || is_same<T, BigInt>::value)
		throw notArithmeticTypeException("T");

	if (is_unsigned<T>::value || num >= 0)
		return num;
	else //T is signed && num < 0
		return -num;
}