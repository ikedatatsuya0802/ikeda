#ifndef _SKYDOME_H_
#define _SKYDOME_H_
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
#define	SKYDOME_TEXFILENAME000	"sky001.jpg"						// テクスチャのファイル名
#define	SKYDOME_VERTICAL		(8)						// ドームの縦ブロック数
#define	SKYDOME_HORIZONTAL		(32)					// ドームの横ブロック数
#define	SKYDOME_RADIUS			(5000.0f)				// ドームの半径
const float SKYDOME_HEIGHT		= SKYDOME_RADIUS / 20.0f;	// ドームの縦幅
const float SKYDOME_HEIGHTMAX	= SKYDOME_HEIGHT * (SKYDOME_VERTICAL + 1);	// ドームの天井の高さ
#define	SKYDOME_MOVEROT			(0.00001f)				// ドームの回転量

const int	SKYDOME_DEGENERATE_POLYGON_NUM	= ((SKYDOME_VERTICAL - 1) * 4);	// 縮退ポリゴン数
const int	SKYDOME_POLYGON_NUM
	= ((SKYDOME_HORIZONTAL * 2) * SKYDOME_VERTICAL + SKYDOME_DEGENERATE_POLYGON_NUM);		// ポリゴン数
const int	SKYDOME_INDEX_NUM
	= (((SKYDOME_HORIZONTAL + 1) * 2) * SKYDOME_VERTICAL + ((SKYDOME_VERTICAL - 1) * 2));	// 頂点インデックス数

const int	SKYDOME_VERTEX_NUM			= ((SKYDOME_VERTICAL + 1) * (SKYDOME_HORIZONTAL + 1));	// 頂点数
const int	SKYDOME_INDEX_STANDARD		= ((SKYDOME_HORIZONTAL + 1) * 2 + 2);					// インデックスの基準値

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CSkydome : public CScene3DDX
{
public:
	CSkydome(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkydome();

	void	Init(bool ifLight = false, cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSkydome* Create(bool ifLight = false, cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(SKYDOME_TEXFILENAME000), &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

protected:
	void	SetVtxBuff(bool ifLight);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// インデックスバッファへのポインタ
};

#endif