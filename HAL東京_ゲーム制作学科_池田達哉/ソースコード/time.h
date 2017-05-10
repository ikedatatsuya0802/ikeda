#ifndef _TIME_H_
#define _TIME_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(time.cpp)
//	ファイル名	time.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2017/01/21
//
//=============================================================================
#include "scene2DDX.h"
#include "number.h"
#include <windows.h>

//=============================================================================
//	マクロ定義
//=============================================================================
#define	TIME_FILENAME	".\\data\\time.txt"	// 時間の保存ファイル

const int TIME_FIGURE = 8;	// 時間の桁数(mm:ss:mm)

//=============================================================================
//	マクロ定義
//=============================================================================
typedef struct {
	int minute;		// 分
	int second;		// 秒
	int millisec;	// ミリ秒
public:
	void SetTime(int min, int sec, int mil) { minute = min; second = sec; millisec = mil; }
} TIME;

//=============================================================================
//	クラス定義
//=============================================================================
class CTime : public CScene2DDX
{
public:
	CTime(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objType = OBJTYPE_NUMBER);
	~CTime();
	
	void	Init(DWORD time = 0, cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CTime* Create(DWORD time = 0, cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO);

	void SetColor(cfloat a = 1.0f, cfloat r = 1.0f, cfloat g = 1.0f, cfloat b = 1.0f);
	void CountStart(void) { m_ifCountStart = true; m_StartTime = timeGetTime(); }
	void CountStop(void) { m_ifCountStart = false; }
	void SaveTime(void);

	static void TimeConvert(TIME* timeStr, DWORD time);

protected:
	CScene2DDX*		m_Instance[TIME_FIGURE];	// ナンバーのインスタンス
	bool			m_ifCountStart;				// カウント中かどうか
	DWORD			m_StartTime;				// 計測開始時間
	DWORD			m_TimeValue;				// 経過時間
	TIME			m_Time;						// 分秒ミリ秒の時間
};

#endif