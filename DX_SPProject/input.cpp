//=============================================================================
//
//	タイトル	シーンファイル(3D)
//	ファイル名	input.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "input.h"
#include "main.h"
#include "debugProc.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECTINPUT8			CInput::m_pInput;				// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	CInput::m_pDevKeyboard;			// 入力3Dデバイス(キーボード)へのポインタ

BYTE	CInput::m_aKeyState[KEYSTATE_NUM];				// キーボードの入力情報ワーク
BYTE	CInput::m_aKeyStateTrigger[KEYSTATE_NUM];		// キーボードのトリガ情報
BYTE	CInput::m_aKeyStateRelease[KEYSTATE_NUM];		// キーボードのリリース情報
BYTE	CInput::m_aKeyStateRepeat[KEYSTATE_NUM];		// キーボードのリピート情報

int		CInput::m_aKeyStateRepeatCount[KEYSTATE_NUM];	// キーボードのリピートカウント
int		CInput::m_nRepeatInterval;						// リピート間隔

LPDIRECTINPUTDEVICE8 CInput::m_pDevMouse;				// 入力3Dデバイス(マウス)へのポインタ
MSTATE	CInput::m_MState;								// マウス情報

BYTE	CInput::m_aMouseState;							// マウスの左ボタン入力情報ワーク
BYTE	CInput::m_aMouseStateTrigger;					// マウスの左ボタントリガ情報
BYTE	CInput::m_aMouseStateRelease;					// マウスの左ボタンリリース情報
BYTE	CInput::m_aMouseStateRepeat;					// マウスの左ボタンリピート情報

int		CInput::m_aMouseRepeatCount;					// キーボードのリピートカウント

//=============================================================================
//	関数名	:InitInput
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 変数初期化
	m_pInput = NULL;
	m_pDevKeyboard = NULL;

	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
	memset(m_aKeyStateRepeat, 0, sizeof(m_aKeyStateRepeat));

	memset(m_aKeyStateRepeatCount, 0, sizeof(m_aKeyStateRepeatCount));
	m_nRepeatInterval = NULL;

	if(m_pInput == NULL)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			MessageBox(hWnd, "ウィンドウを閉じますか？", "ウィンドウの終了", MB_YESNO);
			return E_FAIL;
		}
	}

	InitKeyboard(hInstance, hWnd);
	InitMouse(hInstance, hWnd);

	return S_OK;
}

//=============================================================================
//	関数名	:UnInitInput
//	引数	:無し
//	戻り値	:無し
//	説明	:入力の終了処理を行う。
//=============================================================================
void CInput::Uninit(void)
{
	SafetyRelease(m_pInput);

	UninitKeyboard();
	UninitMouse();
}

//=============================================================================
//	関数名	:UpdateInput
//	引数	:無し
//	戻り値	:無し
//	説明	:入力の更新処理を行う。
//=============================================================================
void CInput::Update(void)
{
	UpdateKeyboard();
	UpdateMouse();
}

//=============================================================================
//	関数名	:InitKeyboard
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CInput::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	if(FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))							// 3Dデバイスの生成
	{
		MessageBox(hWnd, "m_pInput->CreateDeviceに失敗しました", "E_FAILが返されました", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))											// データフォーマットの設定
	{
		MessageBox(hWnd, "m_pDevKeyboard->SetDataFormatに失敗しました", "E_FAILが返されました", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))		// 協調モードの設定
	{
		MessageBox(hWnd, "m_pDevKeyboard->SetCooperativeLevelに失敗しました", "E_FAILが返されました", MB_OK);
		return E_FAIL;
	}
	m_pDevKeyboard->Acquire();		// アクセス権を取得

	// キーボードのリピートカウント初期化
	memset(m_aKeyStateRepeatCount, 0, sizeof(m_aKeyStateRepeatCount));

	// リピート間隔の初期化
	m_nRepeatInterval = REPEAT_INTERVAL_NORMAL;

	return S_OK;
}

//=============================================================================
//	関数名	:UpdateKeyboard
//	引数	:無し
//	戻り値	:無し
//	説明	:入力の更新処理を行う。
//=============================================================================
void CInput::UpdateKeyboard(void)
{
	BYTE aKeyState[KEYSTATE_NUM];

	if(SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for(int nCntKey = 0 ; nCntKey < KEYSTATE_NUM ; nCntKey++)
		{
			// トリガ処理
			m_aKeyStateTrigger[nCntKey] = aKeyState[nCntKey] & (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]);
			
			// リリース処理
			m_aKeyStateRelease[nCntKey] = m_aKeyState[nCntKey] & (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]);

			// リピート処理
			if(aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCount[nCntKey]++;
				if(m_aKeyStateRepeatCount[nCntKey] > REPEAT_TIME)
				{
					// リピート間隔によってリピートを行う
					if(m_aKeyStateRepeatCount[nCntKey] % m_nRepeatInterval == 0)
					{
						m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
					}
					else
					{
						m_aKeyStateRepeat[nCntKey] = 0;
					}
				}
			}
			else if(m_aKeyStateRelease[nCntKey])
			{
				m_aKeyStateRepeatCount[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();
	}
}

//=============================================================================
//	関数名	:UnInitKeyboard
//	引数	:無し
//	戻り値	:無し
//	説明	:入力の終了処理を行う。
//=============================================================================
void CInput::UninitKeyboard(void)
{
	if(m_pDevKeyboard != NULL)
	{
		m_pDevKeyboard->Unacquire();	// アクセス権の開放
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}
}

//=============================================================================
//	関数名	:InitMouse
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CInput::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// マウスポインタ情報の初期化
	SetRect(&m_MState.moveRect, 10, 10, (int)SCREEN_WIDTH-10, (int)SCREEN_HEIGHT-10);		// マウスカーソルの動く範囲
	m_MState.x = m_MState.moveRect.left;									// マウスカーソルのＸ座標を初期化
	m_MState.y = m_MState.moveRect.top;										// マウスカーソルのＹ座標を初期化
	m_MState.lButton = false;												// 左ボタンの情報を初期化
	m_MState.rButton = false;												// 右ボタンの情報を初期化
	m_MState.cButton = false;												// 中央ボタンの情報を初期化
	m_MState.moveAdd = 2;													// マウスカーソルの移動量を設定
	SetRect(&m_MState.imgRect, 400, 0, 420, 20);							// マウスカーソル画像の矩形を設定
	m_MState.imgWidth = m_MState.imgRect.right - m_MState.imgRect.left;		// 画像の幅を計算
	m_MState.imgHeight = m_MState.imgRect.bottom - m_MState.imgRect.top;	// 画像の高さを計算


	return S_OK;
}

//=============================================================================
//	関数名	:UpdateKeyboard
//	引数	:無し
//	戻り値	:無し
//	説明	:入力の更新処理を行う。
//=============================================================================
void CInput::UpdateMouse(void)
{
	CDebugProc::DebugProc("マウス:(%d, %d)\n", m_MState.x, m_MState.y);
	CDebugProc::DebugProc("MD:(%d, %d, %d)\n", m_MState.lButton, m_MState.cButton, m_MState.rButton);
}

//=============================================================================
//	関数名	:UnInitKeyboard
//	引数	:無し
//	戻り値	:無し
//	説明	:入力の終了処理を行う。
//=============================================================================
void CInput::UninitMouse(void)
{
	if(m_pDevMouse != NULL)
	{
		m_pDevMouse->Unacquire();	// アクセス権の開放
		m_pDevMouse->Release();
		m_pDevMouse = NULL;
	}
}