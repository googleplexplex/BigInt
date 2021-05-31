#pragma once
#include <string>
#include "BigInt.hpp"
#include "BigUInt.hpp"
#include "BigFloat.hpp"

BigInt::BigInt(BigUInt& value)
{
	this->size = value.getSize();
	this->sign = positive;
	this->val = value.getByteArray();
}

BigInt to_BigFloat(BigFloat& value)
{
	BigInt result;

	if (value.fract != BigUInt(0))
				warningHolder.setWarning("Converted BigFloat with not empty fractial to BigInt");
		
	result.size = value.whole.getSize();
	result.sign = value.getSign();
	result.val = value.whole.getByteArray();

	return result;
}


BigUInt::BigUInt(BigInt& value)
{
	if (value.sign == negative)
		warningHolder.setWarning("Converted negative BigInt to BigUInt");

	this->size = value.size;
	this->val = value.val;
}

BigUInt to_BigUInt(BigFloat& value)
{
	BigUInt result;

	if (value.fract != BigUInt(0))
		warningHolder.setWarning("Converted BigFloat with not empty fractial to BigUInt");
	if (value.getSign() == negative)
		warningHolder.setWarning("Converted negative BigInt to BigUInt");

	result.size = value.whole.getSize();
	result.val = value.whole.getByteArray();

	return result;
}


BigFloat::BigFloat(BigInt& value)
{
	this->whole = value;
	this->fract = BigUInt(0);
}

BigFloat::BigFloat(BigUInt& value)
{
	this->whole = BigInt(value);
	this->fract = BigUInt(0);
}