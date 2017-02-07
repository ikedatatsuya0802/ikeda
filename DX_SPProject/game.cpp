//=============================================================================
//
//	�^�C�g��	�Q�[������
//	�t�@�C����	title.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
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
//	�ÓI�����o�ϐ�
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
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
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

	// �t���[��������
	m_Frame = -1;

	// BGM�Đ�
	//CSound::Play(SOUNDLABEL_BGM000);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CGame::Uninit(void)
{
	CBuilding::Uninit();

	// �I�������E�C���X�^���X�폜
	CSceneDX::DeleteAll();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CGame::Update(void)
{
	static CTime* time;

	switch(m_GameState)
	{
	case GAMESTATE_COUNT:

		// �V�[���X�V
		CSceneDX::UpdateAll();

		if(m_Frame > START_COUNT)
		{// �J�E���g�_�E���I��

			time = CTime::Create(0, D3DXVECTOR3(SCREEN_WIDTH * 0.18f, SCREEN_HEIGHT * 0.05f, 0.0f), D3DXVECTOR2(400.0f, 60.0f));
			time->CountStart();
			m_GameState++;
		}
		break;
	case GAMESTATE_RACE:

		m_Pause->Update();
		
		// �|�[�Y����Ă��Ȃ��ꍇ�̂ݎ��s
		if(!m_Pause->GetPause())
		{
			// �V�[���X�V
			CSceneDX::UpdateAll();

			CBuilding::Update();
		}

		if(m_Player1->GetPerSpline() >= RAILLINE_GOAL)
		{// �S�[�������ꍇ

			// ���Ԃ�ۑ�
			time->CountStop();
			time->SaveTime();

			// �t�F�[�Y�ȍ~
			m_GameState++;
		}
		break;
	case GAMESTATE_GOAL:

		// �V�[���X�V
		CSceneDX::UpdateAll();

		// �S�[�����Ă�����J�E���^�����炷
		if(m_GoalCount > 0) m_GoalCount--;

		// ���U���g�Ƀt�F�[�h
		if(m_GoalCount == 0) CFade::Start(new CResult, FS_OUT);

		if(0) m_GameState = 0;

		break;
	default:
		break;
	}

	// �t���[�����Z
	m_Frame++;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CGame::Draw(void)
{
	CBuilding::Draw();

	// �V�[���`��
	CSceneDX::DrawAll();
	
	if(m_Pause->GetPause())
	{
		m_Pause->Draw();
	}
}