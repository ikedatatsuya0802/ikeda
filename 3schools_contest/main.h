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
#include "readfile.h"
#include <vector>
#include <list>
#include <random>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

//=============================================================================
//	リンク
//=============================================================================
#pragma comment(lib, "winmm.lib")

//=============================================================================
//	マクロ定義
//=============================================================================
#define SIMPLE_FOR(max)			for(int i=0;i<max;i++)	// 単純複数回処理
#define	uchar					unsigned char
#define	uint					unsigned int
#define	GAME_FPS				(60)
#define	DEFAULT_SCREEN_WIDTH	(1024.0f)	// ウィンドウの横幅
#define	DEFAULT_SCREEN_HEIGHT	(768.0f)	// ウィンドウの縦幅
#define	SCREEN_WIDTH			(1024.0f)	// ウィンドウの横幅
#define	SCREEN_HEIGHT			(768.0f)	// ウィンドウの縦幅
#define	SCREEN_WIDTH_HALF		(SCREEN_WIDTH * 0.5f)	// ウィンドウの横幅の半分
#define	SCREEN_HEIGHT_HALF		(SCREEN_HEIGHT * 0.5f)	// ウィンドウの縦幅の半分

//=============================================================================
//	構造体
//=============================================================================
typedef struct {
	float r;
	float g;
	float b;
} COLOR3;
#define FCOLOR(x)	(x/255.0f)

//=============================================================================
//	プロトタイプ
//=============================================================================
int		GetFPS(void);
HWND	GethWnd(void);
char*	StringToCharP(const string str);
string	CharPToString(const char* str);
vector<std::string> split(const string &str, char sep);

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