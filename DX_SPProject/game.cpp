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
#include "sceneBillboardDX.h"
#include "sceneXDX.h"
#include "meshfield.h"
#include "cylinder.h"
#include "skybox.h"
#include "player.h"
#include "gametime.h"
#include "railLine.h"
#include "rail.h"
#include "orbit.h"

//=============================================================================
//	�v���g�^�C�v
//=============================================================================
static int		g_nCountFPS;				// FPS�J�E���^

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
CMeshfield	*CGame::m_Meshfield;		// ���b�V���t�B�[���h
CRailLine	*CGame::m_RailLine;		// ���b�V���t�B�[���h
CPlayer		*CGame::m_Player1;		// �����̃v���C���[
CPlayer		*CGame::m_Player2;		// ���̒N���̃v���C���[

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CGame::Init(void)
{
	// ���\�[�X�̃��[�h

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

	CGametime::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f + 200.0f), 100.0f, 0.0f), D3DXVECTOR2(250.0f, 100.0f), FIGURE(3));
	
	//CScene3DDX* f = CScene3DDX::Create();
	//f->SetRot(D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f));

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

	// �I�������E�C���X�^���X�폜
	SafetyUninit(m_Meshfield);
	//SafetyUninit(m_RailLine);
	//SafetyUninit(m_Player1);

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
	// �V�[���X�V
	CSceneDX::UpdateAll();
	if(CInput::GetKeyTrigger(DIK_RETURN))
	{
		CFade::Start(new CResult, FS_OUT);
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
}