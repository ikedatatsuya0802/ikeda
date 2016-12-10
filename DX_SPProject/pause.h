#ifndef _PAUSE_H_
#define _PAUSE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(pause.cpp)
//	ファイル名	pause.h
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
#define	PAUSE_TEXFILENAME000			"pause000.png"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME001			"pause001.jpg"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME002			"pause002.png"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME003			"pause003.png"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME004			"pause004.png"						// ポーズポリゴンのファイル名

//=============================================================================
//	クラス定義
//=============================================================================
class CPause : public CScene2DDX
{
public:
	CPause(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CPause();

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CPause	*Create(void);

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff);

	LPDIRECT3DTEXTURE9	m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[2];

	float m_MaxSpeed;	// 最大速度
};

#endif