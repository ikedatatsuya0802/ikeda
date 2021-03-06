#ifndef _RAIL_H_
#define _RAIL_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	rail.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	RAIL_WIDTH		(15.0f)				// 横幅
#define	RAIL_MARGIN		(50.0f)				// レールの間隔
#define	RAIL_SET		(20)				// レールの分割数
#define	RAIL_VECTOR		(4)					// レールのベクトル数
#define	RAIL_VERTEX		(RAIL_SET * 2 + 2)	// レールの頂点数
#define	RAIL_TEXFILENAME000	"rail000.png"	// ポリゴンのファイル名

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CRail : public CScene3DDX
{
public:
	void	Init(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAIL_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CRail* Create(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CRail(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CRail();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ

	int			m_RailLine;				// レールの配置
	SPLINE*		m_Spline;
};

#endif