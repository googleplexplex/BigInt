#pragma once
#include <vector>
#include <string>
#include <math.h>
#include "functions.hpp"
using namespace std;

class BigFloat;
pair<BigFloat, BigFloat> __div(BigFloat f, const BigFloat& s);
string to_string(BigFloat& f);
BigFloat to_BigFloat(string& value);

class BigFloat
{
public:
	BigInt whole;
	BigUInt fract;

	//Конструкторы
public:
	BigFloat()
	{
		whole = 0;
		fract = BigUInt(0);
	}

	BigFloat(string value) //indev, tested 0<=value<=3000
	{
		(*this) = to_BigFloat(value);
	}

	template <typename startValueType>
	BigFloat(startValueType value = 0)
	{
		if constexpr (is_floating_point<startValueType>::value)
		{
			double wholeDouble;
			double fract;
			fract = modf(value, &wholeDouble);
			int whole = int(wholeDouble);

			this->whole = BigInt(whole);
			this->fract = BigUInt(fract);

			return;
		}

		if constexpr (is_integral<startValueType>::value)
		{
			this->whole = BigInt(value);
			this->fract = BigUInt(0);
		}
		else
			throw notArithmeticTypeException("startValueType");
	}

	BigFloat(vector<byte>& whole, vector<byte>& fract, intNegative sign = positive) //totest
	{
		this->whole = BigInt(whole);
		this->fract = BigUInt(fract);
		this->whole.sign = sign;
	}

	BigFloat(BigInt& value);
	BigFloat(BigUInt& value);

	//Методы
public:
	intNegative getSign()
	{
		return whole.getSign();
	}
	uint getSize()
	{
		return whole.size + fract.size;
	}
	uint getWholeSize()
	{
		return whole.getSize();
	}
	vector<byte> getWholeByteArray()
	{
		return whole.getByteArray();
	}
	uint getFractSize()
	{
		return fract.getSize();
	}
	vector<byte> getFractByteArray()
	{
		return fract.getByteArray();
	}
	//private:
public:
	void addByte()
	{
		//still in dev
	}
	void addBytes(int nullsCount)
	{
		//still in dev
	}
	void cutNulls() //totest
	{
		//still in dev
	}
	void cutOneNull()
	{
		//still in dev
	}

	//Внутриклассовые операторы
public:
	friend BigFloat operator++(BigFloat& i);
	friend BigFloat operator--(BigFloat& i);
	friend BigFloat operator++(BigFloat& i, int);
	friend BigFloat operator--(BigFloat& i, int);
	friend BigFloat operator+(const BigFloat& f, const BigFloat& s);
	friend BigFloat operator-(const BigFloat& f, const BigFloat& s);
	friend BigFloat operator*(const BigFloat& f, const BigFloat& s);
	friend BigFloat operator/(const BigFloat& f, const BigFloat& s);
	friend BigFloat operator%(const BigFloat& f, const BigFloat& s);
	friend BigFloat operator+=(BigFloat& f, const BigFloat& s);
	friend BigFloat operator-=(BigFloat& f, const BigFloat& s);
	friend BigFloat operator*=(BigFloat& f, const BigFloat& s);
	friend BigFloat operator/=(BigFloat& f, const BigFloat& s);
	friend BigFloat operator%=(BigFloat& f, const BigFloat& s);
	//Битовые			v&x v<<x v>>x v^x v|x
	friend BigFloat operator&(const BigFloat& i, const BigFloat& j);
	friend BigFloat operator^(const BigFloat& i, const BigFloat& j);
	friend BigFloat operator|(const BigFloat& i, const BigFloat& j);
	friend BigFloat operator>>(BigFloat& f, int s);
	friend BigFloat operator<<(BigFloat& f, int s);
	//Логические		v<x v>x v==x v!=x  (op=)
	friend bool operator<(const BigFloat& f, const BigFloat& s);
	friend bool operator>(const BigFloat& f, const BigFloat& s);
	friend bool operator<=(const BigFloat& f, const BigFloat& s);
	friend bool operator>=(const BigFloat& f, const BigFloat& s);
	friend bool operator==(const BigFloat& f, const BigFloat& s);
	friend bool operator!=(const BigFloat& f, const BigFloat& s);
	//(Остальное)		+v -v v=x v[x](разряд x)  x<<v x>>v
	friend const BigFloat operator+(const BigFloat& i);
	friend const BigFloat operator-(const BigFloat& i);
	const BigFloat& operator=(const BigFloat& i);
	byte operator[](const unsigned int index);
	friend ostream& operator<<(ostream& out, BigFloat& point);
	friend istream& operator>>(istream& in, BigFloat& point);

	//Операторы преобразования
	operator char* () //primitive, totest
	{
		//still in dev
		char* _ = (char*)"";
		return _;
	}

	template <typename T>
	operator T() //totest
	{
		//still in dev
		/*if (!is_arithmetic<T>::value)
			throw typeException(typeid(T).name(), "arithmetic");

		if (is_floating_point<T>::value)
		{
			long long llval = static_cast<long long>(*this);
			T fval = static_cast<T>(llval);
			if (static_cast<long long>(fval) != llval)
				warningHolder.setWarning(string() + "Converted BigFloat bigger than " + typeid(T).name() + " type");

			return fval;
		}

		if (size > sizeof(T))
			warningHolder.setWarning(string() + "Converted BigFloat bigger than " + typeid(T).name() + " type");
		if (is_unsigned<T>::value && sign == negative)
			warningHolder.setWarning("Convert signed type to unsigned");

		T result = 0;
		memcpy(&result, ((void*)(&val)), size);
		if (sign == negative && is_signed<T>::value)
			result *= -1;
		return result;*/
	}
};
string to_string(BigFloat& f) //tested 0<=f<=3000
{
	//still in dev
	return "";
}
BigFloat to_BigFloat(string& value) //tested 0<=f<=3000
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
char to_char(BigFloat& value)
{
	//still in dev
	return ' ';
}

