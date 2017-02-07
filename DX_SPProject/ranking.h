#ifndef _RANKING_H_
#define _RANKING_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(ranking.cpp)
//	ファイル名	ranking.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2017/01/21
//
//=============================================================================
#include "main.h"
#include "scene2DDX.h"
#include "number.h"
#include <windows.h>

//=============================================================================
//	マクロ定義
//=============================================================================
#define	RANKING_FILENAME	".\\data\\ranking.txt"	// 時間の保存ファイル
cint	RANKING_NUM			= 10;


//=============================================================================
//	クラス定義
//=============================================================================
class CRanking : public CScene2DDX
{
public:
	CRanking(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objType = OBJTYPE_NUMBER);
	~CRanking();
	
	void	Init(cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO, cfloat margin = 0.0f);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CRanking	*Create(cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO, cfloat margin = 0.0f);

	void SaveRanking(void);
	void LoadRanking(void);
	
private:
	DWORD			m_NowValue;
	vector<DWORD>	m_Ranking;
	vector<DWORD>	m_NewRanking;
};

#endif