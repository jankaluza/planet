// Math.h
//

#ifndef __Math_h__
#define __Math_h__

#include <math.h>

#define FP_BITS(fp) (*(unsigned long *)&(fp))
#define FP_ABS_BITS(fp) (FP_BITS(fp)&0x7FFFFFFF)
#define FP_SIGN_BIT(fp) (FP_BITS(fp)&0x80000000)
#define FP_ONE_BITS 0x3F800000

extern float two;

class CMath
{
protected:
	static unsigned int fast_sqrt_table[0x10000];

public:
	static void InitStaticMembers();

	static float inv(float f)			{ return 1.0f/f;}
	static float sqrt(float f)			{ return sqrtf(f); }
	static float pow(float f, float e)	{ return powf(f, e); }
	static float exp(float f)			{ return expf(f); }
	static float cos(float f)			{ return cosf(f); }
	static float sin(float f)			{ return sinf(f); }
	static float acos(float f)			{ return acosf(f); }
	static float asin(float f)			{ return asinf(f); }
	static int toint(float f)			{ return (int)f; }
	
	static float fastsqrt(float f)
	{
		unsigned long bits = FP_BITS(f);
		if(bits == 0)
			return 0.0;                 // check for square root of 0
		FP_BITS(f) = fast_sqrt_table[(bits >> 8) & 0xFFFF] | ((((bits - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);
		return f;
	}
};


#endif // __Math_h__
