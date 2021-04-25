#pragma once
#include <vector>
#include "functions.hpp"
using namespace std;

template<typename T>
T Abs(T num);


class BigInt
{
	uint size;
	bool nonNegative;
public:
	vector<byte> val;

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

	//��������������

	//������
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

	//��������������� ���������
	friend const BigInt& operator++(BigInt& i);
	friend const BigInt& operator--(BigInt& i);
	friend const BigInt operator++(BigInt& i, int);
	friend const BigInt operator--(BigInt& i, int);
	friend const BigInt& operator+(const BigInt& f, const BigInt& s);
	friend const BigInt operator-(const BigInt& f, const BigInt& s);
	friend const BigInt operator*(const BigInt& f, const BigInt& s);
	friend const BigInt operator/(const BigInt& f, const BigInt& s);
	friend const BigInt operator%(const BigInt& f, const BigInt& s);
	//�������			v&x v<<x v>>x v^x v|x
	friend const BigInt operator&(const BigInt& i, int j);
	friend const BigInt operator^(const BigInt& i, int j);
	friend const BigInt operator|(const BigInt& i, int j);
	friend const BigInt operator<<(const BigInt& i, int j);
	friend const BigInt operator>>(const BigInt& i, int j);
	//����������		v<x v>x v==x v!=x  (op=)
	friend bool operator<(const BigInt& f, const BigInt& s);
	friend bool operator>(const BigInt& f, const BigInt& s);
	friend bool operator<=(const BigInt& f, const BigInt& s);
	friend bool operator>=(const BigInt& f, const BigInt& s);
	friend bool operator==(const BigInt& f, const BigInt& s);
	friend bool operator!=(const BigInt& f, const BigInt& s);
	//(���������)		+v -v v=x v[x](������ x)
	const BigInt operator+(const BigInt& i);
	const BigInt operator-(const BigInt& i);
	const BigInt& operator=(const BigInt& i);
	const BigInt& operator[](const int index);

	//��������� ��������������
	//(����� ��������� ���)
};
const BigInt nullBigInt(0);

//���������
//���/���������		++v --v v++ v--
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
//����������		+ - * / %  (op=)
const BigInt& operator+(const BigInt& f, const BigInt& s) //totest
{
	logout << "v+j called" << endl;

	//���������� ���������
	if (!f.nonNegative && s.nonNegative) //(-f) + s = s - (-(-f))
		return s - (-f);
	if (f.nonNegative && !s.nonNegative) //f + (-s) = f - (-(-s))
		return f - (-s);
	if (!f.nonNegative && !s.nonNegative) //(-f) + (-s) = -((-(-f)) + (-(-s)))
		return -((-f) + (-s));

	BigInt result(0, ((f >= s) ? (f.size) : (s.size)) + 1);
	const BigInt& biggerInt = (f >= s) ? (f) : (s);
	const BigInt& smallerInt = (f >= s) ? (s) : (f);

	bool addOne = false;
	for (int i = 0; i < smallerInt.size; i++) //���������� �����
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
	for (i = smallerInt.size; i < result.size && addOne; i++) //��������� ������� �� ��������, ���� ����� ��������
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
	for (i = smallerInt.size; i < result.size; i++) //�������� ���������� ����� �������� ����� � ���������
	{
		result.val[i] = biggerInt.val[i];
	}

	return result;
}
const BigInt operator-(const BigInt& f, const BigInt& s)
{
	logout << "v-j called" << endl;

	//���������� ���������
	if (!f.nonNegative && s.nonNegative) //(-f) + s = s - (-(-f))
		return s - (-f);
	if (f.nonNegative && !s.nonNegative) //f + (-s) = f - (-(-s))
		return f - (-s);
	if (!f.nonNegative && !s.nonNegative) //(-f) + (-s) = -((-(-f)) + (-(-s)))
		return -((-f) + (-s));

	BigInt result(0, ((f >= s) ? (f.size) : (s.size)) + 1);
	const BigInt& biggerInt = (f >= s) ? (f) : (s);
	const BigInt& smallerInt = (f >= s) ? (s) : (f);

	return nullBigInt;
}
const BigInt operator*(const BigInt& f, const BigInt& s)
{
	logout << "v*j called" << endl;
	return nullBigInt;
}
const BigInt operator/(const BigInt& f, const BigInt& s)
{
	logout << "v/j called" << endl;
	return nullBigInt;
}
const BigInt operator%(const BigInt& f, const BigInt& s)
{
	logout << "v%j called" << endl;
	return nullBigInt;
}
//(op=)
//�������			v&x v<<x v>>x v^x v|x
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
//����������		v<x v>x v==x v!=x  (op=)
bool operator<(const BigInt& f, const BigInt& s)
{
	logout << "v<j called" << endl;
	return true;
}
bool operator>(const BigInt& f, const BigInt& s)
{
	logout << "v>j called" << endl;
	return true;
}
bool operator<=(const BigInt& f, const BigInt& s)
{
	logout << "v<=j called" << endl;
	return true;
}
bool operator>=(const BigInt& f, const BigInt& s)
{
	logout << "v>=j called" << endl;
	return true;
}
bool operator==(const BigInt& f, const BigInt& s)
{
	logout << "v==j called" << endl;
	return true;
}
bool operator!=(const BigInt& f, const BigInt& s)
{
	logout << "v!=j called" << endl;
	return true;
}
//(���������)		+v -v v=x v[x](������ x)
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
//����������������	!v &v v&&x v||x v() (v,x) v->x



//��������������� �����



//�����






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