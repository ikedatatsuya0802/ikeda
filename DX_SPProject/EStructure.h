#ifndef _ESTRUCTURE_H_
#define _ESTRUCTURE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	EStructure.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
const float	ESTRUCTURE_WIDTH		= (RAILLINE_WIDTH * 5.0f);	// 横幅
const int	ESTRUCTURE_VERTICAL		= RAILLINE_SET;				// 奥方向のブロック数
const int	ESTRUCTURE_HORIZONTAL	= 10;						// 平行方向のブロック数
const float	ESTRUCTURE_WALL_HEIGHT	= 10.0f;					// 外壁の高さ
#define	ESTRUCTURE_TEXFILENAME000	"EStructure000.png"			// テクスチャファイル名

const int ESTRUCTURE_POLYGON_NUM = ((ESTRUCTURE_HORIZONTAL * 2) * ESTRUCTURE_VERTICAL + ((ESTRUCTURE_VERTICAL - 1) * 2));			// ポリゴン数
const int ESTRUCTURE_INDEX_NUM = (((ESTRUCTURE_HORIZONTAL + 1) * 2) * ESTRUCTURE_VERTICAL + ((ESTRUCTURE_VERTICAL - 1) * 2));	// 頂点インデックス数

const int ESTRUCTURE_VERTEX_NUM = ((ESTRUCTURE_VERTICAL + 1) * (ESTRUCTURE_HORIZONTAL + 1));	// 頂点数
const int ESTRUCTURE_INDEX_STANDARD = ((ESTRUCTURE_HORIZONTAL + 1) * 2 + 2);					// インデックスの基準値

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CEStructure : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ESTRUCTURE_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CEStructure	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CEStructure(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CEStructure();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_MeshPos[ESTRUCTURE_VERTEX_NUM];

	SPLINE*		m_Spline;
};

#endif