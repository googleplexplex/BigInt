#pragma once
using namespace std;

#define GET_BYTE(x, pos) ((byte*)(&(x)))[(pos)]
#define GET_BIT(x, pos) (bool((1 << (pos)) & (x)))
#define SET_BIT_ONE(x, pos) ((x) |= (1 << (pos)));
#define SET_BIT_NULL(x, pos) ((x) &= ~(1 << (pos)));
#define SET_BIT(x, pos, val) if((val) == true) SET_BIT_ONE(x, pos) else SET_BIT_NULL(x, pos);
#define nop() __asm { nop }

using uint = unsigned int;
using byte = unsigned char;
#define BYTE_MAX ((byte)UCHAR_MAX)
#define BYTE_MIN ((byte)0)

struct typeException : public exception
{
public:
	typeException() : exception("Wrong type") {}
	typeException(string typeName) : exception((typeName + " is wrong type").c_str()) {}
	typeException(string typeName, string waitedType) : exception((typeName + " is not waited " + waitedType +" type").c_str()) {}
};

struct notArithmeticTypeException : public typeException
{
public:
	notArithmeticTypeException() : typeException("Argument type", "arithmetic") {}
	notArithmeticTypeException(string typeName) : typeException(typeName, "arithmetic") {}
};

template<typename T>
bool is_Arithmetic(T)
{
	return is_arithmetic<T>::value;
}

template<typename T>
T Abs(T num)
{
	if (!is_Arithmetic(num))
		throw notArithmeticTypeException("T");

	if (is_unsigned<T>::value || num >= int(0))
		return num;
	else //T is signed && num < 0
		return T(int(0) - num);
}

template<typename T>
T Pow(const T& base, const T& exp) //totest
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


struct __warningHolderStruct
{
private:
	string warning;
	byte warningId = 0;

public:
	friend string getLastWarning();
//private:
	void setWarning(string newWarning)
	{
		warning = newWarning;
		warningId++;
	}

	friend class BigInt;
	friend char to_char(BigInt& value);
	friend class BigUInt;
	friend char to_char(BigUInt& value);
} warningHolder;

string getLastWarning()
{
	return warningHolder.warning;
}


typedef enum intNegative : byte
{
	negative = BYTE_MIN,
	positive = BYTE_MIN + 1
};