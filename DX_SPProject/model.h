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
#include "sceneDX.h"
#include "sceneXDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define SPLINE_BUFF	(0.05f)

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CPlayer;
class CModel : public CSceneXDX
{
public:
	CModel(bool ifListAdd = false, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CModel();

	void	Init(char *filename, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	void			SetParent(CModel *model) { m_Parent = model; }
	void			SetPosDef(D3DXVECTOR3 pos = VEC3_ZERO) { m_PosDef = pos; }
	void			SetRotDef(D3DXVECTOR3 rot = VEC3_ZERO) { m_RotDef = rot; }
	void			SetPos(D3DXVECTOR3 pos = VEC3_ZERO) { m_Pos = pos; }
	void			SetRot(D3DXVECTOR3 rot = VEC3_ZERO) { m_Rot = rot; }

	static CModel	*Create(char *filename, D3DXVECTOR3 pos = VEC3_ZERO);

private:
	void	LoadModel(char *filename);
	void	AutomaticSetTexture(void);
	void	AddTexture(vector<TEXTURE> &texture, char* fileName);

	CModel				*m_Parent;		// 親パーツ
	
	D3DXVECTOR3			m_PosDef;		// 基準座標
	D3DXVECTOR3			m_RotDef;		// 基準回転
	D3DXVECTOR3			m_Pos;			// 座標
	D3DXVECTOR3			m_Rot;			// 回転
	D3DXMATRIX			m_mtxWorld;		// ワールドマトリックス
	MODELSTATUS			m_ModelStatus;	// 3Dモデル情報
	
	D3DXMATRIX			GetWorldMatrix(void) { return m_mtxWorld; }	// ワールドマトリックス受け渡し
};

#endif