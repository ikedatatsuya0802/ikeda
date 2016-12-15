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

//=============================================================================
//	マクロ定義
//=============================================================================

//=============================================================================
//	構造体
//=============================================================================
typedef struct{
	LPD3DXMESH		pMesh;		// メッシュ情報
	LPD3DXBUFFER	pBuffMat;	// マテリアル情報
	DWORD			NumMat;		// マテリアル数
} MODELSTATUS;					// 3Dモデル情報

typedef struct {
	char*				FileName;	// メッシュ情報
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
} TEXTURE;	// テクスチャ情報

//=============================================================================
//	クラス定義
//=============================================================================
class CPlayer;
class CModel
{
public:
	CModel(int priority = 1);
	~CModel();

	void	Init(char *filename, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	void			SetParent(CModel *model) { m_Parent = model; }
	void			SetPosDef(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_PosDef = pos; }
	void			SetRotDef(D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_RotDef = rot; }
	void			SetPos(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_Pos = pos; }
	void			SetRot(D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_Rot = rot; }

	static CModel	*Create(char *filename, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void			LoadModel(char *filename);

private:
	vector<TEXTURE>	m_Texture;	// テクスチャへのポインタ
	void			AutomaticSetTexture(void);
	void			AddTexture(vector<TEXTURE> &texture, char* fileName);

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