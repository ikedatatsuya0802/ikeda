#ifndef _WIRING_H_
#define _WIRING_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	wiring.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "sceneXDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	WIRING_CLEAR_BORDER	(0.01f)	// 透明にする境界のスプライン座標差

//=============================================================================
//	クラス定義
//=============================================================================
class CWiring : public CSceneXDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CWiring	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CWiring(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CWiring();

	void SetVtxBuff(void);
	
	vector<LPDIRECT3DVERTEXBUFFER9>	m_pVtxBuff;	// 頂点バッファへのポインタ

	SPLINE*		m_Spline;
};

#endif