#ifndef _SCENEDX_H_
#define _SCENEDX_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene.cpp)
//	ファイル名	scene.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/19
//
//=============================================================================
#include "d3dx9.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PRIORITY_NUM	(4)

//=============================================================================
//	構造体・列挙体
//=============================================================================
typedef enum{
	OBJTYPE_NONE = 0,
	OBJTYPE_PLAYER,
	OBJTYPE_ENEMY,
	OBJTYPE_MAX
} OBJTYPE;

//=============================================================================
//	クラス定義
//=============================================================================
class CSceneDX
{
public:
	virtual void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f))		= 0;
	virtual void	Uninit(void)	= 0;
	virtual void	Update(void)	= 0;
	virtual void	Draw(void)		= 0;

	static void	UpdateAll(void);
	static void	DrawAll(void);
	static void	DeleteAll(void);
	void		Release(void);
	void		UnlinkList(void);
	
	void		AddPos(D3DXVECTOR3 pos)	{ m_Pos += pos; }
	void		AddRot(D3DXVECTOR3 rot)	{ m_Rot += rot; }
	void		SetPos(D3DXVECTOR3 pos)	{ m_Pos = pos; }
	void		SetRot(D3DXVECTOR3 rot)	{ m_Rot = rot; }
	D3DXVECTOR3	GetPos(void)			{ return m_Pos; }
	D3DXVECTOR3	GetRot(void)			{ return m_Rot; }


	void	ChangeDrawFrag(void) { m_flgDraw ? false : true; }

protected:
	CSceneDX(int priority = 1, OBJTYPE objType = OBJTYPE_NONE);
	~CSceneDX();

	static CSceneDX *m_pTop;	// リストの先頭ポインタ
	static CSceneDX *m_pCur;	// リストの終端ポインタ

	OBJTYPE		m_ObjType;	// オブジェクトタイプ

	CSceneDX *m_pPrev;		// 前参照先ポインタ
	CSceneDX *m_pNext;		// 後参照先ポインタ

	D3DXVECTOR3 m_Pos;		// 位置
	D3DXVECTOR3 m_Rot;		// 回転角

	bool	m_flgDraw;		// 描画フラグ
};

#endif