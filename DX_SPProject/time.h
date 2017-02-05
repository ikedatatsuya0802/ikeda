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
	CTime(int value = 0);
	~CTime();
	
	void	Init(int value, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CTime	*Create(int value = 0, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);

	void CountStart(void) { m_ifCountStart ? 0 : true; m_StartTime = timeGetTime(); }
	void CountStop(void) { m_ifCountStart = false; }
	void SaveTime(void);

protected:
	vector<CNumber>	m_Instance;		// ナンバーのインスタンス
	bool			m_ifCountStart;	// カウント中かどうか
	DWORD			m_StartTime;	// 計測開始時間
	TIME			m_Time;			// 分秒ミリ秒の時間
};

#endif