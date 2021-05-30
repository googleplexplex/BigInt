#pragma once
#include <string>
#include "BigInt.hpp"
#include "BigUInt.hpp"

BigInt::BigInt(BigUInt& value)
{
	size = value.getSize();
	sign = positive;
	val = value.getByteArray();
}


BigUInt::BigUInt(BigInt& value)
{
	if (value.sign == negative)
		warningHolder.setWarning("Converted negative BigInt to BigUInt");

	size = value.size;
	val = value.val;
}
