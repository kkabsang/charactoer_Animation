#pragma once
#include "TStd.h"
class DDxState
{
public:
	static ID3D11DepthStencilState*		g_pCurrentDSS;
	static ID3D11SamplerState*			g_pCurrentSS[3];
	static ID3D11RasterizerState*		g_pCurrentRS;

	static ID3D11DepthStencilState*		g_pLessEqualDSS;
	static ID3D11SamplerState*			g_pClampSS;
	static ID3D11SamplerState*			g_pWrapSS;
	static ID3D11RasterizerState*		g_pRSSolid;
	static ID3D11RasterizerState*		g_pRSWireFrame;

	static HRESULT		CreateDepthStencilState();
	static HRESULT		CreateSamplerState();
	static HRESULT		CreateRasterizerState();

	static bool			Init();
	static bool			Release();
};

static ID3D11DepthStencilState* ApplyDSS(ID3D11DeviceContext* pContext, ID3D11DepthStencilState* pState)
{
	ID3D11DepthStencilState* pDDS = DDxState::g_pCurrentDSS;
	pContext->OMSetDepthStencilState(pState, 0x01);
	DDxState::g_pCurrentDSS = pState;
	return pDDS;
}

static ID3D11SamplerState* ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pState, UINT StartSlot=0)
{
	ID3D11SamplerState* pSS = DDxState::g_pCurrentSS[StartSlot];
	pContext->PSSetSamplers(StartSlot, 1, &pState);
	DDxState::g_pCurrentSS[StartSlot] = pState;
	return pSS;
}

static ID3D11RasterizerState* ApplyRS(ID3D11DeviceContext* pContext, ID3D11RasterizerState* pState)
{
	ID3D11RasterizerState* pRS = DDxState::g_pCurrentRS;
	pContext->RSSetState(pState);
	DDxState::g_pCurrentRS = pState;
	return pRS;
}