//Операторы
//Инк/декремент		++v --v v++ v--
BigFloat operator++(BigFloat& i)
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator--(BigFloat& i)
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator++(BigFloat& i, int)
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator--(BigFloat& i, int)
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
//Арифметика		+ - * / %  (op=)
BigFloat operator+(const BigFloat& f, const BigFloat& s) //totest
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator-(const BigFloat& f, const BigFloat& s) //totest
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator*(const BigFloat& f, const BigFloat& s) //totest
{
	if (f == 0 || s == 0) return 0;
	if (f == 1) return s;
	if (s == 1) return f;

	BigFloat result = 0;
	for (int i = 0; i < s; i++)
	{
		result += f;
	}

	return result;
}
BigFloat operator+=(BigFloat& f, const BigFloat& s)
{
	f = f + s;
	return f;
}
BigFloat operator-=(BigFloat& f, const BigFloat& s)
{
	f = f - s;
	return f;
}
BigFloat operator*=(BigFloat& f, const BigFloat& s)
{
	f = f * s;
	return f;
}
pair<BigFloat, BigFloat> __div(BigFloat f, const BigFloat& s)
{
	if (f == BigFloat(0))
		return pair<BigFloat, BigFloat>(0, 0);
	if (s == BigFloat(0))
		throw exception("Division on zero");

	BigFloat result = 0;
	while (true)
	{
		if (f >= s)
		{
			f -= s;
			result++;
		}
		else
			break;
	}

	return make_pair(f, result); //%, /
}
BigFloat operator/(const BigFloat& f, const BigFloat& s) //totest
{
	return __div(f, s).second;
}
BigFloat operator%(const BigFloat& f, const BigFloat& s) //totest
{
	return __div(f, s).first;
}
BigFloat operator/=(BigFloat& f, const BigFloat& s)
{
	f = f / s;
	return f;
}
BigFloat operator%=(BigFloat& f, const BigFloat& s)
{
	f = f % s;
	return f;
}
//Битовые			v&x v<<x v>>x v^x v|x
BigFloat operator&(const BigFloat& f, const BigFloat& s) //tested 0<=f<500 0<=s<500
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator^(const BigFloat& f, const BigFloat& s) //tested 0<=f<500 0<=s<500
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator|(const BigFloat& f, const BigFloat& s) //tested 0<=f<500 0<=s<500
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator<<(BigFloat& f, int s) //tested 0<=f<500 0<=s<=8
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
BigFloat operator>>(BigFloat& f, int s) //tested 0<=f<500 0<=s<=8
{
	//still in dev
	BigFloat _ = 0;
	return _;
}
//Логические		v<x v>x v==x v!=x  (op=)
bool operator<(const BigFloat& f, const BigFloat& s) //intested 0<=f<=1001 0<=s<=1001
{
	//still in dev
	return false;
}
bool operator>(const BigFloat& f, const BigFloat& s) //dependent
{
	return s < f;
}
bool operator<=(const BigFloat& f, const BigFloat& s) //dependent
{
	return (f < s || f == s);
}
bool operator>=(const BigFloat& f, const BigFloat& s) //dependent
{
	return (f > s || f == s);
}
bool operator==(const BigFloat& f, const BigFloat& s) //intested 0<=f<=1001 0<=s<=1001
{
	//still in dev
	return false;
}
bool operator!=(const BigFloat& f, const BigFloat& s) //primitive
{
	return !(f == s);
}
//(Остальное)		+v -v v=x v[x](разряд x) x<<v x>>v
const BigFloat operator+(const BigFloat& f) //inprimitive
{
	return f;
}
const BigFloat operator-(const BigFloat& f) //inprimitive
{
	//still in dev
	return false;
}
const BigFloat& BigFloat::operator=(const BigFloat& i) //inprimitive, tested 0<=i<=1000000
{
	//still in dev
	return false;
}
byte BigFloat::operator[](const unsigned int index)
{
	//still in dev
	return false;
}
ostream& operator<<(ostream& out, BigFloat& f) //primitive, totest
{
	out << to_string(f);
	return out;
}
istream& operator>>(istream& in, BigFloat& f) //primitive, totest
{
	string input;
	in >> input;
	f = BigFloat(input);
	return in;
}
//Неиспользованные	!v &v v&&x v||x v() (v,x) v->x



//Неотрицательные числа



//Дроби








template<typename T>
constexpr bool is_BigFloat(T)
{
	return (is_same<T, BigFloat>::value);
}

template<typename T>
constexpr bool is_arithmetic_or_BigFloat(T)
{
	return (is_arithmetic<T>::value || is_same<T, BigFloat>::value);
}
template<typename T>
constexpr bool is_signed_or_BigFloat(T)
{
	return (is_signed<T>::value || is_same<T, BigFloat>::value);
}

//is_float //still in dev