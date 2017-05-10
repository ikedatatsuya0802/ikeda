#ifndef _DRIFTMARK_H_
#define _DRIFTMARK_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(driftMark.cpp)
//	ファイル名	driftMark.h
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
#define	DRIFTMARK_WIDTH		(500.0f * 0.5f)	// ポリゴンの横幅
#define	DRIFTMARK_HEIGHT	(300.0f * 0.5f)	// ポリゴンの縦幅

#define	DRIFTMARK_TEXNAME000	"drift001.jpg"
#define	DRIFTMARK_TEXNAME001	"drift002.jpg"
#define	DRIFTMARK_TYPE			(2)
#define	DRIFTMARK_SRIDESPEED	(0.015f)
#define	DRIFTMARK_FUTURE		(0.01f)		// ドリフトマーク表示地点

//=============================================================================
//	構造体
//=============================================================================
typedef struct {
	D3DXVECTOR3	Pos;
	D3DXVECTOR2	Size;
	D3DCOLOR	Col;
} DRIFTMARK;

//=============================================================================
//	クラス定義
//=============================================================================
class CDriftMark : CScene2DDX
{
public:
	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CDriftMark* Create(void);

	static void VisibleDriftMark(bool type, bool curve, int time);
	static void InvisibleDriftMark(int time);

	void	SetColor(float a = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);

protected:
	CDriftMark(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CDriftMark();

	void SetVtxBuff(void);
	
	LPDIRECT3DTEXTURE9		m_pTexture[DRIFTMARK_TYPE];		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファへのポインタ

	DRIFTMARK	m_DriftMark;

	static bool		m_VisibleType;	// 表示タイプ、false->通常, true->点滅
	static bool		m_flgCounter;	// 点滅用フラグ、trueだと減らし、falseだと増やす
	static bool		m_Curve;
	static int		m_Count;
	static int		m_SetCounter;
};

#endif