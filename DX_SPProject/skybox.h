#ifndef _SKYBOX_H_
#define _SKYBOX_H_
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

//=============================================================================
//	マクロ定義
//=============================================================================
#define	SKYBOX_TEXFILENAME000	"skybox003.jpg"						// テクスチャのファイル名
#define	SKYBOX_SIZE				(60000.0f)							// ドームの縦ブロック数
#define	SKYBOX_PRIMITIVE_NUM	(6)									// プリミティブ数
#define	SKYBOX_VERTEX_NUM		(VERTEX_NUM * SKYBOX_PRIMITIVE_NUM)	// 頂点数

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CSkybox : public CScene3DDX
{
public:
	void	Init(bool ifLight, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSkybox	*Create(bool ifListAdd = true, int priority = 2, OBJTYPE objtype = OBJTYPE_NONE,
		bool ifLight = false, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(SKYBOX_TEXFILENAME000), &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

protected:
	CSkybox(bool ifListAdd = true, int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkybox();

	void	SetVtxBuff(bool ifLight);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
};

#endif