//=============================================================================
//
//	タイトル	マネージャ
//	ファイル名	manager.cpp
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
#include "number.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
CMode*		CManager::m_Mode;
CCameraDX*	CManager::m_Camera;
bool		CManager::m_ifEdhitMode;

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_ifEdhitMode = false;

	// レンダラ―読み込み
	CRendererDX::Init(hInstance, hWnd, TRUE);

	// 各種インスタンス生成
#ifdef _DEBUG
	m_Mode = new CTitle();
#else
	m_Mode = new CGame();
#endif // DEBUG
	m_Camera = new CCameraDX();

	// 初期化処理
	CInput::Init(hInstance, hWnd);
	CSound::Init();
	CLightDX::Init();
	m_Mode->Init();
	m_Camera->Init();
	CFade::Init();
	CDebugProc::Init();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CManager::Uninit(void)
{
	CInput::Uninit();
	CRendererDX::Uninit();
	CDebugProc::Uninit();
	SafetyUninit(m_Camera);
	
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
	CInput::Update();

	m_Mode->Update();
	CFade::Update();
	
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

	// カメラの設定
	m_Camera->SetCamera();

	// シーン描画
	m_Mode->Draw();

	// フェード描画
	CFade::Draw();

#ifdef _DEBUG
	// デバッグプロシージャ
	if(CManager::GetEdhitMode())
		CDebugProc::Draw();
#endif
	CDebugProc::Clear();
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

//=============================================================================
//	関数名	:GetClientWindowSize
//	引数	:無し
//	戻り値	:無し
//	説明	:ウィンドウのクライアントサイズを取得する。
//=============================================================================
D3DXVECTOR2 CManager::GetClientWindowSize(void)
{
	D3DXVECTOR2 size;
	RECT rect;
	GetClientRect(GethWnd(), &rect);
	size.x = (float)(rect.right - rect.left);
	size.y = (float)(rect.bottom - rect.top);
	return size;
}

//=============================================================================
//	関数名	:GetCWAspect
//	引数	:無し
//	戻り値	:無し
//	説明	:想定している初期ウィンドウサイズに対しての比率を返す。
//=============================================================================
D3DXVECTOR2 CManager::GetCWAspect(void)
{
	D3DXVECTOR2 size;
	size.x = GetClientWindowSize().x / DEFAULT_SCREEN_WIDTH;
	size.y = GetClientWindowSize().y / DEFAULT_SCREEN_HEIGHT;
	return size;
}