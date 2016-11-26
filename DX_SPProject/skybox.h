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
#include "sceneDX.h"
#include "rendererDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	SKYBOX_TEXFILENAME000	"skybox000.jpg"						// テクスチャのファイル名
#define	SKYBOX_SIZE				(60000.0f)							// ドームの縦ブロック数
#define	SKYBOX_PRIMITIVE_NUM	(6)									// プリミティブ数
#define	SKYBOX_VERTEX_NUM		(VERTEX_NUM * SKYBOX_PRIMITIVE_NUM)	// 頂点数

//=============================================================================
//	前方宣言
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CSkybox : public CSceneDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"SKYBOX_TEXFILENAME000, &m_Texture); }
	// リソースのアンロード
	static void	Unload(void) { if(m_Texture != NULL){m_Texture->Release(); m_Texture = NULL;} }

	static CSkybox	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSkybox(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkybox();

	void	SetSkyboxData(VERTEX_3D *pVtx);
	void	DrawSkyboxData(void);
	
	static LPDIRECT3DTEXTURE9	m_Texture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// 頂点バッファへのポインタ
	
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
};

#endif