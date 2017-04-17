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
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PRIORITY_NUM	(4)
#define	DEBUGPROC_CNT	(120)	// デバッグ情報の表示時間

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
	virtual void	Init() {};
	virtual void	Uninit(void)	= 0;
	virtual void	Update(void)	= 0;
	virtual void	Draw(void)		= 0;

	static void	UpdateAll(void);
	static void	DrawAll(void);
	static void	DeleteAll(void);
	void		Release(void);
	void		UnlinkList(void);
	
	void		AddPos(D3DXVECTOR3 pos)	{ m_Pos += pos; }
	void		AddPos(float x, float y, float z) { m_Pos += D3DXVECTOR3(x, y, z); }
	void		AddRot(D3DXVECTOR3 rot)	{ m_Rot += rot; }
	void		AddRot(float x, float y, float z) { m_Rot += D3DXVECTOR3(x, y, z); }
	void		SetScl(D3DXVECTOR3 scl) { m_Scl = scl; }
	void		SetScl(float x, float y, float z) { m_Scl = D3DXVECTOR3(x, y, z); }
	void		SetPos(D3DXVECTOR3 pos)	{ m_Pos = pos; }
	void		SetPos(float x, float y, float z) { m_Pos = D3DXVECTOR3(x, y, z); }
	void		SetRot(D3DXVECTOR3 rot)	{ m_Rot = rot; }
	void		SetRot(float x, float y, float z) { m_Rot = D3DXVECTOR3(x, y, z); }
	D3DXVECTOR3	GetPos(void)			{ return m_Pos; }
	D3DXVECTOR3	GetRot(void)			{ return m_Rot; }

	static list<CSceneDX*> GetList(void) { return m_List[0]; }
	static list<CSceneDX*> GetList(int priority) { return (priority < PRIORITY_NUM) ? m_List[priority] : m_List[0]; }

	void		ChangeDrawFrag(void)	{ m_flgDraw = m_flgDraw ? false : true; }
		
protected:
	CSceneDX(bool ifListAdd = true, int priority = 1, OBJTYPE objType = OBJTYPE_NONE);
	~CSceneDX();

	static list<CSceneDX*>	m_List[PRIORITY_NUM];	// リスト
	static list<CSceneDX*>::iterator m_ListItr;		// リストのイテレータ

	OBJTYPE	m_ObjType;		// オブジェクトタイプ

	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3 m_Scl;		// 位置
	D3DXVECTOR3 m_Pos;		// 位置
	D3DXVECTOR3 m_Rot;		// 回転角

	bool	m_flgDraw;		// 描画フラグ
	int		m_DebugProcCnt;	// デバッグ情報の表示カウンタ
};

#endif