#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	tutorial.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
#include "mode.h"
class CScene2DDX;
//=============================================================================
//	クラス定義
//=============================================================================
class CTutorial : public CMode
{
public:
	CTutorial(){}
	~CTutorial(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
private:
	CScene2DDX	*m_TutorialBG;
	CScene2DDX	*m_PushEnter;
	float		m_Alpha;
};

#endif