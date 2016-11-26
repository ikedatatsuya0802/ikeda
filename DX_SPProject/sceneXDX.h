#ifndef _SCENEX_H_
#define _SCENEX_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(sceneX.cpp)
//	ファイル名	sceneX.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "sceneDX.h"
#include "scene3DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	MODEL_TEXTURENUM		(4)
#define	MODEL_TEXFILENAME000	"player000.png"	// テクスチャのファイル名
#define	MODEL_TEXFILENAME001	"player001.png"	// テクスチャのファイル名
#define	MODEL_TEXFILENAME002	"player002.jpg"	// テクスチャのファイル名
#define	MODEL_TEXFILENAME003	"player003.jpg"	// テクスチャのファイル名

#define	MODEL_MOVEMENT			(0.3f)			// テクスチャのファイル名
#define	MODEL_ROT_STEP			(8)				// プレイヤーの回転ステップ

#define	MODEL_SPEED_DOWN		(0.05f)			// プレイヤーの回転ステップ
#define	MODEL_JUMP				(20.0f)			// プレイヤーの回転ステップ
#define	MODEL_GRAVITY			(0.5f)			// プレイヤーの回転ステップ

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CSceneXDX : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(int texNum) {	D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"MODEL_TEXFILENAME000, &m_Texture[0]);
									D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"MODEL_TEXFILENAME001, &m_Texture[1]);
									D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"MODEL_TEXFILENAME002, &m_Texture[2]);
									D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"MODEL_TEXFILENAME003, &m_Texture[3]);
									D3DXLoadMeshFromX("./data/MODEL/box.x", D3DXMESH_SYSTEMMEM, CRendererDX::GetDevice(), NULL, &m_pBuffMat, NULL, &m_NumMat, &m_pMesh);}
	// リソースのアンロード
	static void	Unload(int texNum) { for(int i = 0 ; i < MODEL_TEXTURENUM ; i++){if(m_Texture != NULL){m_Texture[texNum]->Release(); m_Texture[texNum] = NULL;}}
									if(m_pMesh != NULL){ m_pMesh->Release(); m_pMesh = NULL; }if(m_pBuffMat != NULL){ m_pBuffMat->Release(); m_pBuffMat = NULL;} }

	static CSceneXDX	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSceneXDX(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneXDX();

	static LPDIRECT3DTEXTURE9	m_Texture[MODEL_TEXTURENUM];	// テクスチャへのポインタ

	D3DXMATRIX			m_mtxWorld;		// ワールドマトリックス
	static LPD3DXMESH	m_pMesh;		// メッシュ情報
	static LPD3DXBUFFER	m_pBuffMat;		// マテリアル情報
	static DWORD		m_NumMat;		// マテリアル数

	int					m_nCntMove;		// 移動カウンタ
	D3DXVECTOR3			m_Move;			// プレイヤーの相対移動量
	D3DXVECTOR3			m_RotMove;		// プレイヤーの相対回転量
	bool				m_bJump;		// ジャンプ状態
};

#endif