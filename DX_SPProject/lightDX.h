#ifndef _LIGHTDX_H_
#define _LIGHTDX_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene.cpp)
//	ファイル名	scene.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/19
//
//=============================================================================
#include "rendererDX.h"
#include "main.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	LIGHT_NUM		(3)		// ライトの個数

//=============================================================================
//	クラス定義
//=============================================================================
class CLightDX
{
public:
	static HRESULT	Init(void);
	static void		Uninit(void);
	static void		Update(void);
	static void		Draw(void);

	static uint		GetLightNum(void) { return m_LightNum; }

	static void		ChangeHolLight(cuint num, D3DCOLORVALUE col);
	static void		ChangeLight(cuint num, cbool flug);
	static void		SetSpotLight(cuint num, const D3DXVECTOR3 pos, const D3DXVECTOR3 vec);

private:
	static void AddHolizontalLight(D3DXVECTOR3 vec, D3DCOLORVALUE dif,
		D3DCOLORVALUE spec = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DCOLORVALUE amb = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	static void AddPointLight(D3DXVECTOR3 pos, D3DCOLORVALUE dif,
		D3DCOLORVALUE spec = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DCOLORVALUE amb = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	static void AddSpotLight(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DCOLORVALUE dif,
		D3DCOLORVALUE spec = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DCOLORVALUE amb = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//static D3DLIGHT9 m_Light[3];
	static vector<D3DLIGHT9> m_Light;
	static vector<D3DLIGHT9> m_SpotLight;
	static uint m_LightNum;
};

#endif