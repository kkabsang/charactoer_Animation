#pragma once
#include "TCore.h"
#include "DFbxObj.h"
#include "TShape.h"
class Sample :public TCore
    
{
    DViewRT     m_Rt;
    TVector3    m_vMovPos;
    std::vector<DFbxObj*>m_pObjectList;
    DFbxObj     m_FbxObjA;
    DFbxObj     m_FbxObjB;
    /*TMatrix     m_matShadow;*/
    ID3D11PixelShader* m_pPSShadow = nullptr;

   
public:
    ID3D11DepthStencilState* m_pDsvState;
    HRESULT		CreateDepthStencilState();
public:
    bool    Init()override;
    bool    Frame()override;
    bool    Render()override;
    bool    Release()override;
public:
    Sample();
    virtual ~Sample();
};

