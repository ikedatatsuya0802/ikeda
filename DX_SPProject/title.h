#ifndef _TITLE_H_
#define _TITLE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	title.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
#include "mode.h"
class CScene2DDX;
#define	TITLE_START_FRAME	(1300)
//=============================================================================
//	クラス定義
//=============================================================================
class CTitle : public CMode
{
public:
	CTitle(){}
	~CTitle(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static int		GetFrame(void) { return m_Frame; }

private:
	static int	m_Frame;	// 汎用カウンタ

	CScene2DDX* m_Black;
	CScene2DDX* m_TitleBG;
	CScene2DDX* m_PushEnter;
	float		m_Alpha;
	float		m_AlphaBlack;
	float		m_AlphaButton;
};

#endif