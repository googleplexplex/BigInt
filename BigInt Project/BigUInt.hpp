#pragma once
#include <vector>
#include <string>
#include "functions.hpp"
using namespace std;

class BigUInt;
pair<BigUInt, BigUInt> __div(BigUInt f, const BigUInt& s);
string to_string(BigUInt& f);
BigUInt to_BigUInt(string& value);

class BigUInt
{
public:
	uint size;
	vector<byte> val;

	//Конструкторы
public:
	BigUInt()
	{
		size = 1;
		val.push_back(BYTE_MIN);
	}

	BigUInt(string value) //indev, tested 0<=value<=3000
	{
		(*this) = to_BigUInt(value);
	}

	template <typename startValueType>
	BigUInt(startValueType value = 0)
	{
		if constexpr (!is_arithmetic<startValueType>::value && is_same<startValueType, BigUInt>::value)
			throw notArithmeticTypeException("startValueType");

		for (int i = sizeof(value) - 1; true; i--)
		{
			if (GET_BYTE(value, i) != NULL || i == 0)
			{
				this->size = i + 1;
				val.assign(this->size, 0);
				startValueType valueModule = Abs(value);
				memcpy(&(val[0]), &valueModule, this->size);
				break;
			}
		}
	}

	BigUInt(vector<byte>& value) //totest
	{
		size = value.size();
		val = value;
	}

	BigUInt(BigInt& value);

	//Методы
public:
	uint getSize()
	{
		return size;
	}
	vector<byte> getByteArray()
	{
		return val;
	}
//private:
public:
	void addByte()
	{
		size++;
		val.emplace_back(byte(0));
	}
	void addBytes(int nullsCount)
	{
		vector<byte> newVal(size + nullsCount, 0);
		memcpy(&(newVal[0]), &(val[0]), size);
		size += nullsCount;
		val = newVal;
	}
	void cutNulls() //totest
	{
		for (int i = size - 1; true; i--)
		{
			if (i == 0)
			{
				if (val[i] == BYTE_MIN)
				{
					val.assign(1, BYTE_MIN);
					size = 1;
				}
				return;
			}
			if (val[i] != BYTE_MIN)
			{
				vector<byte> newVal(i + 1, BYTE_MIN);
				memcpy(&(newVal[0]), &(val[0]), i + 1);
				val = newVal;
				size = i + 1;
				return;
			}
		}
	}
	void cutOneNull()
	{
		if (val[0] == BYTE_MIN && size == 1)
			return;
		if (val[size - 1] == BYTE_MIN)
		{
			val.erase(val.end() - 1);
			size--;
		}
	}

	//Внутриклассовые операторы
public:
	friend BigUInt operator++(BigUInt& i);
	friend BigUInt operator--(BigUInt& i);
	friend BigUInt operator++(BigUInt& i, int);
	friend BigUInt operator--(BigUInt& i, int);
	friend BigUInt operator+(const BigUInt& f, const BigUInt& s);
	friend BigInt operator-(const BigUInt& f, const BigUInt& s);
	friend BigUInt operator*(const BigUInt& f, const BigUInt& s);
	friend BigUInt operator/(const BigUInt& f, const BigUInt& s);
	friend BigUInt operator%(const BigUInt& f, const BigUInt& s);
	friend BigUInt operator+=(BigUInt& f, const BigUInt& s);
	friend BigInt operator-=(BigUInt& f, const BigUInt& s);
	friend BigUInt operator*=(BigUInt& f, const BigUInt& s);
	friend BigUInt operator/=(BigUInt& f, const BigUInt& s);
	friend BigUInt operator%=(BigUInt& f, const BigUInt& s);
	//Битовые			v&x v<<x v>>x v^x v|x
	friend BigUInt operator&(const BigUInt& i, const BigUInt& j);
	friend BigUInt operator^(const BigUInt& i, const BigUInt& j);
	friend BigUInt operator|(const BigUInt& i, const BigUInt& j);
	friend BigUInt operator>>(BigUInt& f, int s);
	friend BigUInt operator<<(BigUInt& f, int s);
	//Логические		v<x v>x v==x v!=x  (op=)
	friend bool operator<(const BigUInt& f, const BigUInt& s);
	friend bool operator>(const BigUInt& f, const BigUInt& s);
	friend bool operator<=(const BigUInt& f, const BigUInt& s);
	friend bool operator>=(const BigUInt& f, const BigUInt& s);
	friend bool operator==(const BigUInt& f, const BigUInt& s);
	friend bool operator!=(const BigUInt& f, const BigUInt& s);
	//(Остальное)		+v -v v=x v[x](разряд x)  x<<v x>>v
	friend const BigUInt operator+(const BigUInt& i);
	friend const BigInt operator-(const BigUInt& i);
	const BigUInt& operator=(const BigUInt& i);
	const BigUInt& operator=(const BigInt& i);
	byte operator[](const unsigned int index);
	friend ostream& operator<<(ostream& out, BigUInt& point);
	friend istream& operator>>(istream& in, BigUInt& point);

