// Math.cpp
//

//#include "Master.h"
#include "Math.h"

float two = 2.0f;
unsigned int CMath::fast_sqrt_table[0x10000];


void CMath::InitStaticMembers()
{
	typedef union FastSqrtUnion
	{
	  float f;
	  unsigned int i;
	} FastSqrtUnion;

	FastSqrtUnion s;
	unsigned int i;

	// First initialize the square root lookup table
	for (i = 0; i <= 0x7FFF; i++)
	{

		// Build a float with the bit pattern i as mantissa
		//  and an exponent of 0, stored as 127
		s.i = (i << 8) | (0x7F << 23);
		s.f = (float)sqrt(s.f);

		// Take the square root then strip the first 7 bits of
		//  the mantissa into the table
		fast_sqrt_table[i + 0x8000] = (s.i & 0x7FFFFF);

		// Repeat the process, this time with an exponent of 1, 
		//  stored as 128
		s.i = (i << 8) | (0x80 << 23);
		s.f = (float)sqrt(s.f);

		fast_sqrt_table[i] = (s.i & 0x7FFFFF);
	}
}
