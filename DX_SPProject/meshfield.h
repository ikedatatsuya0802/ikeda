#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
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
#define	MESHFIELD_TEXFILENAME000	"field002.jpg"	// 地面ポリゴンのファイル名

#define	MESHFIELD_VERTICAL		(20)				// メッシュフィールドの縦ブロック数
#define	MESHFIELD_HORIZONTAL	(20)				// メッシュフィールドの横ブロック数
#define	MESHFIELD_WIDTH			(10000.0f)			// 地面の横幅
#define	MESHFIELD_HEIGHT		(10000.0f)			// 地面の縦幅

#define	MESHFIELD_TOTALWIDTH	(MESHFIELD_HORIZONTAL * MESHFIELD_WIDTH)	// メッシュフィールドの合計の横幅
#define	MESHFIELD_TOTALHEIGHT	(MESHFIELD_VERTICAL * MESHFIELD_HEIGHT)		// メッシュフィールドの合計の縦幅

#define MESHFIELD_DEGENERATE_POLYGON_NUM	((MESHFIELD_VERTICAL - 1) * 4)																// 縮退ポリゴン数
#define	MESHFIELD_POLYGON_NUM				((MESHFIELD_HORIZONTAL * 2) * MESHFIELD_VERTICAL + MESHFIELD_DEGENERATE_POLYGON_NUM)		// ポリゴン数
#define	MESHFIELD_INDEX_NUM					(((MESHFIELD_HORIZONTAL + 1) * 2) * MESHFIELD_VERTICAL + ((MESHFIELD_VERTICAL - 1) * 2))	// 頂点インデックス数

#define	MESHFIELD_VERTEX_NUM				((MESHFIELD_VERTICAL + 1) * (MESHFIELD_HORIZONTAL + 1))	// 頂点数
#define MESHFIELD_INDEX_STANDARD			((MESHFIELD_HORIZONTAL + 1) * 2 + 2)					// インデックスの

#define MESHFIELD_WAVEROT	(0.007f)	// ウェーブの周期
#define MESHFIELD_WAVE		(100.0f)	// ウェーブの縦幅
#define MESHFIELD_WAVESPEED	(10.0f)		// ウェーブのスピード

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CMeshfield : public CScene3DDX
{
public:
	CMeshfield(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CMeshfield();

	void	Init(cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CMeshfield* Create(cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MESHFIELD_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	inline int	GetFrontMesh(D3DXVECTOR3 pos);
	float		GetHeight(D3DXVECTOR3 pos);
	
	D3DXVECTOR3			GetMeshfieldPos(void){ return m_Pos; }
	D3DXVECTOR3		* GetMeshfieldNor(void){ return m_Nor; }
	static D3DXVECTOR3	GetVtxPos(int nNumVtx);
	static D3DXVECTOR3	GetVtxNor(int nNumVtx);

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3					m_Nor[MESHFIELD_VERTEX_NUM];	// 法線情報

	void	SetVtxBuff(void);
};

#endif