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
#define	RAILLINE_TEXFILENAME000	"effect000.jpg"			// テクスチャのファイル名
#define	RAILLINE_TEXFILENAME001	"drift000.png"			// テクスチャのファイル名
#define	RAILLINE_TEXFILENAME002	"drift001.png"			// テクスチャのファイル名
#define	RAILLINE_TEXFILENAME003	"drift002.png"			// テクスチャのファイル名

#define	RAILLINE_GOAL			(0.98f)					// ゴール地点のスプライン座標
#define	RAILLINE_LENGTH			(10000.0f)				// 横幅
#define	RAILLINE_WIDTH			(15.0f)					// 横幅
#define	RAILLINE_MARGIN			(50.0f)					// レールの間隔
#define	RAILLINE_SET			(100)					// レールの分割数
#define	RAILLINE_VECTOR			(4)						// レールのベクトル数
#define	RAILLINE_VERTEX			(RAILLINE_SET * 2 + 2)	// レールの頂点数
#define	RAILLINE_SPOINT_SIZE	(20.0f)					// レール頂点の確認用エフェクトサイズ
#define	RAILLINE_LPOINT_SIZE	(50.0f)					// スプライン頂点の確認用エフェクトサイズ
#define	RAILLINE_DRAG_SIZE		(100.0f)				// ドラッグの選択半径
#define	RAILLINE_DRIFT_SEARCH	(1000)					// マウスからの最近点の探索数
#define	RAILLINE_DRIFT_DRAG		(0.02f)					// ドリフトマークの可動範囲

#define	RAILLINE_SLOPE			(1.0f)					// 坂の勾配

//=============================================================================
//	構造体
//=============================================================================
typedef enum {
	ET_POSVEC = 0,
	ET_DRIFT,
	ET_MAX
} EDIT_TYPE;		// ドリフト点情報

typedef struct {
	float	Begin;		// ドリフトの開始点
	float	End;		// ドリフトの終了点
	bool	Curve;		// どちらにカーブするか(false:左, true:右)
	bool	ifHoldBegin;// ホールド情報
	bool	ifHoldEnd;	// ホールド情報
} DRIFT_POINT;			// ドリフト点情報

typedef struct {
	vector<D3DXVECTOR3>	Pos;		// スプラインの誘導点
	vector<D3DXVECTOR3>	Vec;		// スプラインの誘導ベクトル
	vector<D3DXVECTOR3>	PosHermite;	// スプライン分割点
	D3DXVECTOR3			Rot;		// スプライン上での(主にY軸の)回転値
	vector<int> 		ifHold;		// マウスでホールド中かどうか
	vector<DRIFT_POINT>	Drift;		// ドリフト点情報
	vector<float>		LengthMin;	// スプライン区間長(ざっくり)
	float				Length;		// スプライン長(ざっくり)
} SPLINE;

typedef struct {
	bool	ifDrift;	// ドリフト範囲か
	bool	Curve;		// どちらのカーブか
	int		Status;		// ドリフトのどの状態か　-1->始点, 0->ドリフト中, 1->終点
} DRIFT_STATUS;			// ドリフトステータス

//=============================================================================
//	クラス定義
//=============================================================================
class CRailLine : public CScene3DDX
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextures[2];	// テクスチャへのポインタ

public:
	void	Init(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CRailLine	*Create(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	
	// リソースのロード
	static void	Load(void) {
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAILLINE_TEXFILENAME000, &m_pTextures[0]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAILLINE_TEXFILENAME001, &m_pTextures[1]);
	}

	static void	Unload(void) {
		SafetyRelease(m_pTextures[0]);
		SafetyRelease(m_pTextures[1]);
	}

	SPLINE* GetSpline(void) { return &m_Spline; }
	D3DXVECTOR3	GetSplinePos(float t);
	DRIFT_STATUS GetDriftStatus(float oldt, float t);

	D3DXVECTOR3 GetMoveVec(float t);
	static float AngleOf2Vector(D3DXVECTOR3 a, D3DXVECTOR3 b);

private:
	CRailLine(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CRailLine();

	void	MouseEdit();
	void	AddPoints();
	void	DeletePoints();
	void	SaveSpline();
	void	LoadSpline(int line = 0);
	void	CalcSpline(int line = 0);
	void	SetSplineVtx(int line = 0);
	void	SetSplineVtxVec(int line = 0);
	void	SetSplineVtxSPoints(int line = 0);
	void	SetSplineVtxLPoints(int line = 0);
	void	SetSplineVtxPointer(int line = 0);
	void	SetSplineVtxDrift(int line = 0);
	
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffVec;		// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffSPoints;	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLPoints;	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffPointer;	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffDrift;	// 頂点バッファへのポインタ


	EDIT_TYPE	m_EditType;		// エディットモード、0->Pos, 1->Vec, 2->Drift
	float		m_YScale;		// カメラの高さに応じたスケール
	int			m_RailLine;		// レールの配置
	SPLINE		m_Spline;		// スプライン情報
};

#endif