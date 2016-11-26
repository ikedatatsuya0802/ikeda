#ifndef _SOLARI_H_
#define _SOLARI_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene3D.cpp)
//	ファイル名	scene3D.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	SOLARI_WIDTH		(300.0f)	// ポリゴンの横幅
#define	SOLARI_HEIGHT		(300.0f)	// ポリゴンの縦幅
#define	SOLARI_TEXFILENAME000	"polygon000.jpg"	// ポリゴンのファイル名

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CSolari : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSolari	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSolari(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSolari();

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_Texture;	// テクスチャへのポインタ

	D3DXMATRIX m_mtxWorld;
};

#endif