//=============================================================================
//
//	タイトル	ゲーム処理
//	ファイル名	title.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "game.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sceneDX.h"
#include "scene2DDX.h"
#include "scene3DDX.h"
#include "sceneXDX.h"
#include "meshfield.h"
#include "cylinder.h"
#include "skybox.h"
#include "player.h"
#include "gametime.h"
#include "railLine.h"
#include "rail.h"
#include "orbit.h"
#include "driftMark.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
CMeshfield	*CGame::m_Meshfield;
CRailLine	*CGame::m_RailLine;
CPlayer		*CGame::m_Player1;
CPlayer		*CGame::m_Player2;
CDriftMark	*CGame::m_DriftMark;

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CGame::Init(void)
{
	// 3D
	m_Meshfield	= CMeshfield::Create();
	CSkybox::Create();
	CCylinder::Create();
	m_RailLine = CRailLine::Create(0);
	//m_RailLine->ChangeDrawFrag();
	CRail::Create(0);
	//CRail::Create(1);
	m_Player1 = CPlayer::Create();
	COrbit *orbit1 = COrbit::Create();
	COrbit *orbit2 = COrbit::Create();
	orbit1->SetLocPos(D3DXVECTOR3(-6.0f, 22.0f, 72.0f), D3DXVECTOR3(-8.0f, 22.0f, 72.0f));
	orbit2->SetLocPos(D3DXVECTOR3(6.0f, 22.0f, 72.0f), D3DXVECTOR3(8.0f, 22.0f, 72.0f));

	//CGametime::Create(true, 2, OBJTYPE_NONE, D3DXVECTOR3((SCREEN_WIDTH * 0.5f + 200.0f), 100.0f, 0.0f), D3DXVECTOR2(250.0f, 100.0f), FIGURE(3));

	// 2D
	m_DriftMark = CDriftMark::Create();

	// BGM再生
	CSound::Play(SOUNDLABEL_BGM000);
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CGame::Uninit(void)
{
	// リソースのアンロード
	SafetyUninit(m_DriftMark);

	// 終了処理・インスタンス削除
	CSceneDX::DeleteAll();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CGame::Update(void)
{
	// 3D
	// シーン更新
	CSceneDX::UpdateAll();

	// 2D
	m_DriftMark->CDriftMark::Update();

	if(CInput::GetKeyTrigger(DIK_RETURN))
	{
		CFade::Start(new CResult, FS_OUT);
	}
	if(KT_J)
	{
		CDriftMark::VisibleDriftMark(true, true, 60);
	}
	if(KT_K)
	{
		CDriftMark::InvisibleDriftMark(30);
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CGame::Draw(void)
{
	// シーン描画
	CSceneDX::DrawAll();

	// 2D
	m_DriftMark->CDriftMark::Draw();
}