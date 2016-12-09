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
//	�ÓI�����o�ϐ�
//=============================================================================
CMeshfield	*CGame::m_Meshfield;
CRailLine	*CGame::m_RailLine;
CPlayer		*CGame::m_Player1;
CPlayer		*CGame::m_Player2;
CDriftMark	*CGame::m_DriftMark;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
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

	// BGM�Đ�
	CSound::Play(SOUNDLABEL_BGM000);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CGame::Uninit(void)
{
	// ���\�[�X�̃A�����[�h
	SafetyUninit(m_DriftMark);

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
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CGame::Draw(void)
{
	// �V�[���`��
	CSceneDX::DrawAll();

	// 2D
	m_DriftMark->CDriftMark::Draw();
}