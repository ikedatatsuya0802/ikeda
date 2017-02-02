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
#define	POLYGON3DDX_WIDTH		(300.0f)	// ポリゴンの横幅
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
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CScene3DDX	*Create(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE,
		D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);

	D3DXMATRIX	*GetWMatrix(void) { return &m_mtxWorld; }	// ワールドマトリックスを取得
	
	// リソースのロード
	void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGON3DDX_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	void	Unload(void) { SafetyRelease(m_pTexture); }

protected:
	CScene3DDX(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CScene3DDX();

	virtual void SetVtxBuff(void);
	void SetMeshIndex(LPDIRECT3DINDEXBUFFER9 *idxBuff, const int horizontal, const int vertical);
	
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
};

#endif