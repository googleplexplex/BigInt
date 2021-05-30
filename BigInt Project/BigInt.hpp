#pragma once
#include <vector>
#include <string>
#include "functions.hpp"
using namespace std;

class BigInt;
pair<BigInt, BigInt> __div(BigInt f, const BigInt& s);
string to_string(BigInt& f);
BigInt to_BigInt(string& value);

typedef enum intNegative : byte
{
	negative = BYTE_MIN,
	positive = BYTE_MIN + 1
};

class BigInt
{
public:
	uint size;
	intNegative sign;
	vector<byte> val;

	//Конструкторы
public:
	BigInt()
	{
		size = 1;
		sign = positive;
		val.push_back(BYTE_MIN);
	}

	BigInt(string value) //indev, tested 0<=value<=3000
	{
		(*this) = to_BigInt(value);
	}
	
	template <typename startValueType>
	BigInt(startValueType value = 0)
	{
		if constexpr (!is_arithmetic<startValueType>::value)
			throw notArithmeticTypeException("startValueType");

		for (int i = sizeof(value) - 1; true; i--)
		{
			if (GET_BYTE(value, i) != NULL || i == 0)
			{
				this->size = i + 1;
				val.assign(this->size, 0);
				bool valueSign = (value >= int(0));
				sign = (valueSign) ? (positive) : (negative);
				startValueType valueModule = Abs(value);
				memcpy(&(val[0]), &valueModule, this->size);
				break;
			}
		}
	}

	BigInt(vector<byte>& value, intNegative sign = positive) //totest
	{
		size = value.size();
		this->sign = sign;
		val = value;
	}

	BigInt(BigUInt& value);

