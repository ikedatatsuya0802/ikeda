#ifndef _MAIN_H_
#define _MAIN_H_
//=============================================================================
//
//	タイトル ヘッダファイル(main.cpp)
//	ファイル名 main.h
//	作成者 AT13A284_07 池田達哉
//	作成日 2016/04/13
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
//=============================================================================
//	インクルード
//=============================================================================
#include <windows.h>
#include "input.h"
#include "readfile.h"
#include <vector>
#include <list>
using namespace std;

//=============================================================================
//	リンク
//=============================================================================
#pragma comment(lib, "winmm.lib")

//=============================================================================
//	マクロ定義
//=============================================================================
#define	SCREEN_WIDTH		(1280.0f)	// ウィンドウの横幅
#define	SCREEN_HEIGHT		(760.0f)	// ウィンドウの縦幅
#define	SCREEN_WIDTH_HALF	(SCREEN_WIDTH * 0.5f)	// ウィンドウの横幅の半分
#define	SCREEN_HEIGHT_HALF	(SCREEN_HEIGHT * 0.5f)	// ウィンドウの縦幅の半分

//=============================================================================
//	プロトタイプ
//=============================================================================
int		GetFPS(void);
HWND	GethWnd(void);
//void ChangeFlag(bool &Flag) { Flag = Flag ? false : true; } // フラグ変更関数

// インスタンス削除関数
template <class INSTANCE>
void SafetyRelease(INSTANCE instance)
{
	if(instance != NULL)
	{
		instance->Release();
		instance = NULL;
	}
}

// インスタンス削除関数(自作クラス版)
template <class INSTANCE>
void SafetyUninit(INSTANCE instance)
{
	if(instance != NULL)
	{
		instance->Uninit();
		delete instance;
		instance = NULL;
	}
}

// データ交換関数
template <class SWAPDATA>
void SwapData(SWAPDATA &data1, SWAPDATA &data2)
{
	SWAPDATA dataBuff;
	dataBuff = data1;
	data1 = data2;
	data2 = dataBuff;
}


#endif