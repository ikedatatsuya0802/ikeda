#ifndef _SCENE2DDX_H_
#define _SCENE2DDX_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene2DDX.cpp)
//	ファイル名	scene2DDX.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "sceneDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CScene2DDX : public CSceneDX
{
public:
	CScene2DDX(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_2D);
	~CScene2DDX();

	void	Init(cchar *str = ".\\data\\TEXTURE\\field000.jpg",
		cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO,
		cVec2 size = D3DXVECTOR2(100.0f, 100.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CScene2DDX	*Create(cchar *str = ".\\data\\TEXTURE\\field000.jpg",
		cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO,
		cVec2 size = D3DXVECTOR2(100.0f, 100.0f));

	// リソースのロード
	void	Load(cchar* str) { D3DXCreateTextureFromFile(D3D_DEVICE, str, &m_pTexture); }
	// リソースのアンロード
	void	Unload(void) { SafetyRelease(m_pTexture); }

	void	SetSize(cbool mode, cfloat x, cfloat y);
	void	SetColor(cfloat a = 1.0f, cfloat r = 1.0f, cfloat g = 1.0f, cfloat b = 1.0f);

protected:
	virtual void SetVtxBuff(void);

	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ

	float	m_fLength;	// 対角線の長さ
	float	m_fAngle;	// 角度
};

#endif