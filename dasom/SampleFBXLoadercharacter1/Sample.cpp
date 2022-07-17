#include "Sample.h"

HRESULT Sample::CreateDepthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	sd.DepthEnable = TRUE;
	sd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_pd3dDevice->CreateDepthStencilState(&sd, &m_pDsvState);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pImmediateContext->OMSetDepthStencilState(m_pDsvState, 0x01);
	return hr;
}

bool		Sample::Init()
{
	CreateDepthStencilState();
	CStopwatch start;
	m_FbxObjB.LoadObject("../../data/object/Man.fbx", "CharacterShader.hlsl");
	start.Output(L"a");
	m_Camera.CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);

		return true;
}
bool		Sample::Frame() 
{
	if (g_Input.GetKey(VK_F2) == KEY_HOLD)
	{
		D3D11_RASTERIZER_DESC RSDesc;
		memset(&RSDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
		RSDesc.FillMode=D3D11_FILL_WIREFRAME;
		RSDesc.CullMode = D3D11_CULL_BACK;
		RSDesc.FrontCounterClockwise = FALSE;
	}
	
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		m_FbxObjB.m_bAnimPlay = !m_FbxObjB.m_bAnimPlay;
	}
	
	m_FbxObjB.Frame();
	
	if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
	{
		m_vMovPos.z += g_fSecPerFrame * 100.0f;
	}
	if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
	{
		m_vMovPos.z -= g_fSecPerFrame * 100.0f;
	}
	if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
	{
		D3DXMatrixRotationY(&m_FbxObjB.m_matWorld, g_fGameTimer);
	}
	
	m_FbxObjB.m_matWorld._41 = m_vMovPos.x;
	m_FbxObjB.m_matWorld._42 = m_vMovPos.y;
	m_FbxObjB.m_matWorld._43 = m_vMovPos.z;
	

	return true;
}
bool		Sample::Render()
{
	
	m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObjB.SetPixelShader(nullptr);
	m_FbxObjB.Render(m_pImmediateContext);
	
	return true;
}
bool		Sample::Release() {
	SAFE_RELEASE(m_pDsvState);
	m_FbxObjB.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)