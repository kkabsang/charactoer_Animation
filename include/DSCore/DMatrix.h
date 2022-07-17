#pragma once
#include"DVector.h"

struct float4x4
{
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
class DMatrix :public float4x4
{
public:
	DMatrix()
	{
		Identity();
	}
	void Identity()
	{
		_12 = _13 = _14 = 0;
		_21 = _23 = _24 = 0;
		_31 = _32 = _34 = 0;
		_41 = _42 = _43 = 0;
		_11 = _22 = _33 = _44 = 1;
	}
	static DMatrix RotationZ(float fRadian)
	{
		DMatrix matRet;
		float fCos = cosf(fRadian);
		float fSin = sinf(fRadian);
		matRet._11 = fCos; matRet._12 = fSin;
		matRet._21 = -fSin; matRet._22 = fCos;
		return matRet;
	}
	DMatrix Transpose()
	{
		DMatrix matrix;
		matrix._11 = _11; matrix._12 = _21; matrix._13 = _31; matrix._14 = _41;
		matrix._21 = _12; matrix._22 = _22; matrix._23 = _32; matrix._24 = _42;
		matrix._31 = _13; matrix._32 = _23; matrix._33 = _33; matrix._34 = _43;
		matrix._41 = _14; matrix._42 = _24; matrix._43 = _34; matrix._44 = _44;
		return matrix;
	}
	static DMatrix ViewLookAt(DVector3& vPos, DVector3& vTarget, DVector3& vUp)//뷰 행렬생성
	{
		DMatrix matRet;
		DVector3 vZ = (vTarget - vPos).Normal();//목표(Target)에 카메라의 위치(vPos)빼고 정규화,시선벡터z축
		DVector3 vX = (vUp ^ vZ).Normal();//vUp 임시벡터 vZ를 외적하여 vX축을 만들기 위해 그러고 정규화
		DVector3 vY = (vZ ^ vX).Normal();//vZ와 vX를 외적하여 vY 축생성
		matRet._11 = vX.x; matRet._21 = vX.y; matRet._31 = vX.z;
		matRet._12 = vY.x; matRet._22 = vY.y; matRet._32 = vY.z;
		matRet._13 = vZ.x; matRet._23 = vZ.y; matRet._33 = vZ.z;
		matRet._41 = -(vPos | vX);
		matRet._42 = -(vPos | vY);
		matRet._43 = -(vPos | vZ);
		return matRet;//이거 모르겟다
	}

	static DMatrix PerspectiveFovLH(float fNearPlane, float fFarPlane, float fovy, float Aspect)//원근
		//fNearPlane-근단면,FarPlane - 원단면,fFovy - 시야각,fAspect - 비율, w/h=1.33
	{
		float h, w, Q;
		h = 1 / tan(fovy * 0.5f);
		w = h / Aspect;
		Q = fFarPlane / (fFarPlane - fNearPlane);
		DMatrix ret;
		ZeroMemory(&ret, sizeof(DMatrix));
		ret._11 = w;
		ret._22 = h;
		ret._33 = Q;
		ret._43 = -Q * fNearPlane;
		ret._34 = 1;
		return ret;
	}
};



