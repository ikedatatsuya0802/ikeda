#ifndef _RAILLINE_H_
#define _RAILLINE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	railLine.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include <vector>
using namespace std;

//=============================================================================
//	マクロ定義
//=============================================================================
#define	RAILLINE_WIDTH			(15.0f)					// 横幅
#define	RAILLINE_MARGIN			(50.0f)					// レールの間隔
#define	RAILLINE_SET			(20)					// レールの分割数
#define	RAILLINE_VECTOR			(4)						// レールのベクトル数
#define	RAILLINE_VERTEX			(RAILLINE_SET * 2 + 2)	// レールの頂点数
#define	RAILLINE_TEXFILENAME000	"effect000.jpg"			// テクスチャのファイル名
#define	RAILLINE_SPOINT_SIZE	(20.0f)					// レール頂点の確認用エフェクトサイズ
#define	RAILLINE_LPOINT_SIZE	(50.0f)					// スプライン頂点の確認用エフェクトサイズ
#define	RAILLINE_DRAG_SIZE		(50.0f)					// ドラッグの選択半径

//=============================================================================
//	構造体
//=============================================================================

typedef struct {
	vector<D3DXVECTOR3>	Pos;			// スプラインの誘導点
	vector<D3DXVECTOR3>	Vec;			// スプラインの誘導ベクトル
	vector<D3DXVECTOR3>	PosHermite;		// スプライン分割点
	D3DXVECTOR3			Rot;			// スプライン上での(主にY軸の)回転値
	vector<bool>		ifHold;			// マウスでホールド中かどうか
} SPLINE;

//=============================================================================
//	クラス定義
//=============================================================================
class CRailLine : public CScene3DDX
{
public:
	void	Init(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAILLINE_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CRailLine	*Create(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SPLINE* GetSpline(void) { return &m_Spline; }

private:
	CRailLine(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CRailLine();

	void	SaveSpline();
	void	LoadSpline(int line = 0);
	void	CalcSpline(int line = 0);
	void	SetSplineVtx(int line = 0);
	void	SetSplineVtxVec(int line = 0);
	void	SetSplineVtxSPoints(int line = 0);
	void	SetSplineVtxLPoints(int line = 0);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffVec;		// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffSPoints;	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLPoints;	// 頂点バッファへのポインタ
	
	D3DXMATRIX	m_mtxWorld;				// ワールドマトリックス
	int			m_RailLineLine;				// レールの配置
	SPLINE		m_Spline;
};

#endif