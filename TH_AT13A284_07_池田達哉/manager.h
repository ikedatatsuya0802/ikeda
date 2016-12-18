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
#include "main.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	SCENE_NUM		(4)		// シーン数

//=============================================================================
//	前方宣言
//=============================================================================
class CCameraDX;
class CMeshfield;
class CSkyBox;
class CModel;

//=============================================================================
//	クラス定義
//=============================================================================
class CManager
{
public:
	static void	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);
	
	static CCameraDX	*GetCamera(void){ return m_Camera; }
	static CModel		*GetModel(void) { return m_Model; }
	static int			GetFrame(void) { return m_Frame; }

private:
	static CCameraDX	*m_Camera;		// カメラのインスタンス
	static CModel		*m_Model;		// メッシュフィールドのインスタンス

	static int	m_Frame;	// 汎用カウンタ
};

#endif