#include <iostream>
#include "BigInt.hpp"
using namespace std;

int main()
{
	long long int llia = LLONG_MAX;
	BigInt a(16, llia);

	long long int llib = 260;
	BigInt b(1, llib);

	return 0;
}