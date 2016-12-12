//=============================================================================
//
//	タイトル	タイトル処理
//	ファイル名	result.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene2DDX.h"

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CResult::Init(void)
{
	m_ResultBG = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "result000.jpg");
	m_PushEnter = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.8f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), "pushbutton000.png");

	m_Alpha = 0.0f;
	m_Frame = -1;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CResult::Uninit(void)
{
	CSceneDX::DeleteAll();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CResult::Update(void)
{
	// シーン更新
	CSceneDX::UpdateAll();
	if(CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetKeyTrigger(DIK_SPACE))
	{
		CFade::Start(new CTitle, FS_OUT);
	}

	m_Frame++;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CResult::Draw(void)
{
	// ボタン押下表示を点滅
	if(m_Frame % PUSHBUTTON_BRIGHT > (PUSHBUTTON_BRIGHT / 2))
	{
		m_Alpha += (1.0f / (PUSHBUTTON_BRIGHT / 2));
		if(m_Alpha > 1.0f) m_Alpha = 1.0f;
	}
	else
	{
		m_Alpha -= (1.0f / (PUSHBUTTON_BRIGHT / 2));
		if(m_Alpha < 0.0f) m_Alpha = 0.0f;
	}
	m_PushEnter->SetColor(m_Alpha, 1.0f, 1.0f, 1.0f);

	// シーン描画
	CSceneDX::DrawAll();
}