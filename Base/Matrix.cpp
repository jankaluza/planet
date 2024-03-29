/*
	The Universe Development Kit
	Copyright (C) 2000  Sean O'Neil
	s_p_oneil@hotmail.com

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//#include "Master.h"
#include "Matrix.h"


void CMatrix::operator=(const CQuaternion &q)
{
	// 9 muls, 15 adds
	float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
	float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
	float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
	float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;

	f14 = f24 = f34 = f41 = f42 = f43 = 0; f44 = 1;
	f11 = 1-(yy+zz);	f21 = xy-wz;		f31 = xz+wy;
	f12 = xy+wz;		f22 = 1-(xx+zz);	f32 = yz-wx;
	f13 = xz-wy;		f23 = yz+wx;		f33 = 1-(xx+yy);
}

CMatrix CMatrix::operator*(const CMatrix &m) const
{
		// 36 muls, 27 adds
		// | f11 f21 f31 f41 |   | m.f11 m.f21 m.f31 m.f41 |   | f11*m.f11+f21*m.f12+f31*m.f13 f11*m.f21+f21*m.f22+f31*m.f23 f11*m.f31+f21*m.f32+f31*m.f33 f11*m.f41+f21*m.f42+f31*m.f43+f41 |
		// | f12 f22 f32 f42 |   | m.f12 m.f22 m.f32 m.f42 |   | f12*m.f11+f22*m.f12+f32*m.f13 f12*m.f21+f22*m.f22+f32*m.f23 f12*m.f31+f22*m.f32+f32*m.f33 f12*m.f41+f22*m.f42+f32*m.f43+f42 |
		// | f13 f23 f33 f43 | * | m.f13 m.f23 m.f33 m.f43 | = | f13*m.f11+f23*m.f12+f33*m.f13 f13*m.f21+f23*m.f22+f33*m.f23 f13*m.f31+f23*m.f32+f33*m.f33 f13*m.f41+f23*m.f42+f33*m.f43+f43 |
		// | 0   0   0   1   |   | 0     0     0     1     |   | 0                             0                             0                             1                                 |
		CMatrix mRet;
		mRet.f11 = f11*m.f11+f21*m.f12+f31*m.f13;
		mRet.f21 = f11*m.f21+f21*m.f22+f31*m.f23;
		mRet.f31 = f11*m.f31+f21*m.f32+f31*m.f33;
		mRet.f41 = f11*m.f41+f21*m.f42+f31*m.f43+f41;
		mRet.f12 = f12*m.f11+f22*m.f12+f32*m.f13;
		mRet.f22 = f12*m.f21+f22*m.f22+f32*m.f23;
		mRet.f32 = f12*m.f31+f22*m.f32+f32*m.f33;
		mRet.f42 = f12*m.f41+f22*m.f42+f32*m.f43+f42;
		mRet.f13 = f13*m.f11+f23*m.f12+f33*m.f13;
		mRet.f23 = f13*m.f21+f23*m.f22+f33*m.f23;
		mRet.f33 = f13*m.f31+f23*m.f32+f33*m.f33;
		mRet.f43 = f13*m.f41+f23*m.f42+f33*m.f43+f43;
		mRet.f14 = mRet.f24 = mRet.f34 = 0;
		mRet.f44 = 1;
		return mRet;
}

void CQuaternion::operator=(const CMatrix &m)
{
	// Check the sum of the diagonal
	float tr = m(0, 0) + m(1, 1) + m(2, 2);
	if(tr > 0.0f)
	{
		// The sum is positive
		// 4 muls, 1 div, 6 adds, 1 trig function call
		float s = CMath::fastsqrt(tr + 1.0f);
		w = s * 0.5f;
		s = 0.5f / s;
		x = (m(1, 2) - m(2, 1)) * s;
		y = (m(2, 0) - m(0, 2)) * s;
		z = (m(0, 1) - m(1, 0)) * s;
	}
	else
	{
		// The sum is negative
		// 4 muls, 1 div, 8 adds, 1 trig function call
		const int nIndex[3] = {1, 2, 0};
		int i, j, k;
		i = 0;
		if(m(1, 1) > m(i, i))
			i = 1;
		if(m(2, 2) > m(i, i))
			i = 2;
		j = nIndex[i];
		k = nIndex[j];

		float s = CMath::fastsqrt((m(i, i) - (m(j, j) + m(k, k))) + 1.0f);
		(*this)[i] = s * 0.5f;
		if(s != 0.0)
			s = 0.5f / s;
		(*this)[j] = (m(i, j) + m(j, i)) * s;
		(*this)[k] = (m(i, k) + m(k, i)) * s;
		(*this)[3] = (m(j, k) - m(k, j)) * s;
	}
}

CQuaternion CQuaternion::operator*(const CQuaternion &q) const
{
	// 12 muls, 30 adds
	float E = (x + z)*(q.x + q.y);
	float F = (z - x)*(q.x - q.y);
	float G = (w + y)*(q.w - q.z);
	float H = (w - y)*(q.w + q.z);
	float A = F - E;
	float B = F + E;
	return CQuaternion(
		(w + x)*(q.w + q.x) + (A - G - H) * 0.5f,
		(w - x)*(q.y + q.z) + (B + G - H) * 0.5f,
		(y + z)*(q.w - q.x) + (B - G + H) * 0.5f,
		(z - y)*(q.y - q.z) + (A + G + H) * 0.5f);
}

// Spherical linear interpolation between two quaternions
CQuaternion Slerp(const CQuaternion &q1, const CQuaternion &q2, const float t)
{
	// Calculate the cosine of the angle between the two
	float fScale0, fScale1;
	float fCos = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	// If the angle is significant, use the spherical interpolation
	if((1.0 - ABS(fCos)) > DELTA)
	{
		float fTemp = CMath::acos(ABS(fCos));
		float fSin = CMath::sin(fTemp);
		fScale0 = (float)(CMath::sin((1.0f - t) * fTemp) / fSin);
		fScale1 = (float)(CMath::sin(t * fTemp) / fSin);
	}
	// Else use the cheaper linear interpolation
	else
	{
		fScale0 = 1.0f - t;
		fScale1 = t;
	}
	if(fCos < 0.0)
		fScale1 = -fScale1;

	// Return the interpolated result
	return (q1 * fScale0) + (q2 * fScale1);
}
