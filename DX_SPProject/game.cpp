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

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
int			CGame::m_Frame;
CMeshfield	*CGame::m_Meshfield;
CRailLine	*CGame::m_RailLine;
CPlayer		*CGame::m_Player1;
CPlayer		*CGame::m_Player2;
CDriftMark	*CGame::m_DriftMark;

int			CGame::m_GoalCount = GOAL_COUNT;

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
	CCylinder::Create();
	m_RailLine = CRailLine::Create(0);
	//m_RailLine->ChangeDrawFrag();
	CRail::Create(0);
	//CRail::Create(1);
	m_Player1 = CPlayer::Create();
	CGoal::Create();
	
	// 2D
	CDriftMark::Create();
	CSpeedmeter::Create(100.0f, D3DXVECTOR3((SCREEN_WIDTH * 0.12f), (SCREEN_HEIGHT * 0.8f), 0.0f));
	CPause::Create();
	CFarGoal::Create((int)RAILLINE_LENGTH, D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.05f, 0.0f),
		D3DXVECTOR2((250.f * WINDOW_ASPECT_X), (60.f * WINDOW_ASPECT_Y)));
	CMap::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.9f), (SCREEN_HEIGHT * 0.9f), 0.0f));
	CCountdown::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f),
		D3DXVECTOR2((250.f * WINDOW_ASPECT_X), (60.f * WINDOW_ASPECT_Y)));

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
	// 3D
	// �V�[���X�V
	CSceneDX::UpdateAll();

	if(m_Player1->GetPerSpline() >= RAILLINE_GOAL)
	{
		// �S�[�����Ă�����J�E���^�����炷
		if(m_GoalCount > 0) m_GoalCount--;

		// ���U���g�Ƀt�F�[�h
		if(m_GoalCount == 0) CFade::Start(new CResult, FS_OUT);
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
	// �V�[���`��
	CSceneDX::DrawAll();
}