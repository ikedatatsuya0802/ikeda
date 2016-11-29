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

D3DLIGHT9 CLightDX::m_Light[3];
//list<D3DLIGHT9> CLightDX::m_Light(3);
list<D3DLIGHT9>::iterator CLightDX::m_ItrLight;
int				CLightDX::m_LightNum;

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
	ZeroMemory(&m_Light[0], sizeof(D3DLIGHT9));
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_Light[0].Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.2f);							// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[0].Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(0, &m_Light[0]);									// ライトの反映
	D3D_DEVICE->LightEnable(0, TRUE);										// ライトの有効化

																		// 2番目ライトの設定
	ZeroMemory(&m_Light[1], sizeof(D3DLIGHT9));
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_Light[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(-0.75f, -0.55f, -0.25f);						// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[1].Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(1, &m_Light[1]);									// ライトの反映
	D3D_DEVICE->LightEnable(1, TRUE);										// ライトの有効化

																		// 3番目ライトの設定
	ZeroMemory(&m_Light[2], sizeof(D3DLIGHT9));
	m_Light[2].Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_Light[2].Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);							// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[2].Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(2, &m_Light[2]);									// ライトの反映
	D3D_DEVICE->LightEnable(2, TRUE);										// ライトの有効化

	/*
	m_LightNum = 0;

	//AddHolizontalLight(D3DXVECTOR3(1.0, 1.0, 1.0), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	
	D3DXVECTOR3 vecDir;
	m_ItrLight = m_Light.begin();

	// 1番目ライトの設定
	ZeroMemory(&m_ItrLight, sizeof(D3DLIGHT9));
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	//m_ItrLight->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 光の色
	m_ItrLight->Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.2f);							// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(0, &*m_ItrLight);									// ライトの反映
	D3D_DEVICE->LightEnable(0, TRUE);										// ライトの有効化
	m_ItrLight++;
	
	// 2番目ライトの設定
	ZeroMemory(&m_ItrLight, sizeof(D3DLIGHT9));
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_ItrLight->Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(-0.75f, -0.55f, -0.25f);						// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(1, &*m_ItrLight);									// ライトの反映
	D3D_DEVICE->LightEnable(1, TRUE);										// ライトの有効化
	m_ItrLight++;
	
	// 3番目ライトの設定
	ZeroMemory(&m_ItrLight, sizeof(D3DLIGHT9));
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;							// 平行高原
	m_ItrLight->Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);			// 光の色
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);							// 光の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vecDir);	// 正規化
	D3D_DEVICE->SetLight(2, &*m_ItrLight);									// ライトの反映
	D3D_DEVICE->LightEnable(2, TRUE);										// ライトの有効化
	*/
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
	//CDebugProc::DebugProc("ライトベクトル:(%5.2f:%5.2f:%5.2f)\n", m_ItrLight->Direction.x, m_ItrLight->Direction.y, m_ItrLight->Direction.z);
#endif
}

//=============================================================================
//	関数名	:AddHolizontalLight
//	引数	:無し
//	戻り値	:無し
//	説明	:平行光源を追加する。
//=============================================================================
void CLightDX::AddHolizontalLight(D3DXVECTOR3 vec, D3DCOLORVALUE dif, D3DCOLORVALUE spec, D3DCOLORVALUE amb)
{/*
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	// 配列追加
	m_Light.push_back(light);

	m_ItrLight = m_Light.end();

	// 平行光源の設定
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;
					
	// ディフューズカラーの設定
	m_ItrLight->Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	// 方向ベクトルの正規化
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vec);

	// ライトのセット
	D3D_DEVICE->SetLight(0, &*m_ItrLight);

	// ライトの有効化
	D3D_DEVICE->LightEnable(0, TRUE);*/
}

//=============================================================================
//	関数名	:AddPointLight
//	引数	:無し
//	戻り値	:無し
//	説明	:点光源を追加する。
//=============================================================================
void CLightDX::AddPointLight(D3DXVECTOR3 pos, D3DCOLORVALUE dif, D3DCOLORVALUE spec, D3DCOLORVALUE amb)
{

}

//=============================================================================
//	関数名	:AddSpotLight
//	引数	:無し
//	戻り値	:無し
//	説明	:スポット光源を追加する。
//=============================================================================
void CLightDX::AddSpotLight(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DCOLORVALUE dif, D3DCOLORVALUE spec, D3DCOLORVALUE amb)
{

}