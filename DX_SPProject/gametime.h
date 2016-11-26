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
class CGametime : public CNumber
{
public:
	CGametime(int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CGametime();
	
	void	Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CGametime	*Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure);

	void	SetValue(float value);

protected:
	static FIGURE	m_Figure;
	static int		m_NumFigure;

	CNumber	**m_Number;
	float	m_Time;
};

#endif