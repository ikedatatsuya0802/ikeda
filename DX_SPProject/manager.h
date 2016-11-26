#ifndef _MANAGER_H_
#define _MANAGER_H_
//=============================================================================
//
//	タイトル	ヘッダファイル
//	ファイル名	manager.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/05/10
//
//=============================================================================
#include "d3dx9.h"
#include "debugProc.h"

//=============================================================================
//	前方宣言
//=============================================================================
class CMode;
class CInput;
class CDebugProc;
class CRendererDX;
class CSceneDX;
class CCameraDX;
class CLightDX;

//=============================================================================
//	クラス定義
//=============================================================================
class CManager
{
public:
	//=========================================================================
	//	メソッド
	//=========================================================================
	static void	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);
	
	static CCameraDX	*GetCamera(void){ return m_Camera; }
	
	static CMode		*GetMode(void){ return m_Mode; }
	static void			SetMode(CMode *mode);

private:
	//=========================================================================
	//	メンバ変数
	//=========================================================================
	static CMode		*m_Mode;		// モードのインスタンス
	static CCameraDX	*m_Camera;		// カメラのインスタンス
	static CLightDX		*m_Light;		// ライトのインスタンス

};

#endif