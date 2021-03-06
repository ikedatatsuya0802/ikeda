#ifndef _SPEEDMETER_H_
#define _SPEEDMETER_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(speedmeter.cpp)
//	ファイル名	speedmeter.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene2DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
cfloat	SPEEDMETER_WIDTH	= SCREEN_HEIGHT * 0.3f;
cfloat	SPEEDMETER_HEIGHT	= SCREEN_HEIGHT * 0.3f;

#define	SPEEDMETER_ROT_LOW	(-2.0f)						// 針の角度の下限
#define	SPEEDMETER_ROT_HIGH	(2.0f)						// 針の角度の上限

#define	SPEEDMETER_TEXFILENAME000	"speedmeter000.png"
#define	SPEEDMETER_TEXFILENAME001	"speedmeter001.png"

//=============================================================================
//	クラス定義
//=============================================================================
class CSpeedmeter : public CScene2DDX
{
public:
	CSpeedmeter(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CSpeedmeter();

	void	Init(cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO,
		cVec2 size = D3DXVECTOR2(SPEEDMETER_WIDTH, SPEEDMETER_HEIGHT));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSpeedmeter* Create(cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO,
		cVec2 size = D3DXVECTOR2(SPEEDMETER_WIDTH, SPEEDMETER_HEIGHT));

protected:
	void SetVtxBuff(void);

	LPDIRECT3DTEXTURE9	m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
};

#endif