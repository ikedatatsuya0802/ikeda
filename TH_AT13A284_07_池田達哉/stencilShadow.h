#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene3D.cpp)
//	ファイル名	scene3D.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"
#include "manager.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	STENCILSHADOW_TEXFILENAME000	"stencilShadow000.jpg"	// テクスチャのファイル名

#define	STENCILSHADOW_RADIUS		(13.0f)		// 半径
#define	STENCILSHADOW_HORIZONTAL	(32)		// 横ブロック数
#define	STENCILSHADOW_HEIGHT		(1000.0f)	// 縦幅

#define	STENCILSHADOW_POLYGON_NUM	(STENCILSHADOW_HORIZONTAL * 2)			// ポリゴン数
#define	STENCILSHADOW_INDEX_NUM		((STENCILSHADOW_HORIZONTAL + 1) * 2)	// 頂点インデックス数

#define	STENCILSHADOW_WALL_VERTEX_NUM	((STENCILSHADOW_HORIZONTAL + 1) * 2)	// 頂点数
#define STENCILSHADOW_COVER_VERTEX_NUM	(STENCILSHADOW_HORIZONTAL + 2)			// 円柱のふた
#define STENCILSHADOW_INDEX_STANDARD	((STENCILSHADOW_HORIZONTAL + 1) * 2 + 2)	// インデックスの基準値

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CStencilShadow : public CScene3DDX
{
public:
	CStencilShadow(bool isListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CStencilShadow();

	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CStencilShadow	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	static LPDIRECT3DTEXTURE9	m_Texture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffWall;			// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffCoverTop;		// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffCoverBottom;	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffWindow;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// インデックスバッファへのポインタ

	void	SetWorldMatrix(void);
	void	SetIndex(void);
	void	SetVertexData(float alpha = 1.0f);
	void	DrawPillar(void);

	D3DXMATRIX		m_mtxWorld;					// ワールドマトリックス
};

#endif