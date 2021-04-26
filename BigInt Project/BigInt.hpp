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

	BigInt()
	{
		size = 0;
		nonNegative = true;
	}
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

	BigInt(string value)
	{
		throw exception("still in dev!");
	}
	
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

	//��������������

	//������
	void addByte()
	{
		this->size++;
		val.emplace_back(byte(0));
	}

	//��������������� ���������
	friend BigInt operator++(BigInt& i);
	friend BigInt operator--(BigInt& i);
	friend BigInt operator++(BigInt& i, int);
	friend BigInt operator--(BigInt& i, int);
	friend BigInt operator+(const BigInt& f, const BigInt& s);
	friend BigInt operator-(const BigInt& f, const BigInt& s);
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
	//(���������)		+v -v v=x v[x](������ x)  x<<v x>>v
	friend const BigInt operator+(const BigInt& i);
	friend const BigInt operator-(const BigInt& i);
	const BigInt& operator=(const BigInt& i);
	const BigInt& operator[](const int index);
	friend ostream& operator<<(ostream& out, BigInt& point);
	friend istream& operator>>(istream& in, BigInt& point);

	//��������� ��������������
	operator char*()
	{
		log("char*(v) called");
		throw exception("still in dev!");
	}
	operator unsigned long long int() //tested 0<=v<=200000
	{
		if (nonNegative && size <= sizeof(unsigned long long int))
		{
			unsigned long long int result = 0;
			memcpy(&result, &val[0], size);
			return result;
		}
		else
			throw exception("still in dev!");
	}
	operator long long int() //tested -500000<=v<=500000
	{
		if (size <= sizeof(long long int))
		{
			long long int result = 0;
			memcpy(&result, &val[0], size);
			if (!nonNegative)
				result *= -1;
			return result;
		}
		else
			throw exception("still in dev!");
	}
	//(����� ��������� ���)
};
string to_string(BigInt& f) //dependent
{
	if (f.nonNegative && f.size <= sizeof(unsigned long long int))
		return to_string((unsigned long long int)(f));
	else if(!f.nonNegative && f.size <= sizeof(long long int))
		return to_string((long long int)(f));
	else
		throw exception("still in dev!");
	//return string(static_cast<char*>(*this));
}

