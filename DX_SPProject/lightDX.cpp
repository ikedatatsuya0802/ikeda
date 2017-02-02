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
vector<D3DLIGHT9>	CLightDX::m_Light;
uint				CLightDX::m_LightNum;

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
	AddHolizontalLight(D3DXVECTOR3(-0.75f, -0.55f, -0.25f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));

	AddSpotLight(D3DXVECTOR3(0.0f, 100.0f, 400.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
	//ChangeLight(m_LightNum - 1, false);
	
	return S_OK;
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

	// 平行光源の設定
	light.Type = D3DLIGHT_DIRECTIONAL;
	
	// ディフューズカラーの設定
	light.Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	// 方向ベクトルの正規化
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vec);

	// ライトのセット
	D3D_DEVICE->SetLight(m_LightNum, &light);

	// ライトの有効化
	D3D_DEVICE->LightEnable(m_LightNum, TRUE);

	// 配列追加
	m_Light.push_back(light);

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
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	// 平行光源の設定
	//light.Type = D3DLIGHT_SPOT;
	light.Type = D3DLIGHT_POINT;

	// ディフューズカラーの設定
	light.Diffuse	= D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);
	light.Specular	= D3DXCOLOR(spec.r, spec.g, spec.b, spec.a);
	light.Ambient	= D3DXCOLOR(amb.r, amb.g, amb.b, amb.a);

	// 位置の設定
	light.Position = pos;

	// 方向ベクトルの設定
	light.Direction = vec;
	
	light.Range			= 1000.0f;
	light.Falloff		= 1.0f;
	light.Attenuation0	= 0.5f;
	light.Attenuation1	= 0.2f;
	light.Attenuation2	= 0.1f;
	light.Theta			= D3DXToRadian(30.0f);
	light.Phi			= D3DXToRadian(60.0f);

	// ライトのセット
	D3D_DEVICE->SetLight(2, &light);

	// ライトの有効化
	D3D_DEVICE->LightEnable(2, TRUE);

	// 配列追加
	m_Light.push_back(light);

	m_LightNum++;
}

void CLightDX::ChangeLight(cuint num, cbool flug)
{
	if((num >= m_LightNum) || (m_LightNum == 0))
	{// 閾値チェック

		return;
	}

	D3D_DEVICE->LightEnable(num, flug);
}

void CLightDX::SetSpotLight(cuint num, const D3DXVECTOR3 pos, const D3DXVECTOR3 vec)
{
	if((num >= m_Light.size()) || (m_Light.size() == 0))
	{// 閾値チェック

		return;
	}

	// 位置の設定
	m_Light[num].Position = pos;

	// 方向ベクトルの設定
	m_Light[num].Direction = vec;
	
	// ライトのセット
	D3D_DEVICE->SetLight(num, &m_Light[num]);

	// ライトの有効化
	D3D_DEVICE->LightEnable(num, TRUE);
}

//=============================================================================
//	関数名	:ChangeHolLight
//	引数	:無し
//	戻り値	:無し
//	説明	:平行光源の色を変える。
//=============================================================================
void CLightDX::ChangeHolLight(cuint num, D3DCOLORVALUE col)
{
	if(num < m_Light.size())
	{
		// 色変更
		m_Light[num].Diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);

		// ライトのセット
		D3D_DEVICE->SetLight(num, &m_Light[num]);
	}
}