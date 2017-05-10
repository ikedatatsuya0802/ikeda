#ifndef _BUILDING_H_
#define _BUILDING_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	building.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
#include "sceneXDX.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define		BUILDING_SCALE			(5.0f)		// 建物のスケール
const int	BUILDING_TYPE_NUM		= 17;		// 建物の種類
const float	BUILDING_MARGIN			= 1000.0f;	// 建物同士の間隔
const int	BUILDING_INSIDE			= 15;		// 建物の縦ブロック数
const int	BUILDING_HORIZONTAL		= 15;		// 建物の横ブロック数
const int	BUILDING_ALL			= BUILDING_INSIDE * BUILDING_HORIZONTAL;	// 建物の総ブロック数
const float	BUILDING_CLEAR_LENGTH	= BUILDING_MARGIN;	// 建物を非表示にする距離
const float	BUILDING_INVISIBLE_FAR	= 2000.0f;	// 建物を非表示にする距離

const float	BUILDING_STRIDE_X		= BUILDING_MARGIN * BUILDING_HORIZONTAL;	// 建物の縦ブロック数
const float	BUILDING_STRIDE_Z		= BUILDING_MARGIN * BUILDING_INSIDE;		// 建物の縦ブロック数
const float	BUILDING_MOVE_STRIDE	= BUILDING_MARGIN * 5.0f;						// 建物の縦ブロック数

typedef struct {
	uint		Type;
	bool		Visible;
	D3DXVECTOR3	Pos;
} BUILDING;

//=============================================================================
//	クラス定義
//=============================================================================
class CBuilding
{
public:
	static void	Init(void);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);

	static void	RecalcTexCoord(LPD3DXMESH* mesh);

protected:
	static bool			m_ifInitialize;	// 初期化完了フラグ
	static SPLINE*		m_Spline;		// スプライン情報
	static D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス

	static MODELSTATUS		m_BuildingMesh[BUILDING_TYPE_NUM];	// メッシュへのポインタ
	static D3DXMATERIAL*	m_BuildingMat[BUILDING_TYPE_NUM];	// メッシュへのポインタ
	static BUILDING			m_Building[BUILDING_ALL];

	static uint __stdcall	UpdateS(void*);
	static uint				m_thID;	// スレッドID1
	static HANDLE			m_hTh;	// スレッドハンドル1
};

#endif