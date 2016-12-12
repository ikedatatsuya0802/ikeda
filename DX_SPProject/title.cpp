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
int	CTitle::m_Frame;

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CTitle::Init(void)
{
	m_Alpha = 0.0f;
	m_AlphaBlack = 0.0f;
	m_AlphaButton = 0.0f;

	m_TitleBG = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "titlelogo.png");
	m_TitleBG->SetColor(0.0f, 1.0f, 1.0f, 1.0f);
	m_PushEnter = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.8f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), "pushbutton000.png");
	m_PushEnter->SetColor(m_AlphaButton, 1.0f, 1.0f, 1.0f);
	m_Black = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "black.jpg");
	m_Black->SetColor(0.0f, 1.0f, 1.0f, 1.0f);

	m_Frame = -1;

	CManager::GetCamera()->Init();
	CLightDX::ChangeHolLight(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 3D
	CMeshfield::Create();
	CSkybox::Create(true, 2, OBJTYPE_NONE, true);
	CRail_Title::Create();
	CPlayer_Title::Create(true, 2, OBJTYPE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -(30000.0f)));
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

	m_Frame++;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CTitle::Draw(void)
{
	if(m_Frame > TITLE_START_FRAME)
	{
		m_Alpha += 0.05f;
		if(m_Alpha > 1.0f) m_Alpha = 1.0f;
	}

	if(m_Frame % 4141 > 4000)
	{
		m_AlphaBlack += 0.05f;
		if(m_AlphaBlack > 1.0f) m_AlphaBlack = 1.0f;
		m_Alpha -= 0.05f;
		if(m_Alpha < 0.0f) m_Alpha = 0.0f;
	}
	else
	{
		m_AlphaBlack -= 0.05f;
		if(m_AlphaBlack < 0.0f) m_AlphaBlack = 0.0f;
	}

	if(m_Frame > TITLE_START_FRAME)
	{
		// ボタン押下表示を点滅
		if(m_Frame % PUSHBUTTON_BRIGHT > (PUSHBUTTON_BRIGHT / 2))
		{
			m_AlphaButton += (1.0f / (PUSHBUTTON_BRIGHT / 2));
			if(m_AlphaButton > 1.0f) m_AlphaButton = 1.0f;
		}
		else
		{
			m_AlphaButton -= (1.0f / (PUSHBUTTON_BRIGHT / 2));
			if(m_AlphaButton < 0.0f) m_AlphaButton = 0.0f;
		}
	}

	m_Black->SetColor(m_AlphaBlack, 1.0f, 1.0f, 1.0f);
	m_TitleBG->SetColor(m_Alpha, 1.0f, 1.0f, 1.0f);
	m_PushEnter->SetColor(m_AlphaButton, 1.0f, 1.0f, 1.0f);

	// シーン描画
	CSceneDX::DrawAll();
}