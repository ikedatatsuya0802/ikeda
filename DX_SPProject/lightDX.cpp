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

//D3DLIGHT9 CLightDX::m_Light[3];
vector<D3DLIGHT9> CLightDX::m_Light(3);
int	CLightDX::m_LightNum;

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CLightDX::Init(void)
{
	m_LightNum = 0;

	AddHolizontalLight(D3DXVECTOR3(0.7f, -0.5f, 0.2f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(-0.75f, -0.55f, -0.25f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	
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
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	// 配列追加
	m_Light.push_back(light);

	// 平行光源の設定
	m_Light[m_LightNum].Type = D3DLIGHT_DIRECTIONAL;
	
	// ディフューズカラーの設定
	m_Light[m_LightNum].Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	// 方向ベクトルの正規化
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[m_LightNum].Direction, &vec);

	// ライトのセット
	D3D_DEVICE->SetLight(m_LightNum, &m_Light[m_LightNum]);

	// ライトの有効化
	D3D_DEVICE->LightEnable(m_LightNum, TRUE);

	// ライトの数を増やす
	m_LightNum++;
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