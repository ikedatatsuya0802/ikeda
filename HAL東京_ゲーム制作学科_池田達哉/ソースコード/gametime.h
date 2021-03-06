#ifndef _GAMETIME_H_
#define _GAMETIME_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(gametime.cpp)
//	ファイル名	gametime.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/21
//
//=============================================================================
#include "number.h"

//=============================================================================
//	マクロ定義
//=============================================================================

//=============================================================================
//	構造体
//=============================================================================
typedef struct FIGURE{
	FIGURE(int upper = 0, bool dot = 0, int lower = 0) { Upper = upper; Dot = dot; Lower = lower; }	// キャスタ

	int		Upper;	// 整数部
	bool	Dot;	// 小数点
	int		Lower;	// 小数部
} FIGURE;			// 浮動小数点桁型

//=============================================================================
//	クラス定義
//=============================================================================
class CGametime
{
public:
	CGametime(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CGametime();
	
	void	Init(cVec3 pos, cVec2 size, FIGURE figure);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CGametime* Create(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE,
		cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO, FIGURE figure = 0);

	void	SetValue(float value);

protected:
	static FIGURE	m_Figure;
	static int		m_NumFigure;

	CNumber* *m_Number;
	float	m_Time;
};

#endif