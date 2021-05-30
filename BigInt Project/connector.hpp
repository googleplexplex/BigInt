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

//BigInt::BigInt(BigFloat& value)
//{
//	if(value.fract != BigUInt(0))
//		warningHolder.setWarning("Converted BigFloat with not empty fractial to BigInt");
//
//	size = value.whole.getSize();
//	sign = value.getSign();
//	val = value.whole.getByteArray();
//}


BigUInt::BigUInt(BigInt& value)
{
	if (value.sign == negative)
		warningHolder.setWarning("Converted negative BigInt to BigUInt");

	this->size = value.size;
	this->val = value.val;
}

//BigUInt::BigUInt(BigFloat& value)
//{
//	if (value.fract != BigUInt(0))
//		warningHolder.setWarning("Converted BigFloat with not empty fractial to BigUInt");
//	if (value.getSign() == negative)
//		warningHolder.setWarning("Converted negative BigInt to BigUInt");
//
//	this->size = value.whole.getSize();
//	this->val = value.whole.getByteArray();
//}


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