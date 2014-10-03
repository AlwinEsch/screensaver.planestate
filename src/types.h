////////////////////////////////////////////////////////////////////////////
// This contains the types and math classes that I'm used to work with.
// They have been stipped of most unneeded stuff that isn't needed in 
// the screensaver
//
// Author:
//   Joakim Eriksson
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

/***************************** D E F I N E S *******************************/

typedef signed char			s8;
typedef unsigned char		u8;
typedef signed short		s16;
typedef unsigned short		u16;
typedef signed long			s32;
typedef unsigned long		u32;
typedef int64_t		s64;
typedef uint64_t	u64;
typedef float				f32;
typedef double				f64;
#define null				0

const f32 PI				= 3.14159265358979323f;
const f32 FLOATEPSILON		= 0.00001f;

/****************************** M A C R O S ********************************/

#define SAFE_DELETE(_p)			{ if(_p) { delete _p;		_p=NULL; } }
#define SAFE_DELETE_ARRAY(_p)	{ if(_p) { delete [] _p;	_p=NULL; } }
#define SAFE_RELEASE(_p)		{ if(_p) { _p->Release();	_p=NULL; } }

#define DEGTORAD(d)			((d)*(PI / 180.0f))
#define RADTODEG(r)			((r)*(180.0f / PI))
#define assert(_x)

/***************************** C L A S S E S *******************************/

////////////////////////////////////////////////////////////////////////////
//
class CRGBA
{
public:
	union
	{
		f32			col[4];	
		struct
		{
			f32		r,g,b,a;
		};
	};

	 			CRGBA()								{ col[0] = col[1] = col[2] = 0.0f;	col[3] = 1.0f;	}
	 			CRGBA(f32 R, f32 G, f32 B, f32 A)	{ col[0]=R; col[1]=G; col[2]=B; col[3]=A;			}
	u32			RenderColor(void) const;
	void		Set(f32 R, f32 G, f32 B, f32 A)		{ col[0]=R; col[1]=G; col[2]=B; col[3]=A;			}
	CRGBA&		operator = (const CRGBA& c)			{ r = c.r; g = c.g; b = c.b; a = c.a; return *this; }
};

////////////////////////////////////////////////////////////////////////////
//
class CVector
{
public:
				CVector()						{ }
				CVector(f32 _x, f32 _y, f32 _z)	{ x = _x; y = _y; z = _z; }
	void		Zero()							{ x = y = z = 0.0f;	}
	void		Set(f32 _x, f32 _y, f32 _z)		{ x = _x; y = _y; z = _z; }
        f32 x, y, z;
};

////////////////////////////////////////////////////////////////////////////
//
class CMatrix
{
public:
	void		Identity(void);
	void		Rotate(f32 angleX, f32 angleY, f32 angleZ);
	void		Scale(f32 sx, f32 sy, f32 sz)					{ _11 *= sx; _22 *= sy; _33 *= sz; }
	void		Multiply(const CMatrix& m1, const CMatrix& m2);
	CVector operator * ( const CVector& v ) const;

        f32 _11, _12, _13, _14;
        f32 _21, _22, _23, _24;
        f32 _31, _32, _33, _34;
        f32 _41, _42, _43, _44;
};

/***************************** G L O B A L S *******************************/
/***************************** I N L I N E S *******************************/

inline f32	Clamp(f32 x, f32 min, f32 max)		{ return (x <= min ? min : (x >= max ? max : x)); }
inline f32	RandFloat(void)						{ return (1.0f / RAND_MAX) * ((f32)rand());	}
inline int	ISEQUAL(f32 a,f32 b,f32 absprec)	{ return (fabsf((a)-(b))<=absprec);	}
inline int	Rand(int max)						{ return rand() % max; }

////////////////////////////////////////////////////////////////////////////
// Returns a u32 color data, which is used in rendering.
//
inline u32	CRGBA::RenderColor(void) const
{
	return ((((u32)FloatToByte(a) << 24) | ( (u32)FloatToByte(r) << 16) | ( (u32)FloatToByte(g) << 8) |  (u32)FloatToByte(b)));
}

////////////////////////////////////////////////////////////////////////////
//
inline void CMatrix::Identity(void)
{	
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

////////////////////////////////////////////////////////////////////////////
// Create a rotation matrix (In the order we need. Not the normal DX order)
//
inline void	CMatrix::Rotate(f32 angleX, f32 angleY, f32 angleZ)
{
	f32		A,B,C,D,E,F, BD, BC;
	A	= cosf(DEGTORAD(angleX));	B = sinf(DEGTORAD(angleX));
	C	= cosf(DEGTORAD(angleY));	D = sinf(DEGTORAD(angleY));
	E	= cosf(DEGTORAD(angleZ));	F = sinf(DEGTORAD(angleZ));
	BD	= B*D;
	BC	= B*C;

	_11 = (E*C) + (F*BD);	_12 = F*A;		_13 = (E*-D)+(F*BC);	_14 = 0.0f;
	_21 = (-F*C) + (E*BD);	_22 = E*A;		_23 = (F*D)+(E*BC);		_24 = 0.0f;
	_31 = A*D;				_32 = -B;		_33 = A*C;				_34 = 0.0f;
	_41 = 0.0f;				_42 = 0.0f;		_43 = 0.0f;				_44 = 1.0f;
}

////////////////////////////////////////////////////////////////////////////
//
inline CVector CMatrix::operator * ( const CVector& v ) const
{
	return CVector(	v.x * _11 + v.y * _21 + v.z * _31 + _41,
					v.x * _12 + v.y * _22 + v.z * _32 + _42,
					v.x * _13 + v.y * _23 + v.z * _33 + _43);
}

////////////////////////////////////////////////////////////////////////////
//
inline void CMatrix::Multiply(const CMatrix& m1, const CMatrix& m2)
{
	CMatrix m;
	D3DXMatrixMultiply(&m, &m2, &m1);
	*this = m;
}

////////////////////////////////////////////////////////////////////////////
//
inline	f32		InterpolateFloat(f32 v1, f32 v2, f32 t, bool linear)
{
	assert((t >= 0.0f) && (t <= 1.0f));
	if (linear)
	{
		return v1 + t*(v2 - v1);
	}

	// Compute Hermite spline coefficients for t, where 0 <= t <= 1.
	f32	t2	= t * t;
	f32	t3	= t * t2;
	f32	z	= 3.0f * t2 - t3 - t3;
	return v1*(1.0f - z) + v2*z;
}