	//Методы
public:
	uint getSize()
	{
		return size;
	}
	intNegative getSign()
	{
		return sign;
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
	friend BigInt operator++(BigInt& i);
	friend BigInt operator--(BigInt& i);
	friend BigInt operator++(BigInt& i, int);
	friend BigInt operator--(BigInt& i, int);
	friend BigInt operator+(const BigInt& f, const BigInt& s);
	friend BigInt operator-(const BigInt& f, const BigInt& s);
	friend BigInt operator*(const BigInt& f, const BigInt& s);
	friend BigInt operator/(const BigInt& f, const BigInt& s);
	friend BigInt operator%(const BigInt& f, const BigInt& s);
	friend BigInt operator+=(BigInt& f, const BigInt& s);
	friend BigInt operator-=(BigInt& f, const BigInt& s);
	friend BigInt operator*=(BigInt& f, const BigInt& s);
	friend BigInt operator/=(BigInt& f, const BigInt& s);
	friend BigInt operator%=(BigInt& f, const BigInt& s);
	//Битовые			v&x v<<x v>>x v^x v|x
	friend BigInt operator&(const BigInt& i, const BigInt& j);
	friend BigInt operator^(const BigInt& i, const BigInt& j);
	friend BigInt operator|(const BigInt& i, const BigInt& j);
	friend BigInt operator>>(BigInt& f, int s);
	friend BigInt operator<<(BigInt& f, int s);
	//Логические		v<x v>x v==x v!=x  (op=)
	friend bool operator<(const BigInt& f, const BigInt& s);
	friend bool operator>(const BigInt& f, const BigInt& s);
	friend bool operator<=(const BigInt& f, const BigInt& s);
	friend bool operator>=(const BigInt& f, const BigInt& s);
	friend bool operator==(const BigInt& f, const BigInt& s);
	friend bool operator!=(const BigInt& f, const BigInt& s);
	//(Остальное)		+v -v v=x v[x](разряд x)  x<<v x>>v
	friend const BigInt operator+(const BigInt& i);
	friend const BigInt operator-(const BigInt& i);
	const BigInt& operator=(const BigInt& i);
	byte operator[](const unsigned int index);
	friend ostream& operator<<(ostream& out, BigInt& point);
	friend istream& operator>>(istream& in, BigInt& point);

	//Операторы преобразования
	operator char*() //primitive, totest
	{
		char* result = new char[size];
		string resultStr = to_string(*this);
		memcpy(result, resultStr.c_str(), size);

		return result;
	}

	template <typename T>
	operator T() //totest
	{
		if(!is_arithmetic<T>::value)
			throw typeException(typeid(T).name(), "arithmetic");

		if (is_floating_point<T>::value)
		{
			long long llval = static_cast<long long>(*this);
			T fval = static_cast<T>(llval);
			if(static_cast<long long>(fval) != llval)
				warningHolder.setWarning(string() + "Converted BigInt bigger than " + typeid(T).name() + " type");

			return fval;
		}

		if (size > sizeof(T))
			warningHolder.setWarning(string() + "Converted BigInt bigger than " + typeid(T).name() + " type");
		if (is_unsigned<T>::value && sign == negative)
			warningHolder.setWarning("Convert signed type to unsigned");

		T result = 0;
		memcpy(&result, ((void*)(&val)), size);
		if (sign == negative && is_signed<T>::value)
			result *= -1;
		return result;
	}
};
string to_string(BigInt& f) //tested 0<=f<=3000
{
	string result = "";
	BigInt workAt = f;

	while (workAt != BigInt(0))
	{
		auto divResult = __div(workAt, 10);
		result.push_back(char(divResult.first));
		workAt = divResult.second;
	}
	reverse(result.begin(), result.end());

	if (result == "")
		return "0";

	return result;
}
BigInt to_BigInt(string& value) //tested 0<=f<=3000
{
	BigInt result;

	if (value[0] == NULL)
		return BigInt(0);
	if (value[0] == '-') //totest!!
	{
		result = -result;
		value.erase(value.begin());
	}

	for (int i = 0; i < value.size(); i++)
	{
		BigInt digitNum = BigInt(value[i] - '0');
		BigInt powerOfTen = Pow(BigInt(10), BigInt(value.size() - i - 1));
		result += digitNum * powerOfTen;
	}

	return result;
}
char to_char(BigInt& value)
{
	if (value.getSign() == negative)
	{
		warningHolder.setWarning("Convert signed negative type to charset");
	}
	if (value > BigInt(9))
	{
		warningHolder.setWarning("Converted BigInt has more than one digit");
	}

	return '0' + value.getByteArray()[0];
}

//Операторы
//Инк/декремент		++v --v v++ v--
BigInt operator++(BigInt& i)
{
	i = i + BigInt(1);
	return i;
}
BigInt operator--(BigInt& i)
{
	i = i - BigInt(1);
	return i;
}
BigInt operator++(BigInt& i, int)
{
	BigInt result = i;
	i = i + BigInt(1);
	return result;
}
BigInt operator--(BigInt& i, int)
{
	BigInt result = i;
	i = i - BigInt(1);
	return result;
}
//Арифметика		+ - * / %  (op=)
BigInt operator+(const BigInt& f, const BigInt& s) //totest
{
	//Валидируем аргументы
	if (!f.sign && s.sign) //(-f) + s = s - (-(-f))
		return s - (-f);
	if (f.sign && !s.sign) //f + (-s) = f - (-(-s))
		return f - (-s);
	if (!f.sign && !s.sign) //(-f) + (-s) = -((-(-f)) + (-(-s)))
		return -((-f) + (-s));
	//f >= 0, s >= 0

	BigInt result;
	result.size = ((f >= s) ? (f.size) : (s.size)) + 1;
	result.val.assign(result.size, 0);

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
BigInt operator-(const BigInt& f, const BigInt& s) //totest
{
	//Валидируем аргументы
	if (!f.sign && s.sign) //-f - s = -(f + s) = -(-(-f) + s)
		return -(-f + s);
	if (f.sign && !s.sign) //f - (-s) = f + s = f + -(-s)
		return f + -s;
	if (!f.sign && !s.sign) //-f - (-s) = -f + s = s - f = -(-s) - -(-f)
		return -s - -f;
	if (s > f) //f - s = -(s - f)  (Только если s > f)
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

	result.cutOneNull(); //indev!!

	return result;
}
BigInt operator*(const BigInt& f, const BigInt& s) //totest
{
	if (f == 0 || s == 0) return 0;
	if (f == 1) return s;
	if (s == 1) return f;

	BigInt result = 0;
	for (int i = 0; i < s; i++)
	{
		result += f;
	}

	return result;
}
BigInt operator+=(BigInt& f, const BigInt& s)
{
	f = f + s;
	return f;
}
BigInt operator-=(BigInt& f, const BigInt& s)
{
	f = f - s;
	return f;
}
BigInt operator*=(BigInt& f, const BigInt& s)
{
	f = f * s;
	return f;
}
pair<BigInt, BigInt> __div(BigInt f, const BigInt& s)
{
	if (f == BigInt(0))
		return pair<BigInt, BigInt>(0, 0);
	if (s == BigInt(0))
		throw exception("Division on zero");

	BigInt result = 0;
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
BigInt operator/(const BigInt& f, const BigInt& s) //totest
{
	return __div(f, s).second;
}
BigInt operator%(const BigInt& f, const BigInt& s) //totest
{
	return __div(f, s).first;
}
BigInt operator/=(BigInt& f, const BigInt& s)
{
	f = f / s;
	return f;
}
BigInt operator%=(BigInt& f, const BigInt& s)
{
	f = f % s;
	return f;
}
//Битовые			v&x v<<x v>>x v^x v|x
BigInt operator&(const BigInt& f, const BigInt& s) //tested 0<=f<500 0<=s<500
{
	int minSize = (f > s) ? (s.size) : (f.size);
	const BigInt& biggestInt = (f > s) ? (f) : (s);
	BigInt result;
	result.size = biggestInt.size;
	result.val.assign(result.size, 0);

	for (int i = 0; i < minSize; i++)
	{
		result.val[i] = f.val[i] & s.val[i];
	}
	if(biggestInt.size != minSize)
		memset(&(result.val[minSize]), 0, biggestInt.size - minSize); // v AND 0 = 0

	return result;
}
BigInt operator^(const BigInt& f, const BigInt& s) //tested 0<=f<500 0<=s<500
{
	int minSize = (f > s) ? (s.size) : (f.size);
	const BigInt& biggestInt = (f > s) ? (f) : (s);
	BigInt result;
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
BigInt operator|(const BigInt& f, const BigInt& s) //tested 0<=f<500 0<=s<500
{
	int minSize = (f > s) ? (s.size) : (f.size);
	const BigInt& biggestInt = (f > s) ? (f) : (s);
	BigInt result;
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
BigInt operator<<(BigInt& f, int s) //tested 0<=f<500 0<=s<=8
{
	BigInt result;
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
BigInt operator>>(BigInt& f, int s) //tested 0<=f<500 0<=s<=8
{
	BigInt result;
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
bool operator<(const BigInt& f, const BigInt& s) //intested 0<=f<=1001 0<=s<=1001
{
	//Валидируем аргументы
	if (!f.sign && s.sign) //-f < s
		return true;
	if (f.sign && !s.sign) //f < -s
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
	return s < f;
}
bool operator<=(const BigInt& f, const BigInt& s) //dependent
{
	return (f < s || f == s);
}
bool operator>=(const BigInt& f, const BigInt& s) //dependent
{
	return (f > s || f == s);
}
bool operator==(const BigInt& f, const BigInt& s) //intested 0<=f<=1001 0<=s<=1001
{
	//Валидируем аргументы
	if (!f.sign && s.sign) //-f < s
		return false;
	if (f.sign && !s.sign) //f < -s
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
bool operator!=(const BigInt& f, const BigInt& s) //primitive
{
	return !(f == s);
}
//(Остальное)		+v -v v=x v[x](разряд x) x<<v x>>v
const BigInt operator+(const BigInt& f) //inprimitive
{
	return f;
}
const BigInt operator-(const BigInt& f) //inprimitive
{
	BigInt result = f;
	result.sign = (result.sign == positive) ? (negative) : (positive);
	return result;
}
const BigInt& BigInt::operator=(const BigInt& i) //inprimitive, tested 0<=i<=1000000
{
	this->size = i.size;
	this->sign = i.sign;
	this->val = i.val;

	return i;
}
byte BigInt::operator[](const unsigned int index)
{
	if (index >= size)
		throw exception("Segmentation fault");

	return val[index];
}
ostream& operator<<(ostream& out, BigInt& f) //primitive, totest
{
	out << to_string(f);
	return out;
}
istream& operator>>(istream& in, BigInt& f) //primitive, totest
{
	string input;
	in >> input;
	f = BigInt(input);
	return in;
}
//Неиспользованные	!v &v v&&x v||x v() (v,x) v->x



//Неотрицательные числа



//Дроби








template<typename T>
constexpr bool is_BigInt(T)
{
	return (is_same<T, BigInt>::value);
}

template<typename T>
constexpr bool is_arithmetic_or_BigInt(T)
{
	return (is_arithmetic<T>::value || is_same<T, BigInt>::value);
}
template<typename T>
constexpr bool is_signed_or_BigInt(T)
{
	return (is_signed<T>::value || is_same<T, BigInt>::value);
}