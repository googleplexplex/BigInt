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
		string wholeStr = to_string(whole);
		string fractStr = to_string(fract);
		string result = wholeStr + '.' + fractStr;

		return &(result[0]);
	}

	template <typename T>
	operator T() //totest
	{
		if (!is_arithmetic<T>::value)
			throw typeException(typeid(T).name(), "arithmetic");

		if (is_floating_point<T>::value)
		{
			long long fractll = static_cast<long long>(fract);
			long long wholell = static_cast<long long>(whole);
			T result = fractll;
			result /= Pow(10, fract.getSize());
			result += wholell;
			return result;
		}

		if (size > sizeof(T))
			warningHolder.setWarning(string() + "Converted BigFloat bigger than " + typeid(T).name() + " type");
		if (is_unsigned<T>::value && sign == negative)
			warningHolder.setWarning("Convert signed type to unsigned");

		T result = 0;
		memcpy(&result, ((void*)(&val)), size);
		if (sign == negative && is_signed<T>::value)
			result *= -1;
		return result;
	}
};
string to_string(BigFloat& f) //totest
{
	string wholeStr = to_string(f.whole);
	string fractStr = to_string(f.fract);
	string result = wholeStr + '.' + fractStr;

	return result;
}
BigFloat to_BigFloat(string& value) //totest
{
	BigFloat result;

	int sepPos = value.find('.');
	if (sepPos != string::npos)
	{
		string wholePartStr = value.substr(0, sepPos);
		string fractPartStr = value.substr(sepPos, value.size());

		result.whole = to_BigInt(wholePartStr);
		result.fract = to_BigUInt(fractPartStr);
	}
	else {
		result.whole = to_BigInt(value);
		result.fract = BigUInt(0);
	}

	return result;
}
char to_char(BigFloat& value)
{
	return to_char(value.whole);
}

//Операторы
//Инк/декремент		++v --v v++ v--
BigFloat operator++(BigFloat& i)
{
	i = i + BigFloat(1);
	return i;
}
BigFloat operator--(BigFloat& i)
{
	i = i - BigFloat(1);
	return i;
}
BigFloat operator++(BigFloat& i, int)
{
	BigFloat result = i;
	i = i + BigFloat(1);
	return result;
}
BigFloat operator--(BigFloat& i, int)
{
	BigFloat result = i;
	i = i - BigFloat(1);
	return result;
}
//Арифметика		+ - * / %  (op=)
BigFloat operator+(BigFloat& f, BigFloat& s) //totest, indev
{
	BigUInt maxFract = (f.fract.size > s.fract.size) ? (f.fract) : (s.fract);
	BigUInt minFract = (f.fract.size > s.fract.size) ? (s.fract) : (f.fract);

	minFract.addBytes(maxFract.size - minFract.size);
	int maxFractSize = maxFract.size;
	BigUInt resultFract = minFract + maxFract;
	BigUInt overflowedResultPeace = 0;
	if (resultFract.size > maxFract.size)
	{
		int difference = resultFract.size - maxFract.size;
		vector<byte>::const_iterator first = resultFract.val.begin();
		vector<byte>::const_iterator last = resultFract.val.begin() + difference;
		vector<byte> overflowedResultPeaceVector(first, last);

		resultFract.val.erase(first, last);
		resultFract.size -= difference;

		overflowedResultPeace = BigUInt(overflowedResultPeaceVector);
	}

	BigInt resultWhole = f.whole + s.whole + BigInt(overflowedResultPeace);

	BigFloat result;
	result.whole = resultWhole;
	result.fract = resultFract;
	return result;
}
BigFloat operator-(const BigFloat& f, const BigFloat& s) //totest
{
	BigFloat result;
	result.whole = f.whole - s.whole;

	BigUInt ffract = f.fract;
	BigUInt sfract = s.fract;
	if (ffract.size != sfract.size)
	{
		int difference = Abs(ffract.size - sfract.size);
		if (ffract.size > sfract.size)
			sfract.addBytes(difference);
		else
			ffract.addBytes(difference);
	}
	result.fract = ffract - sfract;
	return result;
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
BigFloat operator&(const BigFloat& f, const BigFloat& s) //totest
{
	BigFloat result = f;

	result.whole = result.whole & s.whole;
	result.fract = result.fract & s.fract;

	return result;
}
BigFloat operator^(const BigFloat& f, const BigFloat& s) //totest
{
	BigFloat result = f;

	result.whole = result.whole ^ s.whole;
	result.fract = result.fract ^ s.fract;

	return result;
}
BigFloat operator|(const BigFloat& f, const BigFloat& s) //totest
{
	BigFloat result = f;

	result.whole = result.whole | s.whole;
	result.fract = result.fract | s.fract;

	return result;
}
//Логические		v<x v>x v==x v!=x  (op=)
bool operator<(const BigFloat& f, const BigFloat& s) //totest
{
	if (f.whole < s.whole)
		return true;
	else if (f.whole == s.whole)
	{
		if (f.fract < s.fract)
			return true;
		else if (f.fract == s.fract)
			return false;
		else // f > s
			return false;
	}
	else { //f > s
		return false;
	}

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
	if (f.whole == s.whole)
	{
		if (f.fract == s.fract)
			return true;
		else
			return false;
	}
	else
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
	BigFloat result = f;
	result.whole.sign = (result.whole.sign == positive) ? (negative) : (positive);
	return result;
}
const BigFloat& BigFloat::operator=(const BigFloat& i) //inprimitive, tested 0<=i<=1000000
{
	this->whole = i.whole;
	this->fract = i.fract;
	return i;
}
byte BigFloat::operator[](const unsigned int index) //totest
{
	if (index < whole.size)
	{
		return whole[index];
	}
	else if (index < whole.size + fract.size)
	{
		return fract[index - whole.size];
	}
	
	throw exception("Segmentation fault");
	return BYTE_MIN;
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

template<typename T>
constexpr bool is_floating_point_or_BigFloat(T)
{
	return (is_floating_point<T>::value || is_same<T, BigFloat>::value);
}