#ifndef _ESTRUCTURE_H_
#define _ESTRUCTURE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	EStructure.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	ESTRUCTURE_WIDTH		(15.0f)				// 横幅
#define	ESTRUCTURE_MARGIN		(50.0f)				// レールの間隔
#define	ESTRUCTURE_SET		(20)				// レールの分割数
#define	ESTRUCTURE_VECTOR		(4)					// レールのベクトル数
#define	ESTRUCTURE_VERTEX		(ESTRUCTURE_SET * 2 + 2)	// レールの頂点数
#define	ESTRUCTURE_TEXFILENAME000	"EStructure000.png"	// ポリゴンのファイル名

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CEStructure : public CScene3DDX
{
public:
	void	Init(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ESTRUCTURE_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CEStructure	*Create(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CEStructure(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CEStructure();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ

	int			m_EStructureLine;				// レールの配置
	SPLINE*		m_Spline;
};

#endif