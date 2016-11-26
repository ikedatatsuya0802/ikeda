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
#include <process.h>
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
#include "solari.h"
#include "rail.h"
#include "sound.h"

//=============================================================================
//	�v���g�^�C�v
//=============================================================================
static int		g_nCountFPS;				// FPS�J�E���^

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
CMeshfield	*CGame::m_Meshfield;		// ���b�V���t�B�[���h
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
	CMeshfield	::Load();
	CSkybox		::Load();
	CCylinder	::Load();
	CRail		::Load();
	CNumber		::Load();

	//CScene2DDX::Create(D3DXVECTOR3(1200.0f, 100.0f, 0.0f));
	//CScene3DDX::Create();
	//CSceneBillboardDX::Create();
	m_Meshfield	= CMeshfield::Create();
	CSkybox::Create();
	CCylinder::Create();
	CRail::Create(0);
	//CRail::Create(1);
	m_Player1 = CPlayer::Create();
	CGametime::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f + 200.0f), 100.0f, 0.0f), D3DXVECTOR2(250.0f, 100.0f), FIGURE(3));
	
	//CSolari::Create(D3DXVECTOR3(0.0f, 200.0f, 0.0f));

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
	CMeshfield	::Unload();
	CSkybox		::Unload();
	CCylinder	::Unload();
	CRail		::Unload();
	CNumber		::Unload();

	// �I�������E�C���X�^���X�폜
	if(m_Meshfield != NULL)
	{
		m_Meshfield->Uninit();
		delete m_Meshfield;
		m_Meshfield = NULL;
	}
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
	if(CInput::GetKeyboardTrigger(DIK_RETURN))
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