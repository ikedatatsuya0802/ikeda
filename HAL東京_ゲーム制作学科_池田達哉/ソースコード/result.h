#ifndef _RESULT_H_
#define _RESULT_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	result.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
#include "mode.h"
class CScene2DDX;
//=============================================================================
//	クラス定義
//=============================================================================
class CResult : public CMode
{
public:
	CResult(){}
	~CResult(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static int		GetFrame(void) { return m_Frame; }

private:
	static int	m_Frame;	// 汎用カウンタ
	CScene2DDX *m_ResultBG;
	CScene2DDX *m_PushEnter;
	float		m_Alpha;
};

#endif