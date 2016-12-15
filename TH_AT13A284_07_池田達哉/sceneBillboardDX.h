#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(sceneBillboard.cpp)
//	ファイル名	sceneBillboard.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	POLYGONBILLBOARDDX_POSX		(0.0f)	// ポリゴンの横幅
#define	POLYGONBILLBOARDDX_POSY		(100.0f)	// ポリゴンの縦幅
#define	POLYGONBILLBOARDDX_POSZ		(0.0f)	// ポリゴンの縦幅
#define	POLYGONBILLBOARDDX_WIDTH		(20.0f)	// ポリゴンの横幅
#define	POLYGONBILLBOARDDX_HEIGHT		(20.0f)	// ポリゴンの縦幅
#define	POLYGONBILLBOARDDX_TEXFILENAME000	"effect000.jpg"	// ポリゴンのファイル名

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CSceneBillboardDX : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGONBILLBOARDDX_TEXFILENAME000, &m_pTexture); }
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CSceneBillboardDX	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSceneBillboardDX(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneBillboardDX();
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	
	D3DXMATRIX				m_mtxWorld;	// ワールドマトリックス
};

#endif