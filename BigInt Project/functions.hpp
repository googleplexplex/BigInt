#pragma once
using namespace std;

#define GET_BYTE(x, pos) ((byte*)(&(x)))[(pos)]
#define nop() __asm { nop }
#define log(x) //cout << x << endl;

using uint = unsigned int;
using byte = unsigned char;
#define BYTE_MAX UCHAR_MAX
#define BYTE_MIN UCHAR_MIN

class notArithmeticTypeException : public exception
{
public:
	notArithmeticTypeException() : exception("Not arithmetic type") {}
	notArithmeticTypeException(string typeName) : exception((typeName + " is not arithmetic type").c_str()) {}
};