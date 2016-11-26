#ifndef _GAME_H_
#define _GAME_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	game.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
#include "mode.h"

//=============================================================================
//	前方宣言
//=============================================================================
class CMeshfield;
class CPlayer;

//=============================================================================
//	クラス定義
//=============================================================================
class CGame : public CMode
{
public:
	CGame(){}
	~CGame(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CMeshfield	*GetMeshfield(void){ return m_Meshfield; }
	static CPlayer		*GetPlayer1(void){ return m_Player1; }
	static CPlayer		*GetPlayer2(void){ return m_Player2; }

private:
	static CMeshfield	*m_Meshfield;	// メッシュフィールドのインスタンス
	static CPlayer		*m_Player1;		// プレイヤーのインスタンス
	static CPlayer		*m_Player2;		// プレイヤーのインスタンス
};

#endif