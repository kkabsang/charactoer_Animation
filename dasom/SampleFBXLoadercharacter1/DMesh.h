#pragma once
#include "TModel.h"
#include <fbxsdk.h>


const enum OBJECTCLASSTYPE
{
	CLASS_GEOM = 0, CLASS_BONE, CLASS_DUMMY, CLASS_BIPED
};

struct DMtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	TTexture				m_Texture;
	std::vector<DMtrl*>		m_pSubMtrl;
	DMtrl() {}
	DMtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}
	void Release()
	{
		m_Texture.Release();
		for (auto data : m_pSubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
struct DLayer
{
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
};
struct DWeight
{
	std::vector<int>	m_IndexList;
	std::vector<float>  m_WeightList;
};
struct DSkinData
{
	std::vector<FbxNode*>  m_MatrixList;
	std::vector<DWeight>   m_VertexList;
};
struct PNCTIW_VERTEX 
{
	float     index[4];
	float	  weight[4];
};
struct DAnimMatrix
{
	TMatrix matAnimation[255];
};
class DMesh :public TModel
{
public:
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<TMatrix>m_matBindPoseList;
	std::vector<DMesh*>	m_pMeshLinkList;

	DAnimMatrix			m_matAnimMatrix;
	ID3D11Buffer*		m_pAnimCB=nullptr;
	ID3D11Buffer*		m_pIWVertexBuffer=nullptr;
	std::vector<PNCTIW_VERTEX>       m_WeightList;
	FbxNode* m_pFbxNode = nullptr;
	OBJECTCLASSTYPE		m_ClassType;
	std::wstring		m_szName;
	std::wstring		m_szParentName;
	int					m_iNumLayer=0;
	std::vector<DLayer> m_LayerList;
	int					m_iMtrlRef;
	TMatrix				m_matWorld;
	DMesh* m_pParent=nullptr;
	std::vector<TMatrix>m_AnimationTrack;
	std::vector<DMesh*> m_pSubMesh;
	
public:
	bool Release() override;
	virtual HRESULT		CreateConstantBuffer() override;
	virtual HRESULT		CreateVertexBuffer() override;
	virtual HRESULT		CreateVertexLayout() override;
	virtual bool		PreRender(ID3D11DeviceContext*pContext)override;
public:
	DMesh();
	virtual ~DMesh();
};

