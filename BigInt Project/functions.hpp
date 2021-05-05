#pragma once
using namespace std;

#define GET_BYTE(x, pos) ((byte*)(&(x)))[(pos)]
#define GET_BIT(x, pos) (bool((1 << pos)  &  x))
#define SET_BIT_ONE(x, pos) (x |= (1 << pos));
#define SET_BIT_NULL(x, pos) (x &= ~(1 << pos));
#define SET_BIT(x, pos, val) if(val == 1) SET_BIT_ONE(x, pos) else SET_BIT_NULL(x, pos);
#define nop() __asm { nop }

using uint = unsigned int;
using byte = unsigned char;
#define BYTE_MAX ((byte)UCHAR_MAX)
#define BYTE_MIN ((byte)0)

class notArithmeticTypeException : public exception
{
public:
	notArithmeticTypeException() : exception("Not arithmetic type") {}
	notArithmeticTypeException(string typeName) : exception((typeName + " is not arithmetic type").c_str()) {}
};