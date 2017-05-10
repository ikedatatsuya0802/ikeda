#ifndef _GOAL_H_
#define _GOAL_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	goal.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "sceneXDX.h"
#include "rail.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	RAIL_LENGTH	(10000.0f)

//=============================================================================
//	構造体
//=============================================================================
typedef struct {
	D3DXVECTOR3 nearPos;
	D3DXVECTOR3 farPos;
} GOALPOS;

//=============================================================================
//	クラス定義
//=============================================================================
class CGoal : public CScene3DDX
{
public:
	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CGoal* Create(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAIL_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	GOALPOS GetGoalPos(void) {
		GOALPOS pos ={ m_Pos, m_PosFar };
		return pos;
	}

protected:
	CGoal(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CGoal();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff[2];

	D3DXVECTOR3 m_PosFar;
	D3DXVECTOR3 m_Vec;

	CSceneXDX* m_Instance[2];
};

#endif