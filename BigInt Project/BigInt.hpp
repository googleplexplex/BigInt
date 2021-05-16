#pragma once
#include <vector>
#include "functions.hpp"
using namespace std;

template<typename T>
T Abs(T num);
template<typename T>
T Pow(const T& base, const T& exp);
class BigInt;
pair<BigInt, BigInt> __div(BigInt f, const BigInt& s);
string to_string(BigInt& f);
BigInt to_BigInt(string& value);
template<typename T>
bool is_Arithmetic(T);

struct __warningHolderStruct;

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

	//������������
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
		if constexpr (!is_arithmetic<startValueType>::value && is_same<startValueType, BigInt>::value)
			throw notArithmeticTypeException("startValueType");

		for (int i = sizeof(value) - 1; true; i--)
		{
			if (GET_BYTE(value, i) != NULL || i == 0)
			{
				this->size = i + 1;
				val.assign(this->size, 0);
				sign = (value >= 0) ? (positive) : (negative);
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

	//������
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
private:
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

	//��������������� ���������
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
	//�������			v&x v<<x v>>x v^x v|x
	friend BigInt operator&(const BigInt& i, const BigInt& j);
	friend BigInt operator^(const BigInt& i, const BigInt& j);
	friend BigInt operator|(const BigInt& i, const BigInt& j);
	friend BigInt operator>>(BigInt& f, int s);
	friend BigInt operator<<(BigInt& f, int s);
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
	operator char*() //dependent, primitive, totest
	{
		char* result = new char[size];
		string resultStr = to_string(*this);
		memcpy(result, resultStr.c_str(), size);

		return result;
	}

	operator unsigned long long int() //totest
	{
		if(sign == negative && size > sizeof(unsigned long long int))
			warningHolder.setWarning("Convert signed type to unsigned + Converted BigInt bigger than unsigned long long type");
		else
		{
			if (sign == negative)
				warningHolder.setWarning("Convert signed type to unsigned");
			if (size > sizeof(unsigned long long int))
				warningHolder.setWarning("Converted BigInt bigger than unsigned long long type");
		}
		
		unsigned long long int result = 0;
		memcpy(&result, &val[0], size);
		return result;
	}
	operator long long int() //tested -500000<=v<=500000
	{
		if(size > sizeof(long long int))
			warningHolder.setWarning("Converted BigInt bigger than long long type");

		long long int result = 0;
		memcpy(&result, &val[0], size);
		if (!sign)
			result *= -1;
		return result;
	}
	operator unsigned int()
	{
		//still in dev

		return 0;
	}
	operator int()
	{
		//still in dev

		return 0;
	}
	operator unsigned short()
	{
		//still in dev

		return 0;
	}
	operator short()
	{
		//still in dev

		return 0;
	}
	operator unsigned char() //This is byte() too
	{
		//still in dev

		return 0;
	}
	operator char()
	{
		//still in dev

		return 0;
	}
	//(����� ��������� ���)
};
string to_string(BigInt& f) //dependent, tested 0<=f<=3000
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
BigInt to_BigInt(string& value) //dependent, tested 0<=f<=3000
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
	if(value.getSign() == negative)

	if (value.sign == positive && value <= BigInt(9))
		return '0' + value.getByteArray()[0];
	else
		throw exception("still in dev!");
}

