#pragma once
using namespace std;

#define GET_BYTE(x, pos) ((byte*)(&(x)))[(pos)]
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