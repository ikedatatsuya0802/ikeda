#ifndef _MODEL_H_
#define _MODEL_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(model.cpp)
//	ファイル名	model.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/14
//
//=============================================================================
#include "sceneXDX.h"

//----- 頂点フォーマット定義
typedef struct {
	float		x, y, z;			// 頂点座標（座標変換あり）
	float		nx, ny, nz;			// 法線ベクトル
	float		tu, tv;				// テクスチャ座標
} VERTEX;
#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

typedef struct {
	LPD3DXMESH		pMesh;		// メッシュ情報
	LPD3DXBUFFER	pBuffMat;	// マテリアル情報
	DWORD			NumMat;		// マテリアル数
	VERTEX			*pt_vertex;	// 頂点データへのポインタ
	WORD			*pt_index;	// インデックスデータへのポインタ
	int				num_vertex;	// 頂点数
	int				num_face;	// 面数
	int				MorphTime;
} MODELSTATUS_MORPH;			// 3Dモデル情報

//=============================================================================
//	クラス定義
//=============================================================================
class CModel : public CSceneXDX
{
public:
	CModel(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CModel();

	void	Init(string fileDir, string filename);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CModel	*Create(string fileDir, string filename);

	int		GetMorphCountAll(void) { return m_MorphCountAll; }

protected:
	void	LoadModel(string fileDir, string filename);
	string	to_string(int val);

	LPDIRECT3DTEXTURE9 m_pTexture;
	
	string		m_FileDir;

	VERTEX		*m_MorphVertex;			// モーフィング用頂点ワークへのポインタ
	WORD		*m_MorphIndex;			// モーフィング用インデックスワークへのポインタ

	vector<MODELSTATUS_MORPH>	m_ModelStatus;	// 3Dモデル情報
	int	m_MorphCountAll;	// モーフィングカウントの合計
	int	m_MorphCount;		// モーフィングカウント
	int	m_MorphStatus;		// モーフィング状態
};

#endif