#ifndef _MAP_H_
#define _MAP_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	map.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	MAP_WIDTH			(SCREEN_WIDTH * 0.3f * WINDOW_ASPECT_X)	// マップの横幅
#define	MAP_HEIGHT			(MAP_WIDTH)								// マップの縦幅
#define	MAP_POINT_WIDTH		(MAP_WIDTH * 0.05f)	// マップの横幅
#define	MAP_POINT_HEIGHT	(MAP_WIDTH * 0.05f)	// マップの
#define	MAP_POINT_ASPECT	(0.01f)		// マップの縦幅

#define	MAP_TEXFILENAME000	"map000.png"	// ポリゴンのファイル名

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CMap : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MAP_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CMap	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CMap(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CMap();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff[5];	// 頂点バッファ
	LPDIRECT3DTEXTURE9	m_pMapTexture;	// マップテクスチャ
	LPDIRECT3DSURFACE9	m_pMapSurf;		// マップテクスチャのサーフェス
	LPDIRECT3DTEXTURE9	m_pMapTexture2;	// マップテクスチャ
	LPDIRECT3DSURFACE9	m_pMapSurf2;		// マップテクスチャのサーフェス
	LPDIRECT3DSURFACE9	m_BackBufferSurf;

	int			m_MapLine;				// レールの配置
	SPLINE*		m_Spline;
	D3DXVECTOR3 m_SplinePos;
};

#endif