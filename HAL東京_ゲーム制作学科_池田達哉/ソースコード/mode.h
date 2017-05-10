#ifndef _MODE_H_
#define _MODE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	mode.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CMode
{
public:
	virtual void	Init(void)		= 0;
	virtual void	Uninit(void)	= 0;
	virtual void	Update(void)	= 0;
	virtual void	Draw(void)		= 0;

protected:
};
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"

#endif