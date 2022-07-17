#pragma once
#include<math.h>
#include<Windows.h>

#define DBASIS_EPSILON ((FLOAT)   0.001f)	
#define DBASIS_PI      ((FLOAT)   3.141592654f)
#define DegreeToRadiam(degree) ((degree)*(DBASIS_PI/180.0f))
#define RadianToDegree(radian) ((radian)*(180.0f/DBASIS_PI))
#define MAKECOLOR_ARGB(a,r,g,b)  (((a)&0xff)<<24|(((r)&0xff)<<16)|(((g)&)0xff)<<8|(((b)&0xff)))
#define IS_IN_RANGE(value,r0,r1)(( ((r0)<=(value))&&((value)<=(r1)))?1:0)

namespace DBASIS_VECTOR {
	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};
	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	struct float4
	{
		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};
	class DVector2 :public float2
	{
	public:
		DVector2();
		DVector2(const DVector2& v0);
		DVector2(float fX, float fY);
	};
	class DVector3 : public float3
	{
	public:
		DVector3();
		DVector3(const DVector3& v0);
		DVector3(float fX, float fY, float fZ);

		//연산자 재정의
		DVector3 operator + (DVector3 const& v0);
		DVector3 operator - (DVector3 const& v0);
		DVector3 operator * (float const& fScala);
		DVector3 operator + (float const& fScala);
		DVector3 operator / (float const& fScala);

		//내적
		float operator |(DVector3 const& v0);
		//외적
		DVector3 operator ^(DVector3 const& v0);
		bool operator==(DVector3 const& v0);
		//벡터 길이의 제곱
		float LengthSquared();
		//원점으로의 거리 
		float Length();
		DVector3 Normal();
		float Angle(DVector3& v0);
	};
	class DVector4 : public float4
	{
	public:
		DVector4();
		DVector4(const DVector4& v0);
		DVector4(float fX, float fY, float fZ, float fW);
	};
	class XVector2 : public float2
	{
	public:
		XVector2();
		XVector2(const XVector2& v0);
		XVector2(float fX, float fY);
	};
}
using namespace DBASIS_VECTOR;


