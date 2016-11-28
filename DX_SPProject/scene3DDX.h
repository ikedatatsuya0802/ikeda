#ifndef _SCENE3D_H_
#define _SCENE3D_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene3D.cpp)
//	ファイル名	scene3D.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "sceneDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	POLYGON3DDX_POSX		(0.0f)	// ポリゴンの横幅
#define	POLYGON3DDX_POSY		(0.0f)	// ポリゴンの縦幅
#define	POLYGON3DDX_POSZ		(0.0f)	// ポリゴンの縦幅
#define	POLYGON3DDX_WIDTH		(30.0f)	// ポリゴンの横幅
#define	POLYGON3DDX_HEIGHT		(300.0f)	// ポリゴンの縦幅
#define	POLYGON3DDX_TEXFILENAME000	"field000.jpg"	// ポリゴンのファイル名

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CScene3DDX : public CSceneDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGON3DDX_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CScene3DDX	*Create(void);
	static CScene3DDX	*Create(D3DXVECTOR3 pos);

protected:
	CScene3DDX(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CScene3DDX();
	
	static LPDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// 頂点バッファへのポインタ
	
	D3DXMATRIX					m_mtxWorld;	// ワールドマトリックス
};

#endif