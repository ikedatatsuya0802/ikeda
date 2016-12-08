#ifndef _ORBIT_H_
#define _ORBIT_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene3D.cpp)
//	ファイル名	scene3D.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "sceneDX.h"
#include "rendererDX.h"
#include "manager.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	ORBIT_TEXFILENAME000	"field000.jpg"	// テクスチャ名

#define	ORBIT_VERTICAL		(1)				// メッシュの縦ブロック数
#define	ORBIT_HORIZONTAL	(5)				// メッシュの横ブロック数
#define	ORBIT_WIDTH			(20.0f)				// ポリゴンの横幅
#define	ORBIT_HEIGHT		(10.0f)			// ポリゴンの縦幅

#define	ORBIT_TOTALWIDTH	(ORBIT_HORIZONTAL * ORBIT_WIDTH)	// メッシュの合計の横幅
#define	ORBIT_TOTALHEIGHT	(ORBIT_VERTICAL * ORBIT_HEIGHT)		// メッシュの合計の縦幅

#define ORBIT_DEGENERATE_POLYGON_NUM	((ORBIT_VERTICAL - 1) * 4)																// 縮退ポリゴン数
#define	ORBIT_POLYGON_NUM				((ORBIT_HORIZONTAL * 2) * ORBIT_VERTICAL + ORBIT_DEGENERATE_POLYGON_NUM)		// ポリゴン数
#define	ORBIT_INDEX_NUM					(((ORBIT_HORIZONTAL + 1) * 2) * ORBIT_VERTICAL + ((ORBIT_VERTICAL - 1) * 2))	// 頂点インデックス数

#define	ORBIT_VERTEX_NUM				((ORBIT_VERTICAL + 1) * (ORBIT_HORIZONTAL + 1))	// 頂点数
#define ORBIT_INDEX_STANDARD			((ORBIT_HORIZONTAL + 1) * 2 + 2)					// インデックスの基準頂点

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class COrbit : public CSceneDX
{
public:
	COrbit(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~COrbit();

	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ORBIT_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static COrbit		*Create(D3DXVECTOR3 pos = VEC3_ZERO);
	
	void	SetOrbitData(void);
	void	SetLocPos(D3DXVECTOR3 locPos1, D3DXVECTOR3 locPos2) { m_LocPos[0] = locPos1; m_LocPos[1] = locPos2; }


protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// インデックスバッファへのポインタ

	D3DXMATRIX		m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3		m_Nor[ORBIT_VERTEX_NUM];	// 法線情報

	D3DXVECTOR3		m_OldPos[ORBIT_VERTEX_NUM];
	D3DXVECTOR3		m_LocPos[2];

};

#endif