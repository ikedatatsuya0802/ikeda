#ifndef _PILLAR_H_
#define _PILLAR_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	pillar.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define array_sizeof(p) ( sizeof( p) / sizeof( (p)[0] ) )
#define	PILLAR_RADIUS			(10.0f)					// 半径
#define	PILLAR_HEIGHT			(100.0f)				// 縦幅
#define	PILLAR_VERTICAL			(1)						// 縦ブロック数
#define	PILLAR_HORIZONTAL		(16)					// 横ブロック数
#define	PILLAR_TEXFILENAME000	"wall000.jpg"			// ポリゴンのファイル名

#define	PILLAR_POLYGON_NUM		((PILLAR_HORIZONTAL * 2) * PILLAR_VERTICAL + ((PILLAR_VERTICAL - 1) * 4))		// ポリゴン数
#define	PILLAR_INDEX_NUM		(((PILLAR_HORIZONTAL + 1) * 2) * PILLAR_VERTICAL + ((PILLAR_VERTICAL - 1) * 2))	// 頂点インデックス数

#define	PILLAR_VERTEX_NUM		((PILLAR_VERTICAL + 1) * (PILLAR_HORIZONTAL + 1))	// 頂点数
#define PILLAR_INDEX_STANDARD	((PILLAR_HORIZONTAL + 1) * 2 + 2)					// インデックスの基準値

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CPillar : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PILLAR_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CPillar	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CPillar(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CPillar();

	void SetVtxBuff(void);
	void SetMeshNor(void);
	D3DXVECTOR3 GetVecNor(D3DXVECTOR3 *vec, uint size);

	static LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9			m_pIdxBuff;	// インデックスバッファへのポインタ
	
	vector<LPDIRECT3DVERTEXBUFFER9>	m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 m_MeshPos[PILLAR_VERTEX_NUM];

	SPLINE*		m_Spline;
};

#endif