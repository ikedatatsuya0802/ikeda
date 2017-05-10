#ifndef _PAUSE_H_
#define _PAUSE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(pause.cpp)
//	ファイル名	pause.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene2DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PAUSE_TEXTURE_NUM				(5)
#define	PAUSE_TEXFILENAME000			"pause000.png"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME001			"pause001.jpg"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME002			"pause002.png"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME003			"pause003.png"						// ポーズポリゴンのファイル名
#define	PAUSE_TEXFILENAME004			"pause004.png"						// ポーズポリゴンのファイル名

#define	PAUSE_WIDTH			(SCREEN_HEIGHT * 0.8f)		// ポーズの横幅
#define	PAUSE_HEIGHT		(SCREEN_HEIGHT * 0.8f)		// ポーズの縦幅
#define	PAUSE_POSX			(SCREEN_WIDTH * 0.5f)		// ポーズのX位置
#define	PAUSE_POSY			(SCREEN_HEIGHT * 0.5f)		// ポーズのY位置

#define	PAUSE_MENU_WIDTH	(PAUSE_WIDTH * 0.54f)					// ポーズメニューの縦幅
#define	PAUSE_MENU_HEIGHT	(PAUSE_HEIGHT * 0.085f)					// ポーズメニューの横幅
#define	PAUSE_MENU_POSX		(PAUSE_POSX * 1.014f)					// ポーズメニューのX位置
#define	PAUSE_MENU_POSY		(PAUSE_POSY - (PAUSE_WIDTH * 0.401f))	// ポーズメニューのY位置

#define	PAUSE_MOVE_SPEED		(1.5f)								// ポーズの移動速度
#define	PAUSE_STOP_SPEED		(PAUSE_MOVE_SPEED * 0.1f)			// 移動速度の減速量
#define	PAUSE_SCOPE_SPEED		(10.0f)								// ポーズの拡縮速度
#define	PAUSE_ROT_SPEED			(0.05f)								// ポーズの回転速度
#define	PAUSE_ROT_STOP_SPEED	(1.0f)								// 回転速度の減速量

typedef enum {
	PAUSE_MENU_CONTINUE = 0,	// 再開
	PAUSE_MENU_RETRY,			// 再挑戦
	PAUSE_MENU_QUIT,			// 終了
	PAUSE_MENU_MAX				// ポーズメニューの最大値
}PAUSE_MENU;

//=============================================================================
//	クラス定義
//=============================================================================
class CPause : public CScene2DDX
{
public:
	CPause(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CPause();

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CPause	*Create(void);

	void ChangePause(void) { m_flgPause = m_flgPause ? false : true; }
	bool GetPause(void) { return m_flgPause; }

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff);

	LPDIRECT3DTEXTURE9	m_pTexture[PAUSE_TEXTURE_NUM];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[3];

	bool	m_flgPause;	// ポーズ中かどうか
	int		m_PauseMenu;
};

#endif