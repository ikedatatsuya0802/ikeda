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
#include "lightDX.h"
#include "input.h"
#include "fade.h"
#include "sceneDX.h"
#include "scene2DDX.h"
#include "scene3DDX.h"
#include "sceneXDX.h"
#include "pause.h"
#include "meshfield.h"
#include "cylinder.h"
#include "skybox.h"
#include "player.h"
#include "gametime.h"
#include "railLine.h"
#include "rail.h"
#include "orbit.h"
#include "driftMark.h"
#include "speedmeter.h"
#include "farGoal.h"
#include "goal.h"
#include "map.h"
#include "countdown.h"
#include "pillar.h"
#include "EStructure.h"
#include "wiring.h"
#include "building.h"
#include "time.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
int			CGame::m_Frame;
CMeshfield	*CGame::m_Meshfield;
CRailLine	*CGame::m_RailLine;
CPlayer		*CGame::m_Player1;
CPlayer		*CGame::m_Player2;
CDriftMark	*CGame::m_DriftMark;
CScene2DDX	*CGame::m_Hakushin;
CPause		*CGame::m_Pause;

int			CGame::m_GoalCount		= GOAL_COUNT;
int			CGame::m_GameState		= GAMESTATE_COUNT;

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CGame::Init(void)
{
	CLightDX::ChangeHolLight(0, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	// 3D
	m_Meshfield	= CMeshfield::Create();
	CSkybox::Create();
	//CCylinder::Create();
	m_RailLine = CRailLine::Create(0);
	//m_RailLine->ChangeDrawFrag();
	CPillar::Create();
	CEStructure::Create();
	CWiring::Create();
	CBuilding::Init();
	CRail::Create(0);
	//CRail::Create(1);
	m_Player1 = CPlayer::Create();
	CGoal::Create();
	
	// 2D
	CDriftMark::Create();
	m_Hakushin = CScene2DDX::Create(".\\data\\TEXTURE\\hakushin.png",
		D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 1.5f), 0.0f);
	m_Hakushin->SetColor(0.0f);
	CSpeedmeter::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.12f), (SCREEN_HEIGHT * 0.8f), 0.0f));
	CFarGoal::Create((int)RAILLINE_LENGTH, D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.05f, 0.0f),
		D3DXVECTOR2((250.f * WINDOW_ASPECT_X), (60.f * WINDOW_ASPECT_Y)));
	CMap::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.9f), (SCREEN_HEIGHT * 0.9f), 0.0f));
	CCountdown::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f),
		D3DXVECTOR2((250.f * WINDOW_ASPECT_X), (60.f * WINDOW_ASPECT_Y)));
	m_Pause = CPause::Create();
	m_Pause->UnlinkList();

	// フレーム初期化
	m_Frame = -1;

	// BGM再生
	//CSound::Play(SOUNDLABEL_BGM000);
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CGame::Uninit(void)
{
	CBuilding::Uninit();

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
	static CTime* time;

	switch(m_GameState)
	{
	case GAMESTATE_COUNT:

		// シーン更新
		CSceneDX::UpdateAll();

		if(m_Frame > START_COUNT)
		{// カウントダウン終了

			time = CTime::Create(0, D3DXVECTOR3(SCREEN_WIDTH * 0.18f, SCREEN_HEIGHT * 0.05f, 0.0f), D3DXVECTOR2(400.0f, 60.0f));
			time->CountStart();
			m_GameState++;
		}
		break;
	case GAMESTATE_RACE:

		m_Pause->Update();
		
		// ポーズされていない場合のみ実行
		if(!m_Pause->GetPause())
		{
			// シーン更新
			CSceneDX::UpdateAll();

			CBuilding::Update();
		}

		if(m_Player1->GetPerSpline() >= RAILLINE_GOAL)
		{// ゴールした場合

			// 時間を保存
			time->CountStop();
			time->SaveTime();

			// フェーズ以降
			m_GameState++;
		}
		break;
	case GAMESTATE_GOAL:

		// シーン更新
		CSceneDX::UpdateAll();

		// ゴールしていたらカウンタを減らす
		if(m_GoalCount > 0) m_GoalCount--;

		// リザルトにフェード
		if(m_GoalCount == 0) CFade::Start(new CResult, FS_OUT);

		if(0) m_GameState = 0;

		break;
	default:
		break;
	}

	// フレーム加算
	m_Frame++;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CGame::Draw(void)
{
	CBuilding::Draw();

	// シーン描画
	CSceneDX::DrawAll();
	
	if(m_Pause->GetPause())
	{
		m_Pause->Draw();
	}
}