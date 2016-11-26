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
#define	POLYGON2DDX_POSX	(SCREEN_WIDTH * 0.5f)		// ポリゴンの横幅
#define	POLYGON2DDX_POSY	(SCREEN_HEIGHT * 0.5f)		// ポリゴンの縦幅
#define	POLYGON2DDX_WIDTH	(100.0f)					// ポリゴンの横幅
#define	POLYGON2DDX_HEIGHT	(100.0f)					// ポリゴンの縦幅
#define	POLYGON2DDX_TURN	(-0.05f)
#define	POLYGON2DDX_MOVE	(-0.03f)

#define	POLYGON2DDX_TEXFILENAME000	"polygon000.jpg"	// ポリゴンのファイル名

//=============================================================================
//	クラス定義
//=============================================================================
class CScene2DDX : public CSceneDX
{
public:
	CScene2DDX(int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CScene2DDX();

	void	Init(D3DXVECTOR3 pos){}
	void	Init(D3DXVECTOR2 pos, D3DXVECTOR2 size, char *str);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CScene2DDX	*Create(D3DXVECTOR2 pos = D3DXVECTOR2(0.0f, 0.0f),
								D3DXVECTOR2 size = D3DXVECTOR2(POLYGON2DDX_WIDTH, POLYGON2DDX_HEIGHT),
								char *str = ".\\data\\TEXTURE\\"POLYGON2DDX_TEXFILENAME000);
	void	SetColor(float a = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);

protected:
	LPDIRECT3DTEXTURE9		m_Texture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファへのポインタ

	float	m_fLength;	// 対角線の長さ
	float	m_fAngle;	// 角度
};

#endif