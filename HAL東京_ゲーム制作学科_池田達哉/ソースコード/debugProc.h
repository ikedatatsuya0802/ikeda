#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	debugProc.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/26
//
//=============================================================================
#include "d3dx9.h"
//=============================================================================
//	マクロ定義
//=============================================================================
#define	DEBUGSTR_MAX	(65535)	// 配列限界

//=============================================================================
//	プロトタイプ
//=============================================================================
class CDebugProc
{
public:
	static HRESULT	Init(void);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);
	static void	Clear(void) { memset(m_aStrDebug, 0, sizeof(m_aStrDebug)); }

	static void	DebugProc(char *fmt, ...);

private:
	static LPD3DXFONT	m_pFont;					// フォントへのポインタ
	static char			m_aStrDebug[DEBUGSTR_MAX];	// デバッグ用文字列

};

#endif