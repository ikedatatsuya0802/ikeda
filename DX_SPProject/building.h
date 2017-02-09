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
const int	BUILDING_TYPE_NUM		= 17;		// 建物の種類
const float	BUILDING_MARGIN			= 100.0f;	// 建物同士の間隔
const int	BUILDING_INSIDE			= 5;		// 建物の縦ブロック数
const int	BUILDING_HORIZONTAL		= 5;		// 建物の横ブロック数
const float	BUILDING_CLEAR_LENGTH	= 200.0f;	// 建物を非表示にする距離
const float	BUILDING_INVISIBLE_FAR	= 2000.0f;	// 建物を非表示にする距離

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

protected:
	static bool		m_ifInitialize;	// 初期化完了フラグ
	static SPLINE*	m_Spline;		// スプライン情報

	static vector<CSceneXDX*> m_Instance;					// 建物のインスタンス
	static MODELSTATUS m_BuildingMesh[BUILDING_TYPE_NUM];	// メッシュへのポインタ

	static uint __stdcall	UpdateThread(void*);
	static uint				m_thIDUpdate;			// スレッドID1
	static HANDLE			m_hThUpdate;			// スレッドハンドル1
	static uint __stdcall	DrawThread(void*);
	static HANDLE			m_hThDraw;				// スレッドハンドル1
	static uint				m_thIDDraw;				// スレッドID1
};

#endif