	//Операторы преобразования
	operator char* () //primitive, totest
	{
		char* result = new char[size];
		string resultStr = to_string(*this);
		memcpy(result, resultStr.c_str(), size);

		return result;
	}

	template <typename T>
	operator T() //totest
	{
		if (!is_arithmetic<T>::value)
			throw typeException(typeid(T).name(), "arithmetic");

		if (is_floating_point<T>::value)
		{
			long long llval = static_cast<long long>(*this);
			T fval = static_cast<T>(llval);
			if (static_cast<long long>(fval) != llval)
				warningHolder.setWarning(string() + "Converted BigUInt bigger than " + typeid(T).name() + " type");

			return fval;
		}

		if (size > sizeof(T))
			warningHolder.setWarning(string() + "Converted BigUInt bigger than " + typeid(T).name() + " type");
		if (is_signed<T>::value)
			warningHolder.setWarning("Convert unsigned type to signed");

		T result = 0;
		memcpy(&result, ((void*)(&val)), size);
		return result;
	}
};

string to_string(BigUInt& f) //tested 0<=f<=3000
{
	string result = "";
	BigUInt workAt = f;

	while (workAt != BigUInt(0))
	{
		pair<BigUInt, BigUInt> divResult = __div(workAt, BigUInt(10));
		result.push_back(char(divResult.first));
		workAt = divResult.second;
	}
	reverse(result.begin(), result.end());

	if (result == "")
		return "0";

	return result;
}

BigUInt to_BigUInt(string& value) //tested 0<=f<=3000
{
	BigUInt result;

	if (value[0] == NULL)
		return BigUInt(0);
	if (value[0] == '-') //totest!!
	{
		result = -result;
		value.erase(value.begin());
	}

	for (int i = 0; i < value.size(); i++)
	{
		BigUInt digitNum = BigUInt(value[i] - '0');
		BigUInt powerOfTen = Pow(BigUInt(10), BigUInt(value.size() - i - 1));
		result += digitNum * powerOfTen;
	}

	return result;
}

char to_char(BigUInt& value)
{
	if (value > BigUInt(9))
	{
		warningHolder.setWarning("Converted BigUInt has more than one digit");
	}

	return '0' + value.getByteArray()[0];
}

