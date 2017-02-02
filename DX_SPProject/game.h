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
//	マクロ定義
//=============================================================================
#define	GOAL_COUNT	(180)	// ゴールしてからフェードまでの字間

//=============================================================================
//	前方宣言
//=============================================================================
class CMeshfield;
class CPlayer;
class CRailLine;
class CDriftMark;
class CPause;

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
	static int		GetFrame(void) { return m_Frame; }

	static CMeshfield	*GetMeshfield(void){ return m_Meshfield; }
	static CRailLine	*GetRailLine(void) { return m_RailLine; }
	static CPlayer		*GetPlayer1(void){ return m_Player1; }
	static CPlayer		*GetPlayer2(void){ return m_Player2; }
	static CDriftMark	*GetDriftMark(void) { return m_DriftMark; }
	static CScene2DDX	*GetHakushin(void) { return m_Hakushin; }
	static CPause		*GetPause(void) { return m_Pause; }

private:
	static CMeshfield	*m_Meshfield;	// メッシュフィールドのインスタンス
	static CRailLine	*m_RailLine;	// レール情報のインスタンス
	static CPlayer		*m_Player1;		// プレイヤーのインスタンス
	static CPlayer		*m_Player2;		// プレイヤーのインスタンス
	static CDriftMark	*m_DriftMark;	// ドリフトマークのインスタンス
	static CScene2DDX	*m_Hakushin;	// 集中線のインスタンス
	static CPause		*m_Pause;		// ポーズ画面のインスタンス

	static int	m_Frame;	// 汎用カウンタ

	static int	m_GoalCount;
};

#endif