//���������
//���/���������		++v --v v++ v--
BigInt operator++(BigInt& i) //dependent, totest
{
	i = i + BigInt(1);
	return i;
}
BigInt operator--(BigInt& i) //dependent, totest
{
	i = i - BigInt(1);
	return i;
}
BigInt operator++(BigInt& i, int) //dependent, totest
{
	BigInt result = i;
	i = i + BigInt(1);
	return result;
}
BigInt operator--(BigInt& i, int) //dependent, totest
{
	BigInt result = i;
	i = i - BigInt(1);
	return result;
}
//����������		+ - * / %  (op=)
BigInt operator+(const BigInt& f, const BigInt& s) //dependent, totest
{
	//���������� ���������
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

	result.cutOneNull(); //indev!!

	return result;
}
BigInt operator-(const BigInt& f, const BigInt& s) //dependent, totest
{
	//���������� ���������
	if (!f.sign && s.sign) //-f - s = -(f + s) = -(-(-f) + s)
		return -(-f + s);
	if (f.sign && !s.sign) //f - (-s) = f + s = f + -(-s)
		return f + -s;
	if (!f.sign && !s.sign) //-f - (-s) = -f + s = s - f = -(-s) - -(-f)
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

	result.cutOneNull(); //indev!!

	return result;
}
BigInt operator*(const BigInt& f, const BigInt& s) //dependent, dev, totest
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
BigInt operator+=(BigInt& f, const BigInt& s) //dependent, primitive
{
	f = f + s;
	return f;
}
BigInt operator-=(BigInt& f, const BigInt& s) //dependent, primitive
{
	f = f - s;
	return f;
}
BigInt operator*=(BigInt& f, const BigInt& s)
{
	f = f * s;
	return f;
}
pair<BigInt, BigInt> __div(BigInt f, const BigInt& s) //dependent, totest
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
BigInt operator/(const BigInt& f, const BigInt& s) //dependent, primitive, totest
{
	return __div(f, s).second;
}
BigInt operator%(const BigInt& f, const BigInt& s) //dependent, totest
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
//�������			v&x v<<x v>>x v^x v|x
BigInt operator&(const BigInt& f, const BigInt& s) //dependent, tested 0<=f<500 0<=s<500
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
BigInt operator^(const BigInt& f, const BigInt& s) //dependent, tested 0<=f<500 0<=s<500
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
BigInt operator|(const BigInt& f, const BigInt& s) //dependent, tested 0<=f<500 0<=s<500
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
BigInt operator<<(BigInt& f, int s) //dependent, tested 0<=f<500 0<=s<=8
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
BigInt operator>>(BigInt& f, int s) //dependent, tested 0<=f<500 0<=s<=8
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
//����������		v<x v>x v==x v!=x  (op=)
bool operator<(const BigInt& f, const BigInt& s) //independent, tested 0<=f<=1001 0<=s<=1001
{
	//���������� ���������
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
bool operator==(const BigInt& f, const BigInt& s) //independent, tested 0<=f<=1001 0<=s<=1001
{
	//���������� ���������
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
bool operator!=(const BigInt& f, const BigInt& s) //dependent, primitive
{
	return !(f == s);
}
//(���������)		+v -v v=x v[x](������ x) x<<v x>>v
const BigInt operator+(const BigInt& f) //independent, primitive
{
	return f;
}
const BigInt operator-(const BigInt& f) //independent, primitive
{
	BigInt result = f;
	//result.sign = !result.sign;
	return result;
}
const BigInt& BigInt::operator=(const BigInt& i) //independent, primitive, tested 0<=i<=1000000
{
	this->size = i.size;
	this->sign = i.sign;
	this->val = i.val;

	return i;
}
const BigInt& BigInt::operator[](const int index)
{
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
	if (!is_Arithmetic(num))
		throw notArithmeticTypeException("T");

	if (is_unsigned<T>::value || num >= 0)
		return num;
	else //T is signed && num < 0
		return 0 - num;
}

template<typename T>
T Pow(const T& base, const T& exp) //indev, totest
{
	if (!is_Arithmetic(base))
		throw notArithmeticTypeException("T");

	if (exp == 0) return 1;
	if (exp == 1) return base;

	T result = 1;

	for (T i = 0; i < exp; i++)
	{
		result *= base;
	}

	return result;
}

template<typename T>
bool is_Arithmetic(T)
{
	return (is_arithmetic<T>::value || is_same<T, BigInt>::value);
}