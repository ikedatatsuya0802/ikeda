//=============================================================================
//
//	タイトル	カメラファイル
//	ファイル名	light.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/26
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "manager.h"
#include "main.h"
#include "input.h"
#include "DebugProc.h"
#include "rendererDX.h"
#include "mode.h"
#include "fade.h"
#include "cameraDX.h"
#include "lightDX.h"
#include "sceneDX.h"
#include "sound.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
CMode		*CManager::m_Mode;			// モードクラス
CCameraDX	*CManager::m_Camera;		// カメラのインスタンス
CLightDX	*CManager::m_Light;			// ライトのインスタンス

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラ―読み込み
	CRendererDX::Init(hInstance, hWnd, TRUE);

	// 各種インスタンス生成
	m_Mode			= new CGame();
	m_Camera		= new CCameraDX();
	m_Light			= new CLightDX();

	// 初期化処理
	m_Mode->Init();
	CFade::Init();
	CInput::InitKeyboard(hInstance, hWnd);
	CInput::InitMouse(hInstance, hWnd);
	CDebugProc::Init();
	m_Camera->Init();
	m_Light->Init();
	CSound::Init();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CManager::Uninit(void)
{
	CInput::UninitKeyboard();
	CInput::UninitMouse();
	CRendererDX::Uninit();
	CDebugProc::Uninit();
	if(m_Camera != NULL)
	{
		m_Camera->Uninit();
		delete m_Camera;
		m_Camera = NULL;
	}
	
	CSceneDX::DeleteAll();
	CSound::Uninit();
	CFade::Uninit();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CManager::Update(void)
{
	m_Mode->Update();
	CFade::Update();
	
	CInput::UpdateKeyboard();
	CRendererDX::Update();
	m_Camera->Update();
	CDebugProc::Update();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CManager::Draw(void)
{
	// 描画シーケンス開始
	CRendererDX::Begin();

	// ライトベクトル情報
	m_Light->Draw();

	// カメラの設定
	m_Camera->SetCamera();

	// シーン描画
	m_Mode->Draw();

	// フェード描画
	CFade::Draw();

	// デバッグプロシージャ
	CDebugProc::Draw();
#ifdef _DEBUG
	CDebugProc::DebugProc("FPS:%d\n", GetFPS());
#endif

	// 描画シーケンス終了
	CRendererDX::End();
}

//=============================================================================
//	関数名	:SetMode
//	引数	:CMode *mode(クラスのインスタンス)
//	戻り値	:無し
//	説明	:モードを切り替える。
//=============================================================================
void CManager::SetMode(CMode *mode)
{
	if((m_Mode != NULL))
	{
		// モードの終了処理
		m_Mode->Uninit();
		// モードのインスタンスの削除
		delete m_Mode;
	}
	if(mode != NULL)
	{
		// モードの切り替え
		m_Mode = mode;
		// モードの初期処理
		m_Mode->Init();
	}
}