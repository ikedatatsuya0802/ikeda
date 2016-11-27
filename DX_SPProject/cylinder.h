#ifndef _CYLINDER_H_
#define _CYLINDER_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene3D.cpp)
//	ファイル名	scene3D.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"
#include "rendererDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	CYLINDER_TEXFILENAME000	"cylinder000.jpg"	// テクスチャのファイル名

#define	CYLINDER_RADIUS			(29000.0f)						// 半径
#define	CYLINDER_VERTICAL		(1)								// 縦ブロック数
#define	CYLINDER_HORIZONTAL		(32)							// 横ブロック数
#define	CYLINDER_MOVEROT		(0.0f)						// 回転量
#define	CYLINDER_HEIGHT			(CYLINDER_RADIUS * 0.1f)		// 縦幅

#define	CYLINDER_TOTALWIDTH		(CYLINDER_HORIZONTAL * CYLINDER_WIDTH)	// 合計の横幅
#define	CYLINDER_TOTALHEIGHT	(CYLINDER_VERTICAL * CYLINDER_HEIGHT)	// 合計の縦幅

#define CYLINDER_DEGENERATE_POLYGON_NUM	((CYLINDER_VERTICAL - 1) * 4)															// 縮退ポリゴン数
#define	CYLINDER_POLYGON_NUM			((CYLINDER_HORIZONTAL * 2) * CYLINDER_VERTICAL + CYLINDER_DEGENERATE_POLYGON_NUM)		// ポリゴン数
#define	CYLINDER_INDEX_NUM				(((CYLINDER_HORIZONTAL + 1) * 2) * CYLINDER_VERTICAL + ((CYLINDER_VERTICAL - 1) * 2))	// 頂点インデックス数

#define	CYLINDER_VERTEX_NUM				((CYLINDER_VERTICAL + 1) * (CYLINDER_HORIZONTAL + 1))	// 頂点数
#define CYLINDER_INDEX_STANDARD			((CYLINDER_HORIZONTAL + 1) * 2 + 2)						// インデックスの基準値

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CCylinder : public CScene3DDX
{
public:
	CCylinder(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CCylinder();

	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"CYLINDER_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { if(m_pTexture != NULL){m_pTexture->Release(); m_pTexture = NULL;} }
	
	void				SetCylinderData(void);
	static CCylinder	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// インデックスバッファへのポインタ

	D3DXMATRIX		m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3		m_Nor[CYLINDER_VERTEX_NUM];	// 法線情報
};

#endif