//Операторы
//Инк/декремент		++v --v v++ v--
BigUInt operator++(BigUInt& i)
{
	i = i + BigUInt(1);
	return i;
}
BigUInt operator--(BigUInt& i)
{
	if (i == BigUInt(0))
	{
		warningHolder.setWarning("Decremented BigUInt equal 0");
		return i;
	}

	i = BigUInt(i - BigUInt(1));
	return i;
}
BigUInt operator++(BigUInt& i, int)
{
	BigUInt result = i;
	i = i + BigUInt(1);
	return result;
}
BigUInt operator--(BigUInt& i, int)
{
	BigUInt result = i;

	if (i == BigUInt(0))
	{
		warningHolder.setWarning("Decremented BigUInt equal 0");
		return result;
	}

	i = BigUInt(i - BigUInt(1));
	return result;
}
//Арифметика		+ - * / %  (op=)
BigUInt operator+(const BigUInt& f, const BigUInt& s) //totest
{
	//f >= 0, s >= 0
	BigUInt result;
	result.size = ((f >= s) ? (f.size) : (s.size)) + 1;
	result.val.assign(result.size, 0);

	const BigUInt& biggerInt = (f >= s) ? (f) : (s);
	const BigUInt& smallerInt = (f >= s) ? (s) : (f);

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
			presentResult -= (BYTE_MAX + 1);
		}

		result.val[i] = byte(presentResult);
	}

	int i;
	for (i = smallerInt.size; i < biggerInt.size && addOne; i++) //Переносим единицу от сложения, если такая осталась
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
		for (; i < biggerInt.size; i++) //Копируем оставшуюся часть большего числа в результат
		{
			result.val[i] = biggerInt.val[i];
		}
	}

	result.cutOneNull(); //indev!!

	return result;
}
BigInt operator-(const BigUInt& f, const BigUInt& s) //totest
{
	//Валидируем аргументы
	if (s > f) //f - s = -(s - f)  (Только если s > f)
		return -(s - f);
	//f >= 0, s >= 0, f >= s


	const BigUInt& biggerInt = (f >= s) ? (f) : (s);
	const BigUInt& smallerInt = (f >= s) ? (s) : (f);
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

	result.cutOneNull(); //indev!!

	return result;
}
BigUInt operator*(const BigUInt& f, const BigUInt& s) //totest
{
	if (f == 0 || s == 0) return 0;
	if (f == 1) return s;
	if (s == 1) return f;

	BigUInt result = 0;
	for (int i = 0; i < s; i++)
	{
		result += f;
	}

	return result;
}
BigUInt operator+=(BigUInt& f, const BigUInt& s)
{
	f = f + s;
	return f;
}
BigInt operator-=(BigUInt& f, const BigUInt& s)
{
	f = f - s;
	return f;
}
BigUInt operator*=(BigUInt& f, const BigUInt& s)
{
	f = f * s;
	return f;
}
pair<BigUInt, BigUInt> __div(BigUInt f, const BigUInt& s)
{
	if (f == BigUInt(0))
		return pair<BigUInt, BigUInt>(0, 0);
	if (s == BigUInt(0))
		throw exception("Division on zero");

	BigUInt result = 0;
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
BigUInt operator/(const BigUInt& f, const BigUInt& s) //totest
{
	return __div(f, s).second;
}
BigUInt operator%(const BigUInt& f, const BigUInt& s) //totest
{
	return __div(f, s).first;
}
BigUInt operator/=(BigUInt& f, const BigUInt& s)
{
	f = f / s;
	return f;
}
BigUInt operator%=(BigUInt& f, const BigUInt& s)
{
	f = f % s;
	return f;
}
//Битовые			v&x v<<x v>>x v^x v|x
BigUInt operator&(const BigUInt& f, const BigUInt& s) //tested 0<=f<500 0<=s<500
{
	int minSize = (f > s) ? (s.size) : (f.size);
	const BigUInt& biggestInt = (f > s) ? (f) : (s);
	BigUInt result;
	result.size = biggestInt.size;
	result.val.assign(result.size, 0);

	for (int i = 0; i < minSize; i++)
	{
		result.val[i] = f.val[i] & s.val[i];
	}
	if (biggestInt.size != minSize)
		memset(&(result.val[minSize]), 0, biggestInt.size - minSize); // v AND 0 = 0

	return result;
}
BigUInt operator^(const BigUInt& f, const BigUInt& s) //tested 0<=f<500 0<=s<500
{
	int minSize = (f > s) ? (s.size) : (f.size);
	const BigUInt& biggestInt = (f > s) ? (f) : (s);
	BigUInt result;
	result.size = biggestInt.size;
	result.val.assign(result.size, 0);

	for (int i = 0; i < minSize; i++)
	{
		result.val[i] = f.val[i] ^ s.val[i];
	}
	for (int i = minSize; i < biggestInt.size; i++)
	{
		result.val[i] = biggestInt.val[i] ^ BYTE_MIN;
	}

	return result;
}
BigUInt operator|(const BigUInt& f, const BigUInt& s) //tested 0<=f<500 0<=s<500
{
	int minSize = (f > s) ? (s.size) : (f.size);
	const BigUInt& biggestInt = (f > s) ? (f) : (s);
	BigUInt result;
	result.size = biggestInt.size;
	result.val.assign(result.size, 0);

	for (int i = 0; i < minSize; i++)
	{
		result.val[i] = f.val[i] | s.val[i];
	}
	for (int i = minSize; i < biggestInt.size; i++)
	{
		result.val[i] = biggestInt.val[i]; // v OR 0 = v
	}

	return result;
}
BigUInt operator<<(BigUInt& f, int s) //tested 0<=f<500 0<=s<=8
{
	BigUInt result;
	result.size = f.size;
	result.val.assign(result.size, 0);

	for (int byteI = 0; byteI < f.size; byteI++)
	{
		for (int bitI = 0; bitI < 8; bitI++)
		{
			int inPos_inBits = byteI * 8 + bitI + s;
			int inPos_bytesOnly = inPos_inBits / 8;
			int inPos_bitsOnly = inPos_inBits % 8;
			int outPos_bytesOnly = byteI;
			int outPos_bitsOnly = bitI;

			if (inPos_bytesOnly >= result.size)
				return result;

			byte& in = result.val[inPos_bytesOnly];
			bool value = GET_BIT(f.val[outPos_bytesOnly], outPos_bitsOnly);
			SET_BIT(in, inPos_bitsOnly, value);
		}
	}

	return result;
}
BigUInt operator>>(BigUInt& f, int s) //tested 0<=f<500 0<=s<=8
{
	BigUInt result;
	result.size = f.size;
	result.val.assign(result.size, 0);

	for (int byteI = 0; byteI < f.size; byteI++)
	{
		for (int bitI = 0; bitI < 8; bitI++)
		{
			int inPos_bytesOnly = byteI;
			int inPos_bitsOnly = bitI;

			int outPos_inBits = byteI * 8 + bitI + s;
			int outPos_bytesOnly = outPos_inBits / 8;
			int outPos_bitsOnly = outPos_inBits % 8;

			if (outPos_bytesOnly >= result.size)
				return result;

			byte& in = result.val[inPos_bytesOnly];
			bool value = GET_BIT(f.val[outPos_bytesOnly], outPos_bitsOnly);
			SET_BIT(in, inPos_bitsOnly, value);
		}
	}

	return result;
}
//Логические		v<x v>x v==x v!=x  (op=)
bool operator<(const BigUInt& f, const BigUInt& s) //intested 0<=f<=1001 0<=s<=1001
{
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

	const BigUInt& smallerInt = (f.size >= s.size) ? (s) : (f);

	for (int i = smallerInt.size - 1; i >= 0; i--)
	{
		if (f.val[i] != s.val[i])
		{
			return f.val[i] < s.val[i];
		}
	}

	return false;
}
bool operator>(const BigUInt& f, const BigUInt& s) //dependent
{
	return s < f;
}
bool operator<=(const BigUInt& f, const BigUInt& s) //dependent
{
	return (f < s || f == s);
}
bool operator>=(const BigUInt& f, const BigUInt& s) //dependent
{
	return (f > s || f == s);
}
bool operator==(const BigUInt& f, const BigUInt& s) //intested 0<=f<=1001 0<=s<=1001
{
	const BigUInt& biggerInt = (f.size >= s.size) ? (f) : (s);
	const BigUInt& smallerInt = (f.size >= s.size) ? (s) : (f);

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
bool operator!=(const BigUInt& f, const BigUInt& s) //primitive
{
	return !(f == s);
}
//(Остальное)		+v -v v=x v[x](разряд x) x<<v x>>v
const BigUInt operator+(const BigUInt& f) //inprimitive
{
	return f;
}
const BigInt operator-(const BigUInt& f) //inprimitive
{
	BigInt result = f;
	result.sign = negative;
	return result;
}
const BigUInt& BigUInt::operator=(const BigUInt& i) //inprimitive, tested 0<=i<=1000000
{
	this->size = i.size;
	this->val = i.val;
	return i;
}
const BigUInt& BigUInt::operator=(const BigInt& i) //inprimitive, totest
{
	if(i.sign == negative)
		warningHolder.setWarning("Converted negative BigInt to BigUInt");

	this->size = i.size;
	this->val = i.val;
	return i;
}
byte BigUInt::operator[](const unsigned int index)
{
	if (index >= size)
		throw exception("Segmentation fault");

	return val[index];
}
ostream& operator<<(ostream& out, BigUInt& f) //primitive, totest
{
	out << to_string(f);
	return out;
}
istream& operator>>(istream& in, BigUInt& f) //primitive, totest
{
	string input;
	in >> input;
	f = BigUInt(input);
	return in;
}
//Неиспользованные	!v &v v&&x v||x v() (v,x) v->x



//Неотрицательные числа



//Дроби






template<typename T>
constexpr bool is_BigUInt(T)
{
	return (is_same<T, BigUInt>::value);
}

template<typename T>
constexpr bool is_arithmetic_or_BigUInt(T)
{
	return (is_arithmetic<T>::value || is_same<T, BigUInt>::value);
}

template<typename T>
constexpr bool is_unsigned_or_BigUInt(T)
{
	return (is_unsigned<T>::value || is_same<T, BigUInt>::value);
}