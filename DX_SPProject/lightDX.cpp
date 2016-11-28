//=============================================================================
//
//	タイトル	カメラファイル
//	ファイル名	light.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/26
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "lightDX.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"

//=============================================================================
//	関数名	:CLightDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CLightDX::CLightDX()
{
	memset(m_aLight, NULL, sizeof(m_aLight));
}

//=============================================================================
//	関数名	:~CLightDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CLightDX::~CLightDX()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CLightDX::Init(void)
{
	
	D3DXVECTOR3 vecDir;

	// 1番目ライトの設定
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.2f);							// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[0].Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(0, &m_aLight[0]);									// ライトの反映
	D3D_DEVICE->LightEnable(0, TRUE);										// ライトの有効化
	
	// 2番目ライトの設定
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_aLight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(-0.75f, -0.55f, -0.25f);						// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[1].Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(1, &m_aLight[1]);									// ライトの反映
	D3D_DEVICE->LightEnable(1, TRUE);										// ライトの有効化
	
	// 3番目ライトの設定
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_aLight[2].Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);							// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[2].Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(2, &m_aLight[2]);									// ライトの反映
	D3D_DEVICE->LightEnable(2, TRUE);										// ライトの有効化

	return S_OK;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CLightDX::Uninit(void)
{

}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CLightDX::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CLightDX::Draw(void)
{	
	// デバッグ情報表示
#ifdef _DEBUG
	//CDebugProc::DebugProc("ライトベクトル:(%5.2f:%5.2f:%5.2f)\n", m_aLight[0].Direction.x, m_aLight[0].Direction.y, m_aLight[0].Direction.z);
#endif
}