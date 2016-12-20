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
typedef struct {
	LPD3DXMESH		pMesh;		// メッシュ情報
	LPD3DXBUFFER	pBuffMat;	// マテリアル情報
	DWORD			NumMat;		// マテリアル数
} MODELSTATUS;					// 3Dモデル情報

typedef struct {
	string				FileName;	// メッシュ情報
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
} TEXTURE;	// テクスチャ情報

//=============================================================================
//	クラス定義
//=============================================================================
class CSceneXDX : public CScene3DDX
{
public:
	void	Init(char* fileName, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSceneXDX	*Create(char* fileName, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	
	// リソースのロード
	static void	Load(void);
	// リソースのアンロード
	static void	Unload(void);


protected:
	CSceneXDX(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneXDX();

	void	LoadModel(char *filename);
	void	AutomaticSetTexture(void);
	void	AddTexture(vector<TEXTURE> &texture, char* fileName);

	MODELSTATUS	m_ModelStatus;	// 3Dモデル情報

	vector<TEXTURE>	m_pTexture;	// テクスチャへのポインタ
};

#endif