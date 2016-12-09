//=============================================================================
//
//	タイトル	メインファイル
//	ファイル名	main.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/13
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "main.h"
#include "manager.h"

//=============================================================================
//	プロトタイプ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
//	グローバル変数
//=============================================================================
static HWND		g_hWnd;						// ウィンドウハンドル
static int		g_nCountFPS;				// FPSカウンタ

//=============================================================================
//	関数名	:WinMain
//	引数	:HINSTANCE hInstance
//			:HINSTANCE hPrevInstance
//			:LPSTR lpCmdLine
//			:int nCmdLine
//	戻り値	:int
//	説明	:メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0, hInstance, NULL,
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, "NullWindow", NULL
	};
	MSG		msg;
	RECT	rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	DWORD	dwFrameCount;
	DWORD	dwCurrentTime;
	DWORD	dwExecLastTime;
	DWORD	dwFPSLastTime;
	int		frx, fry, cpy;


	timeBeginPeriod(1);
	dwFrameCount =
		dwCurrentTime = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// ウィンドウを作成
	frx = GetSystemMetrics(SM_CXFIXEDFRAME);
	fry = GetSystemMetrics(SM_CYFIXEDFRAME);
	cpy = GetSystemMetrics(SM_CYCAPTION);
	RegisterClassEx(&wcex);
	g_hWnd = CreateWindowEx(
		0, "NullWindow", "Everlasting Rail", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left + GetSystemMetrics(SM_CXFIXEDFRAME) * 2),
		(rect.bottom - rect.top + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL, NULL, hInstance, NULL);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	// 初期化処理
	CManager::Init(hInstance, g_hWnd, TRUE);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// 無限ループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if((dwCurrentTime - dwExecLastTime) >= (1000 / GAME_FPS))
			{
				dwExecLastTime = dwCurrentTime;

				// 各種更新処理
				CManager::Update();

				// 各種描画処理
				CManager::Draw();

				dwFrameCount++;
			}
		}
	}
	UnregisterClass("NullWindow", wcex.hInstance);

	// 各種終了処理
	CManager::Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
//	関数名	:WndProc
//	引数	:HINSTANCE hWnd(ウィンドウハンドル)
//			:UINT uMsg
//			:WPARAM wParam
//			:LPARAM lParam
//	戻り値	:LRESULT
//	説明	:ウィンドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int	nID;

	const RECT rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };// 画面幅調整用

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "ウィンドウを閉じますか？", "ウィンドウの終了", MB_YESNO);
			if(nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_MOUSEMOVE:			// マウスが動いた
		break;

	case WM_LBUTTONDOWN:		// マウス左ボタンが押された
		if(!CInput::GetMousePress(MBTN_CENTER) && !CInput::GetMousePress(MBTN_RIGHT))
		{
			CInput::SetMouseLButton(true);
		}
		break;

	case WM_LBUTTONUP:			// マウス左ボタンが離された
		CInput::SetMouseLButton(false);
		break;

	case WM_MBUTTONDOWN:		// マウス中央ボタンが押された
		if(!CInput::GetMousePress(MBTN_LEFT) && !CInput::GetMousePress(MBTN_RIGHT))
		{
			CInput::SetMouseCButton(true);
		}
		break;

	case WM_MBUTTONUP:			// マウス中央ボタンが離された
		CInput::SetMouseCButton(false);
		break;

	case WM_RBUTTONDOWN:		// マウス右ボタンが押された
		if(!CInput::GetMousePress(MBTN_LEFT) && !CInput::GetMousePress(MBTN_CENTER))
		{
			CInput::SetMouseRButton(true);
		}
		break;

	case WM_RBUTTONUP:			// マウス右ボタンが離された
		CInput::SetMouseRButton(false);
		break;
	case WM_MOUSEWHEEL:
		{
			DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);		// 同時に押されているキー情報
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// 回転量
			
			// 前回の端数を追加
			zDelta += CInput::GetMouseFriction();

			// ノッチ数を求める
			CInput::SetMouseNotch(zDelta / WHEEL_DELTA);

			// 端数を保存する
			CInput::SetMouseFraction(zDelta % WHEEL_DELTA);
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
//	関数名	:GetFPS
//	引数	:無し
//	戻り値	:int
//	説明	:FPS値をリターンする。
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//=============================================================================
//	関数名	:GethWnd
//	引数	:無し
//	戻り値	:HWND
//	説明	:ウィンドウハンドルをリターンする。
//=============================================================================
HWND GethWnd(void)
{
	return g_hWnd;
}