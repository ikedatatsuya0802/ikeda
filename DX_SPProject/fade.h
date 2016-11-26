#ifndef _FADE_H_
#define _FADE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	fade.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
#include "rendererDX.h"
class CMode;
class CScene2DDX;

//=============================================================================
//	マクロ定義
//=============================================================================
#define	FADE_SPEED		(1.0f / 60)	// フェードスピード
#define	FADE_CLOSESPEED	(30.0f)		// フェードが閉じるスピード
#define	FADE_CLOSETIME	(60)		// フェードの閉じている時間
#define	PHASETIME_NONE	(60)		// 初期フェーズの時間

//=============================================================================
//	構造体
//=============================================================================
typedef enum{
	FS_NONE = 0,// 何もしていない
	FS_FIRST,	// 初期フェード
	FS_IN,		// フェードイン
	FS_OUT,		// フェードアウト
	FS_MAX		// フェード数
}FADE_STATE;

typedef enum{
	FADE_TYPE_DOOR = 0,	// ドアフェード
	FADE_TYPE_WHITE,	// 明転フェード
	FADE_TYPE_MAX		// フェード数
}FADE_TYPE;

typedef struct{
	FADE_STATE	State;			// フェード状態
	int			nCntFade;		// フェードカウント
	float		fLength;		// フェード同士の距離
	int			nCntPhaseNone;	// 初期フェーズのカウント
}FADESTATUS;

typedef struct{
	D3DXVECTOR3	Pos;		// 位置
	D3DXVECTOR3	Rot;		// 回転角
	float		fLength;	// 対角線の長さ
	float		fAngle;		// 対角線の角度
}FADE;

//=============================================================================
//	クラス定義
//=============================================================================
class CFade
{
public:
	static void	Init(void);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);

	static void			Start(CMode *nextMode, FADE_STATE fadeState = FS_OUT);
	static FADE_STATE	GetFade(void){ return m_FadeState.State; }
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファへのポインタ
	static LPDIRECT3DTEXTURE9		m_pTexture[2];	// テクスチャへのポインタ
	static FADE						m_Fade[2];		// フェード

	static CMode		*m_NextMode;
	static FADESTATUS	m_FadeState;	// フェードの状態
};


#endif