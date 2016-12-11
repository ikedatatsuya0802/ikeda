//=============================================================================
//
//	タイトル	タイトル処理
//	ファイル名	title.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "title.h"
#include "manager.h"
#include "cameraDX.h"
#include "lightDX.h"
#include "input.h"
#include "fade.h"
#include "scene2DDX.h"
#include "player_title.h"
#include "rail_title.h"
#include "skybox.h"
#include "meshfield.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CTitle::Init(void)
{
	/*
	m_TitleBG = CScene2DDX::Create(true, 2, OBJTYPE_NONE,
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "title000.jpg");
		*/
	m_PushEnter = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.8f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), "pushbutton000.png");

	m_Alpha = 0.0f;
	

	m_Frame = 0;

	CManager::GetCamera()->Init();
	CLightDX::ChangeHolLight(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 3D
	CMeshfield::Create();
	CSkybox::Create(true, 2, OBJTYPE_NONE, true);
	CRail_Title::Create();
	CPlayer_Title::Create(true, 2, OBJTYPE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -(MESHFIELD_TOTALHEIGHT * 0.5f)));
	//CPlayer_Title::Create(true, 2, OBJTYPE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 100.0f));
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CTitle::Uninit(void)
{
	CSceneDX::DeleteAll();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CTitle::Update(void)
{
	// シーン更新
	CSceneDX::UpdateAll();
	if(CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetKeyTrigger(DIK_SPACE))
	{
		CFade::Start(new CTutorial, FS_OUT);
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CTitle::Draw(void)
{
	m_Alpha += 0.05f;
	m_PushEnter->SetColor((cosf(m_Alpha) * 0.5f + 0.5f), 1.0f, 1.0f, 1.0f);

	// シーン描画
	CSceneDX::DrawAll();
}