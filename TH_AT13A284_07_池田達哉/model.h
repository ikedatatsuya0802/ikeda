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

typedef struct {
	LPD3DXMESH		pMesh;		// メッシュ情報
	LPD3DXBUFFER	pBuffMat;	// マテリアル情報
	DWORD			NumMat;		// マテリアル数
	int				MorphTime;
} MODELSTATUS_MORPH;			// 3Dモデル情報

//=============================================================================
//	クラス定義
//=============================================================================
class CModel : public CSceneXDX
{
public:
	CModel(bool ifListAdd = false, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CModel();

	void	Init(string fileDir, string filename);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CModel	*Create(string fileDir, string filename);
	
	void			SetParent(CModel *model) { m_Parent = model; }
	void			SetPosDef(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_PosDef = pos; }
	void			SetRotDef(D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_RotDef = rot; }

protected:
	void	LoadModel(string fileDir, string filename);
	void	AutomaticSetTexture(void);
	void	AddTexture(vector<TEXTURE> &texture, string fileName);
	string	to_string(int val);

	string			m_FileDir;
	CModel			*m_Parent;		// 親パーツ
	
	D3DXVECTOR3		m_PosDef;		// 基準座標
	D3DXVECTOR3		m_RotDef;		// 基準回転

	vector<MODELSTATUS_MORPH>	m_ModelStatus;	// 3Dモデル情報
	int	m_MorphCount;	// モーフィングカウント
	int	m_MorphStatus;	// モーフィング状態
};

#endif