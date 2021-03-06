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
#include "DebugProc.h"
#include "rendererDX.h"
#include "cameraDX.h"
#include "lightDX.h"
#include "skybox.h"
#include "model.h"
#include "input.h"
#include "particle.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
CCameraDX	*CManager::m_Camera;
CModel		*CManager::m_Model;
int			CManager::m_Frame;

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

	// リソースのロード
	CParticle::Load();

	// 各種インスタンス生成
	m_Camera		= new CCameraDX();

	// 初期化処理
	CInput::Init(hInstance, hWnd);
	CLightDX::Init();
	m_Camera->Init();
	CDebugProc::Init();

	CSkybox::Create();

	m_Model = CModel::Create("", "human");
	//CModel::Create("", "car");

	m_Frame = 0;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CManager::Uninit(void)
{
	CRendererDX::Uninit();
	CDebugProc::Uninit();
	SafetyUninit(m_Camera);

	CSceneDX::DeleteAll();
	CLightDX::Uninit();

	CInput::Uninit();


	// リソースのアンロード
	CParticle::Unload();
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

	CRendererDX::Update();
	m_Camera->Update();


	int frame = CManager::GetFrame() % CManager::GetModel()->GetMorphCountAll();

	// パーティクル生成
	if((frame >= 300) && (frame < 540))
	{
		if(frame % 3 == 0)
		CParticle::Create(0);
	}
	else if((frame >= 600) && (frame < 900))
	{
		if(frame % 3 == 0)
			CParticle::Create(1);
	}
	else if((frame >= 930) && (frame < 1200))
	{
		if(frame % 3 == 0)
			CParticle::Create(2);
	}
	else if((frame >= 1200) && (frame < 1500))
	{
		if(frame % 2 == 0)
			CParticle::Create(3);
	}

	// シーン更新
	CSceneDX::UpdateAll();

	CDebugProc::Update();
	CLightDX::Update();

	m_Frame++;
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
	CLightDX::Draw();

	// カメラの設定
	m_Camera->SetCamera();

	// シーン描画
	CSceneDX::DrawAll();

	// デバッグプロシージャ
	CDebugProc::Draw();
#ifdef _DEBUG
	CDebugProc::DebugProc("FPS:%d\n", GetFPS());
#endif

	// 描画シーケンス終了
	CRendererDX::End();
}