//���������
//���/���������		++v --v v++ v--
BigInt operator++(BigInt& i) //dependent, tested 0<=i<=1000000
{
	log("++v called");

	i = i + BigInt(1);
	return i;
}
BigInt operator--(BigInt& i) //dependent, tested 0<=i<=1000000
{
	log("--v called");

	i = i - BigInt(1);
	return i;
}
BigInt operator++(BigInt& i, int) //dependent, tested 0<=i<=1000000
{
	log("v++ called");

	BigInt result = i;
	i = i + BigInt(1);
	return result;
}
BigInt operator--(BigInt& i, int) //dependent, tested 0<=i<=1000000
{
	log("v-- called");

	BigInt result = i;
	i = i - BigInt(1);
	return result;
}
//����������		+ - * / %  (op=)
BigInt operator+(const BigInt& f, const BigInt& s) //dependent, tested 0<=f<1000 0<=s<1000
{
	log("v+j called");

	//���������� ���������
	if (!f.nonNegative && s.nonNegative) //(-f) + s = s - (-(-f))
		return s - (-f);
	if (f.nonNegative && !s.nonNegative) //f + (-s) = f - (-(-s))
		return f - (-s);
	if (!f.nonNegative && !s.nonNegative) //(-f) + (-s) = -((-(-f)) + (-(-s)))
		return -((-f) + (-s));
	//f >= 0, s >= 0

	BigInt result;
	result.size = ((f >= s) ? (f.size) : (s.size)) + 1;
	result.val.assign(result.size, 0);

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
			presentResult -= (BYTE_MAX + 1);
		}

		result.val[i] = byte(presentResult);
	}

	int i;
	for (i = smallerInt.size; i < biggerInt.size && addOne; i++) //��������� ������� �� ��������, ���� ����� ��������
	{
		short presentResult = biggerInt.val[i] + 1;

		addOne = false;
		if (presentResult > BYTE_MAX)
		{
			addOne = true;
			presentResult -= BYTE_MAX;
		}

		result.val[i] = byte(presentResult);
	}

	if (addOne)
	{
		result.val[result.size - 1] = 1;
	}
	else {
		for (; i < biggerInt.size; i++) //�������� ���������� ����� �������� ����� � ���������
		{
			result.val[i] = biggerInt.val[i];
		}
	}

	return result;
}
BigInt operator-(const BigInt& f, const BigInt& s) //dependent, tested 0<=f<1000 0<=s<1000
{
	log("v-j called");

	//���������� ���������
	if (!f.nonNegative && s.nonNegative) //-f - s = -(f + s) = -(-(-f) + s)
		return -(-f + s);
	if (f.nonNegative && !s.nonNegative) //f - (-s) = f + s = f + -(-s)
		return f + -s;
	if (!f.nonNegative && !s.nonNegative) //-f - (-s) = -f + s = s - f = -(-s) - -(-f)
		return -s - -f;
	if (s > f) //f - s = -(s - f)  (������ ���� s > f)
		return -(s - f);
	//f >= 0, s >= 0, f >= s


	const BigInt& biggerInt = (f >= s) ? (f) : (s);
	const BigInt& smallerInt = (f >= s) ? (s) : (f);
	BigInt result = biggerInt;

	for (int i = smallerInt.size - 1; i >= 0; i--)
	{
		if (f.val[i] >= s.val[i])
			result.val[i] = f.val[i] - s.val[i];
		else {
			result.val[i] = (short(BYTE_MAX) + 1) + f.val[i] - s.val[i];

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
	log("v*j called");
	throw exception("still in dev!");
}
const BigInt operator/(const BigInt& f, const BigInt& s)
{
	log("v/j called");
	throw exception("still in dev!");
}
const BigInt operator%(const BigInt& f, const BigInt& s)
{
	log("v%j called");
	throw exception("still in dev!");
}
//(op=)
//�������			v&x v<<x v>>x v^x v|x
const BigInt operator&(const BigInt& i, int j)
{
	log("v&j called");
	throw exception("still in dev!");
}
const BigInt operator^(const BigInt& i, int j)
{
	log("v^j called");
	throw exception("still in dev!");
}
const BigInt operator|(const BigInt& i, int j)
{
	log("v|j called");
	throw exception("still in dev!");
}
const BigInt operator<<(const BigInt& i, int j)
{
	log("v<<j called");
	throw exception("still in dev!");
}
const BigInt operator>>(const BigInt& i, int j)
{
	log("v>>j called");
	throw exception("still in dev!");
}
//����������		v<x v>x v==x v!=x  (op=)
bool operator<(const BigInt& f, const BigInt& s) //independent, tested 0<=f<=1001 0<=s<=1001
{
	log("f<s called");

	//���������� ���������
	if (!f.nonNegative && s.nonNegative) //-f < s
		return true;
	if (f.nonNegative && !s.nonNegative) //f < -s
		return false;

	if (f.size < s.size)
	{
		for (int i = f.size; i < s.size; i++)
		{
			if (s.val[i] != 0)
				return true;
		}
	}
	else if (f.size > s.size)
	{
		for (int i = s.size; i < f.size; i++)
		{
			if (f.val[i] != 0)
				return false;
		}
	}

	const BigInt& smallerInt = (f.size >= s.size) ? (s) : (f);

	for (int i = smallerInt.size - 1; i >= 0; i--)
	{
		if (f.val[i] != s.val[i])
		{
			return f.val[i] < s.val[i];
		}
	}

	return false;
}
bool operator>(const BigInt& f, const BigInt& s) //dependent
{
	log("f>s called");

	return s < f;
}
bool operator<=(const BigInt& f, const BigInt& s) //dependent
{
	log("f<=s called");

	return (f < s || f == s);
}
bool operator>=(const BigInt& f, const BigInt& s) //dependent
{
	log("f>=s called");

	return (f > s || f == s);
}
bool operator==(const BigInt& f, const BigInt& s) //independent, tested 0<=f<=1001 0<=s<=1001
{
	log("v==j called");

	//���������� ���������
	if (!f.nonNegative && s.nonNegative) //-f < s
		return false;
	if (f.nonNegative && !s.nonNegative) //f < -s
		return false;

	const BigInt& biggerInt = (f.size >= s.size) ? (f) : (s);
	const BigInt& smallerInt = (f.size >= s.size) ? (s) : (f);

	for (int i = smallerInt.size; i < biggerInt.size; i++)
	{
		if (biggerInt.val[i] != 0)
			return false;
	}

	for (int i = smallerInt.size - 1; i >= 0; i--)
	{
		if (s.val[i] != f.val[i])
			return false;
	}

	return true;
}
bool operator!=(const BigInt& f, const BigInt& s) //dependent, primitive
{
	log("v!=j called");

	return !(f == s);
}
//(���������)		+v -v v=x v[x](������ x) x<<v x>>v
const BigInt operator+(const BigInt& f) //independent, primitive
{
	log("+v called");
	return f;
}
const BigInt operator-(const BigInt& f) //independent, primitive
{
	log("-v called");

	BigInt result = f;
	result.nonNegative = !result.nonNegative;
	return result;
}
const BigInt& BigInt::operator=(const BigInt& i) //independent, primitive, tested 0<=i<=1000000
{
	log("v=j called");

	this->size = i.size;
	this->nonNegative = i.nonNegative;
	this->val = i.val;

	return i;
}
const BigInt& BigInt::operator[](const int index)
{
	log("v[j] called");
	throw exception("still in dev!");
}
ostream& operator<<(ostream& out, BigInt& f) //dependent, primitive
{
	out << to_string(f);
	return out;
}

istream& operator>>(istream& in, BigInt& f)
{
	throw exception("still in dev!");
	return in;
}
//����������������	!v &v v&&x v||x v() (v,x) v->x



//��������������� �����



//�����






template<typename T>
T Abs(T num)
{
	if (!is_arithmetic<T>::value || is_same<T, BigInt>::value)
		throw notArithmeticTypeException("T");

	if (is_unsigned<T>::value || num >= 0)
		return num;
	else //T is signed && num < 0
		return 0 - num;
}